#include "TutorialManager.h"
#include "DxLib.h"

/// <summary>
/// TyutorialManager.cppで使う定数
/// </summary>
namespace
{
    /// <summary>
    /// テキストのX座標
    /// </summary>
    const int kTextPosX = 220;

    /// <summary>
    /// テキストのY座標
    /// </summary>
    const int kTextPosY = 80;

    /// <summary>
    /// 背景色の左座標
    /// </summary>
    const int kBgLeft = 180;

    /// <summary>
    /// 背景色の上座標
    /// </summary>
    const int kBgTop = 50;

    /// <summary>
    /// 背景色の右座標
    /// </summary>
    const int kBgRight = 700;
    
    /// <summary>
    /// 背景色の下座標
    /// </summary>
    const int kBgBottom = 130;

    /// <summary>
    /// フォントの名前
    /// </summary>
    const char* kFontName = "Yu Mincho Demibold";

    /// <summary>
    /// フォントサイズ
    /// </summary>
    const int kFontSize = 26;

    /// <summary>
    /// フォント太さ
    /// </summary>
    const int kFontThick = 6;

    /// <summary>
    /// 文字列表示のパディングX
    /// </summary>
    const int kPaddingX = 24;

    /// <summary>
    /// 文字列表示のパディングY
    /// </summary>
    const int kPaddingY = 16;
}

/// <summary>
/// コンストラクタ
/// </summary>
TutorialManager::TutorialManager()
    : current_type_(TutorialType::kNone)
    , is_visible_(false)
{
}

/// <summary>
/// 初期化処理
/// </summary>
void TutorialManager::Initialize()
{
    //フォントハンドルを作成
    tutorial_font_handle_ = CreateFontToHandle(
        kFontName,
        kFontSize,
        kFontThick,
        DX_FONTTYPE_ANTIALIASING_4X4
    );
}

/// <summary>
/// 後処理
/// </summary>
void TutorialManager::Finalize()
{
    if (tutorial_font_handle_ != -1)
    {
        DeleteFontToHandle(tutorial_font_handle_);
        tutorial_font_handle_ = -1;
    }
}


/// <summary>
/// 毎フレームの描画処理
/// </summary>
void TutorialManager::Render()
{
    if (!is_visible_)
    {
        return;
    }


    //文字まわりの余白
    const int padding_x = kPaddingX;
    const int padding_y = kPaddingY;

    //フォントハンドルを使って実際の文字幅を取得
    const char* tutorial_text = GetTutorialText();
    const int text_width = GetDrawStringWidthToHandle(
        tutorial_text,
        static_cast<int>(strlen(tutorial_text)),
        tutorial_font_handle_
    );

    //1行表示なので高さはフォントサイズベース
    const int text_height = kFontSize;

    //テキスト位置から背景矩形を動的計算
    const int bg_left = kTextPosX - padding_x;
    const int bg_top = kTextPosY - padding_y;
    const int bg_right = kTextPosX + text_width + padding_x;
    const int bg_bottom = kTextPosY + text_height + padding_y;

    DrawBox(bg_left, bg_top, bg_right, bg_bottom, GetColor(0, 0, 0), TRUE);
    DrawBox(bg_left, bg_top, bg_right, bg_bottom, GetColor(255, 255, 255), FALSE);

    DrawStringToHandle(
        kTextPosX,
        kTextPosY,
        tutorial_text,
        GetColor(255, 255, 255),
        tutorial_font_handle_
    );
}
/// <summary>
/// チュートリアルの表示
/// </summary>
/// <param name="type">チュートリアルタイプ</param>
void TutorialManager::ShowTutorial(TutorialType type)
{
    current_type_ = type;
    is_visible_ = true;
}

/// <summary>
/// チュートリアルの非表示
/// </summary>
void TutorialManager::HideTutorial()
{
    current_type_ = TutorialType::kNone;
    is_visible_ = false;
}

/// <summary>
/// チュートリアルの文言取得
/// </summary>
/// <returns>チュートリアルの文言</returns>
const char* TutorialManager::GetTutorialText()
{
    if (is_gamepad_connected_)
    {
        // パッド用テキスト
        switch (current_type_)
        {
        case TutorialType::kMove:
            return "左スティック / 十字キー : 移動";
        case TutorialType::kJump:
            return "Aボタン : ジャンプ";
        case TutorialType::kDash:
            return "RBボタン + 移動 : ダッシュ";
        case TutorialType::kCloseAttack:
            return "Yボタン : 近距離攻撃";
        case TutorialType::kLongRangeAttack:
            return "Xボタン : 遠距離攻撃";
        case TutorialType::kHiddenHint:
            return "強敵の痕跡：上下二択で赤を取れ";
        case TutorialType::kBossHint:
            return "強敵の痕跡：端の欠落には意味がある";
        default:
            return "表示するものはないはず…";
        }
    }
    else
    {
        // キーボード用テキスト（既存）
        switch (current_type_)
        {
        case TutorialType::kMove:
            return "矢印キー : 移動";
        case TutorialType::kJump:
            return "Space キー : ジャンプ";
        case TutorialType::kDash:
            return "Shift キー ＋ 矢印キー : ダッシュ";
        case TutorialType::kCloseAttack:
            return "X キー : 近距離攻撃";
        case TutorialType::kLongRangeAttack:
            return "Z キー : 遠距離攻撃";
        case TutorialType::kHiddenHint:
            return "強敵の痕跡：上下二択で赤を取れ";
        case TutorialType::kBossHint:
            return "強敵の痕跡：端の欠落には意味がある";
        default:
            return "表示するものはないはず…";
        }
    }
}