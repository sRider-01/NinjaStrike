#pragma once
#include <vector>
#include <string>

/// <summary>
/// 会話パートの表示と進行を管理するクラス
/// </summary>
class DialogueManager
{
public:

    /// <summary>
    /// コンストラクタ
    /// </summary>
    DialogueManager();

    /// <summary>
    /// 初期化処理
    /// </summary>
    void Initialize();

    /// <summary>
    /// 後処理
    /// </summary>
    void Finalize();

    /// <summary>
    /// 毎フレームの更新処理
    /// </summary>
    void Update();

    /// <summary>
    /// 毎フレームの描画処理
    /// </summary>
    void Render();

    /// <summary>
    /// 会話を開始する
    /// </summary>
    /// <param name="messages">表示するメッセージのリスト</param>
    void StartDialogue(const std::vector<std::string>& messages);

    /// <summary>
    /// 会話が終了しているか
    /// </summary>
    bool IsFinished() const
    {
        return is_finished_;
    }

    /// <summary>
    /// 会話が進行中か
    /// </summary>
    bool IsActive() const
    {
        return is_active_;
    }
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
    /// 表示するメッセージのリスト
    /// </summary>
    std::vector<std::string> messages_;

    /// <summary>
    /// 現在表示中のメッセージのインデックス
    /// </summary>
    int current_index_ = 0;

    /// <summary>
    /// 会話が進行中か
    /// </summary>
    bool is_active_ = false;

    /// <summary>
    /// 会話が終了したか
    /// </summary>
    bool is_finished_ = false;

    /// <summary>
    /// 前フレームで決定キーが押されていたか（連続送り防止）
    /// </summary>
    bool was_decide_pressed_ = false;

    /// <summary>
    /// 会話表示用フォントハンドル
    /// </summary>
    int font_handle_ = -1;

    /// <summary>
    /// ゲームパッドが接続されているか
    /// </summary>
    bool is_gamepad_connected_ = false;
};