#define NOMINMAX
#include "DialogueManager.h"
#include "Input/InputManager.h"
#include "DxLib.h"
#include <algorithm>

/// <summary>
/// DialogueManager.cppで使う定数
/// </summary>
namespace
{
    /// <summary>
    /// テキストのX座標
    /// </summary>
    const int kTextPosX = 60;

    /// <summary>
    /// テキストのY座標
    /// </summary>
    const int kTextPosY = 60;

    /// <summary>
    /// 「次へ」促すテキスト
    /// </summary>
    const char* kPromptText = "▼Enter";

    /// <summary>
    /// パッド用「進む」ヒントテキスト
    /// </summary>
    const char* kPromptTextGamepad = "▼A";

    /// <summary>
    /// 「次へ」促すテキストのX座標
    /// </summary>
    const int kPromptPosX = 670;

    /// <summary>
    /// 「次へ」促すテキストのY座標
    /// </summary>
    const int kPromptPosY = 170;

    /// <summary>
    /// ダイアログボックスの横パディング
    /// </summary>
    const int kPaddingX = 24;

    /// <summary>
    /// ダイアログボックスの縦パディング
    /// </summary>
    const int kPaddingY = 16;

    /// <summary>
    /// ダイアログボックス内のテキスト行数（メッセージ行 + プロンプト行）
    /// </summary>
    const int kDialogueTextLineCount = 2;

    /// <summary>
    /// ダイアログボックスのパディング数（上・中・下）
    /// </summary>
    const int kDialoguePaddingSectionCount = 3;

    /// <summary>
    /// フォント名
    /// </summary>
    const char* kFontName = "Yu Mincho Demibold";

    /// <summary>
    /// フォントサイズ
    /// </summary>
    const int kFontSize = 30;

    /// <summary>
    /// フォントの太さ
    /// </summary>
    const int kFontThick = 6;

    /// <summary>
    /// 背景の透明度
    /// </summary>
    const int kBgAlpha = 200;

    /// <summary>
    /// 色の度合
    /// </summary>
    const int kColorLevel = 255;
}

/// <summary>
/// コンストラクタ
/// </summary>
DialogueManager::DialogueManager()
    : current_index_(0)
    , is_active_(false)
    , is_finished_(false)
    , was_decide_pressed_(false)
    , font_handle_(-1)
{
}

/// <summary>
/// 初期化処理
/// </summary>
void DialogueManager::Initialize()
{
    font_handle_ = CreateFontToHandle(
        kFontName,
        kFontSize,
        kFontThick,
        DX_FONTTYPE_ANTIALIASING_4X4
    );
}

/// <summary>
/// 後処理
/// </summary>
void DialogueManager::Finalize()
{
    if (font_handle_ != -1)
    {
        DeleteFontToHandle(font_handle_);
        font_handle_ = -1;
    }
}

/// <summary>
/// 会話を開始する
/// </summary>
void DialogueManager::StartDialogue(const std::vector<std::string>& messages)
{
    messages_ = messages;
    current_index_ = 0;
    is_active_ = true;
    is_finished_ = false;
    was_decide_pressed_ = true;
}

/// <summary>
/// 毎フレームの更新処理
/// </summary>
void DialogueManager::Update()
{
    if (!is_active_)
    {
        return;
    }

    const bool is_decide_pressed =
        InputManager::GetInstance()->IsPushThisFrame(
            InputManager::GameKeyKind::kDecide);

    // 前フレームで押されていなかった場合のみ受け付ける
    if (is_decide_pressed && !was_decide_pressed_)
    {
        current_index_++;

        // 全メッセージを表示し終えた
        if (current_index_ >= static_cast<int>(messages_.size()))
        {
            is_active_ = false;
            is_finished_ = true;
        }
    }

    was_decide_pressed_ = is_decide_pressed;
}

/// <summary>
/// 毎フレームの描画処理
/// </summary>
void DialogueManager::Render()
{
    if (!is_active_)
    {
        return;
    }

    if (current_index_ >= static_cast<int>(messages_.size()))
    {
        return;
    }

    const std::string& current_message = messages_[current_index_];

    //本文の幅を取得
    const int message_width = GetDrawStringWidthToHandle(
        current_message.c_str(),
        static_cast<int>(current_message.size()),
        font_handle_
    );

    //「次へ」テキストの幅を取得
    const std::string prompt_text =
        is_gamepad_connected_ ? kPromptTextGamepad : kPromptText;

    const int prompt_width = GetDrawStringWidthToHandle(
        prompt_text.c_str(),
        static_cast<int>(prompt_text.size()),
        font_handle_
    );

    //本文とプロンプトのうち長い方に合わせて背景幅を決める
    const int content_width = std::max(message_width, prompt_width);

    //背景ボックスの座標を動的に計算
    const int bg_left = kTextPosX - kPaddingX;
    const int bg_top = kTextPosY - kPaddingY;
    const int bg_right = kTextPosX + content_width + kPaddingX;
    const int bg_bottom = kTextPosY + (kFontSize * kDialogueTextLineCount) + (kPaddingY * kDialoguePaddingSectionCount);

    //プロンプトのY座標も動的に計算
    const int prompt_pos_y = bg_bottom - kPaddingY - kFontSize;
    const int prompt_pos_x = bg_right - prompt_width - kPaddingX;

    DrawBox(bg_left, bg_top, bg_right, bg_bottom, GetColor(0, 0, 0), TRUE);

    // 枠線
    DrawBox(bg_left, bg_top, bg_right, bg_bottom, GetColor(kColorLevel, kColorLevel, kColorLevel), FALSE);

    // メッセージ本文
    DrawStringToHandle(
        kTextPosX,
        kTextPosY,
        current_message.c_str(),
        GetColor(kColorLevel, kColorLevel, kColorLevel),
        font_handle_
    );

    // 「次へ」促すテキスト（右寄せ）
    DrawStringToHandle(
        prompt_pos_x,
        prompt_pos_y,
        prompt_text.c_str(),
        GetColor(kColorLevel, kColorLevel, kColorLevel),
        font_handle_
    );
}