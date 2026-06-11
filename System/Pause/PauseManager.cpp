#define NOMINMAX
#include "PauseManager.h"
#include "GameInfo.h"
#include "Level/LevelChanger.h"
#include "Input/InputManager.h"
#include "Audio/AudioManager.h"
#include "DxLib.h"
/// <summary>
/// PauseManagerで使う定数
/// </summary>
namespace
{
    /// <summary>
    /// ポーズUIのパス
    /// </summary>
    const char* kPauseUiPath = "Assets/Image/UI/PauseUI.png";

    /// <summary>
    /// ポーズ中暗くする度合い
    /// </summary>
    const int kOverlayAlpha = 160;

    /// <summary>
    /// 白色を表示するための色の明るさ
    /// </summary>
    const int kColorBrightWhite = 255;

    /// <summary>
    /// 値を半分にする
    /// </summary>
    const float kHalf = 0.5f;

    /// <summary>
    /// 値を四分の一にする
    /// </summary>
    const float kQuarter = 0.25f;

    /// <summary>
    /// 点滅の間隔
    /// </summary>
    const float kBlinkInterval = 1.0f;

    /// <summary>
    /// 無操作でヒントを表示するまでの時間（秒）
    /// </summary>
    const float kIdleHintTime = 5.0f;

    /// <summary>
    /// ヒントテキスト
    /// </summary>
    const char* kHintText = "Escでポーズ・メニュー";

    /// <summary>
    /// ヒントテキスト（ゲームパッド用）
    /// </summary>
    const char* kHintTextGamepad = "右スティック押し込みでポーズ・メニュー";

    /// <summary>
    /// ヒントテキストのY座標（画面下部）
    /// </summary>
    const int kHintTextOffsetFromBottom = 40;

    /// <summary>
    /// 音量調整のフォントサイズ
    /// </summary>
    const int kVolumeFontSize = 28;

    /// <summary>
    /// 音量調整のフォントの太さ
    /// </summary>
    const int kVolumeFontThick = 4;

    /// <summary>
    /// 音量の変化量（左右キー1回分）
    /// </summary>
    const int kVolumeStep = 10;

    /// <summary>
    /// 音量の最小値
    /// </summary>
    const int kVolumeMin = 0;

    /// <summary>
    /// 音量の最大値
    /// </summary>
    const int kVolumeMax = 100;

    /// <summary>
    /// 音量UIのY座標オフセット（画面中央から）
    /// </summary>
    const int kBgmVolumeOffsetY = 20;

    /// <summary>
    /// SEの音量UIのY座標オフセット
    /// </summary>
    const int kSeVolumeOffsetY = 80;

    /// <summary>
    /// 選択中の項目の色（黄色）
    /// </summary>
    const int kSelectedR = 255;
    const int kSelectedG = 255;
    const int kSelectedB = 0;

    /// <summary>
    /// ポーズ中に表示するテキスト
    /// </summary>
    const char* kPauseText = "PAUSE (Press Escape)";

    /// <summary>
    /// ポーズ中に表示するテキスト
    /// </summary>
    const char* kPauseTextGamePad = "PAUSE（Press Right Stick）";

    /// <summary>
    /// ポーズ中に表示するテキストのX座標の微調整
    /// </summary>
    const int kPauseStringAdjustX = 30;

    /// <summary>
    /// BGM音量の表示フォーマット
    /// </summary>
    const char* kBgmVolumeFormat = "BGM音量: %d  ← →で調整";

    /// <summary>
    /// SE音量の表示フォーマット
    /// </summary>
    const char* kSeVolumeFormat = "SE音量:  %d  ← →で調整";

    /// <summary>
    /// 音量項目の選択ガイドテキスト
    /// </summary>
    const char* kVolumeSelectGuideText = "↑↓で項目選択";

    /// <summary>
    /// 音量調整フォントの名前
    /// </summary>
    const char* kVolumeFontName = "Yu Mincho Demibold";

    /// <summary>
    /// 音量UIの表示X座標（画面中央からの左方向オフセット）
    /// </summary>
    const int kVolumeUiOffsetX = 150;

    /// <summary>
    /// 選択ガイドテキストのSE行からの追加Y方向オフセット
    /// </summary>
    const int kVolumeGuideSpacingY = 50;

    /// <summary>
    /// タイトルへ戻るガイドテキスト
    /// </summary>
    const char* kReturnToTitleGuideText = "Return to Title : Press Q";

    /// <summary>
    /// タイトルへ戻るガイドテキスト（ゲームパッド用）
    /// </summary>
    const char* kReturnToTitleGuideTextGamepad = "Return to Title : Press Left Stick";

    /// <summary>
    /// タイトルへ戻るガイドテキストのX方向追加オフセット
    /// </summary>
    const int kReturnToTitleGuideSpacingX = 50;

    /// <summary>
    /// タイトルへ戻るガイドテキストのY方向追加オフセット
    /// </summary>
    const int kReturnToTitleGuideSpacingY = 150;

    /// <summary>
    /// ヒントテキストの右端から内側へのオフセット
    /// </summary>
    const int kHintTextOffsetFromRight = 20;
}

/// <summary>
/// 毎フレームの更新処理
/// </summary>
void PauseManager::Update(float delta_time)
{
    if (input_ == nullptr)
    {
        return;
    }

    //いずれかのキーが押されたら無操作タイマーをリセット
    const bool is_any_key =
        input_->IsPushThisFrame(InputManager::GameKeyKind::kLeft) ||
        input_->IsPushThisFrame(InputManager::GameKeyKind::kRight) ||
        input_->IsPushThisFrame(InputManager::GameKeyKind::kPlayerJump) ||
        input_->IsPushThisFrame(InputManager::GameKeyKind::kPlayerRun) ||
        input_->IsPushThisFrame(InputManager::GameKeyKind::kPlayerCloseAttack) ||
        input_->IsPushThisFrame(InputManager::GameKeyKind::kPlayerLongRangeAttack) ||
        input_->IsPushThisFrame(InputManager::GameKeyKind::kPause);

    if (is_any_key)
    {
        idle_timer_ = 0.0f;
    }
    else
    {
        idle_timer_ += delta_time;
    }

    //5秒経過でヒント表示
    is_showing_hint_ = (idle_timer_ >= kIdleHintTime) && !is_paused_ && is_hint_enabled_;

    current_hint_text_ = is_gamepad_connected_ ? kHintTextGamepad : kHintText;
    current_pause_text_ = is_gamepad_connected_ ? kPauseTextGamePad : kPauseText;

    current_return_to_title_text_ = is_gamepad_connected_
        ? kReturnToTitleGuideTextGamepad
        : kReturnToTitleGuideText;

    //ポーズ切り替え
    if (input_->IsPushThisFrame(InputManager::GameKeyKind::kPause))
    {
        is_paused_ = !is_paused_;
    }

    if (!is_paused_)
    {
        blink_interval_timer_ = 0.0f;
        return;
    }

    if (input_->IsPushThisFrame(InputManager::GameKeyKind::kQuit))
    {
        //ポーズ状態の解除
        is_paused_ = false;

        LevelChanger::GetInstance()->SetLevelChangerState(
            LevelChanger::LevelChangerState::kFinGameLevel);
    }


    //点滅
    blink_interval_timer_ += delta_time;
    if (blink_interval_timer_ >= kBlinkInterval)
    {
        blink_interval_timer_ -= kBlinkInterval;
        is_visible_string_ = !is_visible_string_;
    }

    //音量項目の上下選択
    if (input_->IsPushThisFrame(InputManager::GameKeyKind::kUp))
    {
        volume_select_index_ = 0; // BGM
    }
    if (input_->IsPushThisFrame(InputManager::GameKeyKind::kDown))
    {
        volume_select_index_ = 1; // SE
    }

    //音量の左右調整
    if (input_->IsPushThisFrame(InputManager::GameKeyKind::kLeft))
    {
        if (volume_select_index_ == 0)
        {
            const int new_vol = std::max(kVolumeMin,
            AudioManager::GetInstance()->GetBgmVolume() - kVolumeStep);
            AudioManager::GetInstance()->SetBgmVolume(new_vol);
        }
        else
        {
            const int new_vol = std::max(kVolumeMin,
            AudioManager::GetInstance()->GetSeVolume() - kVolumeStep);
            AudioManager::GetInstance()->SetSeVolume(new_vol);

            AudioManager::GetInstance()->PlaySePreview();
        }
    }

    if (input_->IsPushThisFrame(InputManager::GameKeyKind::kRight))
    {
        if (volume_select_index_ == 0)
        {
            const int new_vol = std::min(kVolumeMax,
            AudioManager::GetInstance()->GetBgmVolume() + kVolumeStep);
            AudioManager::GetInstance()->SetBgmVolume(new_vol);
        }
        else
        {
            const int new_vol = std::min(kVolumeMax,
            AudioManager::GetInstance()->GetSeVolume() + kVolumeStep);
            AudioManager::GetInstance()->SetSeVolume(new_vol);

            AudioManager::GetInstance()->PlaySePreview();
        }
    }
}

/// <summary>
/// 毎フレームの描画処理
/// </summary>
void PauseManager::Render()
{
    //ヒント表示（5秒無操作時）
    if (is_showing_hint_)
    {
        const int hint_text_width = GetDrawStringWidth(current_hint_text_, static_cast<int>(strlen(current_hint_text_)));
        const int hint_x = GameInfo::GetInstance()->GetResolutionX()
            - hint_text_width
            - kHintTextOffsetFromRight;
        const int hint_y = GameInfo::GetInstance()->GetResolutionY()
            - kHintTextOffsetFromBottom;
        DrawString(hint_x, hint_y, current_hint_text_,
            GetColor(kColorBrightWhite, kColorBrightWhite, kColorBrightWhite));
    }

    if (!is_paused_)
    { 
        return;
    }

    //暗転
    const int screen_width = GameInfo::GetInstance()->GetResolutionX();
    const int screen_height = GameInfo::GetInstance()->GetResolutionY();
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, kOverlayAlpha);
    DrawBox(0, 0, screen_width, screen_height, GetColor(0, 0, 0), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    //ポーズUI画像
    DrawGraph(ui_draw_pos_x_, ui_draw_pos_y_, pause_ui_image_handle_, TRUE);

    //「PAUSE」テキスト点滅
    if (is_visible_string_)
    {
        DrawString(string_draw_pos_x_, string_draw_pos_y_,
            current_pause_text_,
            GetColor(kColorBrightWhite, kColorBrightWhite, kColorBrightWhite));
    }

    //BGM音量
    const int center_x = GameInfo::GetInstance()->GetCenterX();
    const int center_y = GameInfo::GetInstance()->GetCenterY();

    const unsigned int bgm_color =
        (volume_select_index_ == 0)
        ? GetColor(kSelectedR, kSelectedG, kSelectedB)
        : GetColor(kColorBrightWhite, kColorBrightWhite, kColorBrightWhite);

    const unsigned int se_color =
        (volume_select_index_ == 1)
        ? GetColor(kSelectedR, kSelectedG, kSelectedB)
        : GetColor(kColorBrightWhite, kColorBrightWhite, kColorBrightWhite);

    DrawFormatStringToHandle(
        center_x - kVolumeUiOffsetX, center_y + kBgmVolumeOffsetY,
        bgm_color, volume_font_handle_,
        kBgmVolumeFormat,
        AudioManager::GetInstance()->GetBgmVolume()
    );

    DrawFormatStringToHandle(
        center_x - kVolumeUiOffsetX, center_y + kSeVolumeOffsetY,
        se_color, volume_font_handle_,
        kSeVolumeFormat,
        AudioManager::GetInstance()->GetSeVolume()
    );

    DrawFormatStringToHandle(
        center_x - kVolumeUiOffsetX, center_y + kSeVolumeOffsetY + kVolumeGuideSpacingY,
        GetColor(kColorBrightWhite, kColorBrightWhite, kColorBrightWhite),
        volume_font_handle_,
        kVolumeSelectGuideText
    );

    DrawFormatStringToHandle(
        center_x - kVolumeUiOffsetX - kReturnToTitleGuideSpacingX,
        center_y + kSeVolumeOffsetY + kVolumeGuideSpacingY + kReturnToTitleGuideSpacingY,
        GetColor(kColorBrightWhite, kColorBrightWhite, kColorBrightWhite),
        volume_font_handle_,
        current_return_to_title_text_
    );
}

/// <summary>
/// 初期化処理
/// </summary>
void PauseManager::Initialize()
{
    pause_ui_image_handle_ = LoadGraph(kPauseUiPath);

    GetGraphSize(pause_ui_image_handle_, &pause_ui_image_width_, &pause_ui_image_height_);

    ui_draw_pos_x_ = static_cast<int>(GameInfo::GetInstance()->GetCenterX() - (pause_ui_image_width_ * kHalf));
    ui_draw_pos_y_ = static_cast<int>(GameInfo::GetInstance()->GetCenterY() - (pause_ui_image_height_));

    string_draw_pos_x_ = static_cast<int>(GameInfo::GetInstance()->GetCenterX() - (pause_ui_image_width_ * kHalf));
    string_draw_pos_y_ = static_cast<int>(GameInfo::GetInstance()->GetCenterY() - (pause_ui_image_height_) - kPauseStringAdjustX);

    volume_font_handle_ = CreateFontToHandle(
        kVolumeFontName,
        kVolumeFontSize,
        kVolumeFontThick,
        DX_FONTTYPE_ANTIALIASING_4X4
    );
}

/// <summary>
/// 後処理
/// </summary>
void PauseManager::Finalize()
{
    if (pause_ui_image_handle_ != -1)
    {
        DeleteGraph(pause_ui_image_handle_);
        pause_ui_image_handle_ = -1;
    }

    if (volume_font_handle_ != -1)
    {
        DeleteFontToHandle(volume_font_handle_);
        volume_font_handle_ = -1;
    }
}