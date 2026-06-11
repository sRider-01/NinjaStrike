#pragma once
#include "Task/Task.h"

/// <summary>
/// 前方宣言
/// </summary>
class InputManager;

/// <summary>
/// ポーズ機能についてのクラス
/// </summary>
class PauseManager :public Task
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PauseManager() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~PauseManager() = default;

	/// <summary>
	/// 毎フレームの更新処理
	/// </summary>
	/// <param name="delta_time">前回実行フレームからの経過時間（秒）</param>
	void Update(float delta_time) override;

	/// <summary>
	/// 毎フレームの描画処理
	/// </summary>
	void Render() override;

	/// <summary>
	/// ポーズ中も更新させるタスクか？
	/// </summary>
	bool CanUpdateWhilePaused() override
	{
		return true;
	}

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 後処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// ポーズ中か？を返す
	/// </summary>
	/// <returns>ポーズ中か？</returns>
	bool IsPaused() const
	{
		return is_paused_;
	}

	/// <summary>
	/// インプットの情報をセット
	/// </summary>
	/// <param name="input">インプットの情報</param>
	void SetInput(InputManager* input)
	{
		input_ = input;
	}

	/// <summary>
	/// ヒント表示を許可するかをセットする
	/// ゲームクリア中やタイトル画面ではfalseにする
	/// </summary>
	/// <param name="enabled">許可するか</param>
	void SetHintEnabled(bool enabled)
	{
		is_hint_enabled_ = enabled;
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
	/// 今がポーズ中か？
	/// </summary>
	bool is_paused_ = false;

	/// <summary>
	/// インプットの情報
	/// </summary>
	InputManager* input_ = nullptr;

	/// <summary>
	/// ポーズUIの画像ハンドル
	/// </summary>
	int pause_ui_image_handle_ = -1;

	/// <summary>
	/// ポーズUIの幅
	/// </summary>
	int pause_ui_image_width_ = 0;

	/// <summary>
	/// ポーズUIの高さ
	/// </summary>
	int pause_ui_image_height_ = 0;

	/// <summary>
	/// ポーズUIを描画するX座標
	/// </summary>
	int ui_draw_pos_x_ = 0;

	/// <summary>
	/// ポーズUIを描画するY座標
	/// </summary>
	int ui_draw_pos_y_ = 0;

	/// <summary>
	/// 「ポーズ」表記の描画するX座標
	/// </summary>
	int string_draw_pos_x_ = 0;

	/// <summary>
	/// 「ポーズ」表記の描画するY座標
	/// </summary>
	int string_draw_pos_y_ = 0;

	/// <summary>
	/// 点滅状態切り替え用経過時間
	/// </summary>
	float blink_interval_timer_ = 0.0f;

	/// <summary>
	/// 「ポーズ」表記を描画するか？
	/// </summary>
	bool is_visible_string_ = true;

	/// <summary>
	/// ヒント表示を許可するか
	/// </summary>
	bool is_hint_enabled_ = true;

	/// <summary>
	/// 無操作の経過時間（秒）
	/// </summary>
	float idle_timer_ = 0.0f;

	/// <summary>
	/// ヒントを表示するか
	/// </summary>
	bool is_showing_hint_ = false;

	/// <summary>
	/// 音量調整のフォントハンドル
	/// </summary>
	int volume_font_handle_ = -1;

	/// <summary>
	/// 選択中の音量項目（0=BGM 1=SE）
	/// </summary>
	int volume_select_index_ = 0;

	/// <summary>
	/// ゲームパッドが接続されているか
	/// </summary>
	bool is_gamepad_connected_ = false;

	/// <summary>
	/// 現在表示するヒントテキスト
	/// </summary>
	const char* current_hint_text_ = nullptr;

	/// <summary>
	/// 現在表示するポーズテキスト
	/// </summary>
	const char* current_pause_text_ = nullptr;

	/// <summary>
	/// 現在表示するタイトルへ戻るガイドテキスト
	/// </summary>
	const char* current_return_to_title_text_ = nullptr;
};