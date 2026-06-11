#pragma once
#include <string>

/// <summary>
/// チュートリアルの表示に関するクラス
/// </summary>
class TutorialManager
{
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    TutorialManager();

    /// <summary>
    /// 初期化処理
    /// </summary>
    void Initialize();

    /// <summary>
    /// 後処理
    /// </summary>
    void Finalize();

    /// <summary>
    /// 毎フレームの描画処理
    /// </summary>
    void Render();

    /// <summary>
    /// チュートリアルの種類
    /// </summary>
    enum class TutorialType
    {
        /// <summary>
        /// なし
        /// </summary>
        kNone,

        /// <summary>
        /// 移動
        /// </summary>
        kMove,

        /// <summary>
        /// ジャンプ
        /// </summary>
        kJump,

        /// <summary>
        /// ダッシュ
        /// </summary>
        kDash,

        /// <summary>
        /// 近距離攻撃
        /// </summary>
        kCloseAttack,

        /// <summary>
        /// 遠距離攻撃
        /// </summary>
        kLongRangeAttack,

        /// <summary>
        /// 隠し場所ヒント
        /// </summary>
        kHiddenHint,

        /// <summary>
        /// ボス強化ヒント
        /// </summary>
        kBossHint,
    };

    /// <summary>
    /// チュートリアル発生地点
    /// </summary>
    struct TutorialTrigger
    {
        /// <summary>
        /// X座標
        /// </summary>
        float x;

        /// <summary>
        /// Y座標
        /// </summary>
        float y;

        /// <summary>
        /// 幅
        /// </summary>
        float width;

        /// <summary>
        /// 高さ
        /// </summary>
        float height;

        /// <summary>
        /// チュートリアルタイプ
        /// </summary>
        TutorialType type;

        /// <summary>
        /// 一度しか表示しないか？
        /// </summary>
        bool is_triggered_once;
    };

    /// <summary>
    /// チュートリアルの表示
    /// </summary>
    /// <param name="type">チュートリアルタイプ</param>
    void ShowTutorial(TutorialType type);

    /// <summary>
    /// チュートリアルの非表示
    /// </summary>
    void HideTutorial();

    /// <summary>
    /// チュートリアルの文言取得
    /// </summary>
    /// <returns>チュートリアルの文言</returns>
    const char* GetTutorialText();


    /// <summary>
    /// ゲームパッド接続状態をセットする
    /// </summary>
    /// <param name="is_connected">接続されているか</param>
    void SetGamepadConnected(bool is_connected)
    {
        is_gamepad_connected_ = is_connected;
    }

private:
    /// <summary>
    /// 今のチュートリアル表示タイプ
    /// </summary>
    TutorialType current_type_;

    /// <summary>
    /// 表示しているか？
    /// </summary>
    bool is_visible_ = false;

    /// <summary>
    /// チュートリアル文字表示用フォントハンドル
    /// </summary>
    int tutorial_font_handle_ = -1;

    /// <summary>
    /// ゲームパッドが接続されているか
    /// </summary>
    bool is_gamepad_connected_ = false;
};