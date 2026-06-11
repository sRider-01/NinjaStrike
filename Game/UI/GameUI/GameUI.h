#pragma once
#include "GameInfo.h"
#include <iostream>
#include <array>

/// <summary>
/// ゲームクリア/ゲームオーバーUIのクラス
/// </summary>
class GameUI
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameUI();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameUI();

	/// <summary>
	/// 画像の読み込み処理
	/// </summary>
	void LoadGraphics();

	/// <summary>
	/// 毎フレームの更新処理
	/// </summary>
	/// <param name="delta_time">前回実行フレームからの経過時間（秒）</param>
	void Update(float delta_time);

	/// <summary>
	/// 毎フレームの描画処理
	/// </summary>
	void Render();

	/// <summary>
	/// 画像を消す処理
	/// </summary>
	void DestroyGraphics();

	/// <summary>
	/// 状態を「通常」にする
	/// </summary>
	void SetStateInit()
	{
		game_ui_state_ = GameUiState::kNone;
	}

	/// <summary>
	/// 状態を「クリア」にする
	/// </summary>
	void SetStateGameClear()
	{
		game_ui_state_ = GameUiState::kClear;
	}

	/// <summary>
	/// 状態を「ゲームオーバー」にする
	/// </summary>
	void SetStateGameOver()
	{
		game_ui_state_ = GameUiState::kOver;
	}

	/// <summary>
	/// ゲームオーバー後の選択肢
	/// </summary>
	enum class GameOverSelection
	{
		/// <summary>
		/// リトライ
		/// </summary>
		kRetry,

		/// <summary>
		/// タイトルへ戻る
		/// </summary>
		kReturnToTitle,
	};

	/// <summary>
	/// ゲームオーバー選択が確定したか
	/// </summary>
	bool IsGameOverSelectionConfirmed() const
	{
		return is_game_over_selection_confirmed_;
	}

	/// <summary>
	/// 選択された内容を返す
	/// </summary>
	GameOverSelection GetGameOverSelection() const
	{
		return game_over_selection_;
	}

	/// <summary>
	/// クリア後の選択が確定したか
	/// </summary>
	bool IsGameClearSelectionConfirmed() const
	{
		return is_game_clear_selection_confirmed_;
	}

	/// <summary>
	/// クリア後の選択内容を返す（タイトルへのみ）
	/// </summary>
	bool IsReturnToTitleFromClear() const
	{
		return is_game_clear_selection_confirmed_;
	}

	/// <summary>
	/// クリアメッセージの種類
	/// </summary>
	enum class ClearMessageKind
	{
		/// <summary>
		/// 通常ボス撃破
		/// </summary>
		kNormalBoss,

		/// <summary>
		/// 強化ボス撃破
		/// </summary>
		kEnhancedBoss,
	};

	/// <summary>
	/// クリアメッセージの種類をセットする
	/// </summary>
	/// <param name="kind">クリアメッセージの種類</param>
	void SetClearMessageKind(ClearMessageKind kind)
	{
		clear_message_kind_ = kind;
	}

	/// <summary>
	/// 画像の種類（ゲームクリア／オーバーの2つ）
	/// </summary>
	static const int kImageCount = 2;
private:
	/// <summary>
	/// 画像の種類
	/// </summary>
	enum ImageKind
	{
		kClear, //ゲームクリア
		kOver,  //ゲームオーバー
	};

	/// <summary>
	/// UI表示の状態
	/// </summary>
	enum class GameUiState
	{
		kNone,	//通常
		kClear,	//クリア
		kOver,	//ゲームオーバー
	};


	/// <summary>
	/// ハンドルの配列
	/// </summary>
	std::array<int, kImageCount> handles_{};

	/// <summary>
	/// 画像の横幅の配列
	/// </summary>
	std::array<int, kImageCount> widths_{};

	/// <summary>
	/// 画像の高さの配列
	/// </summary>
	std::array<int, kImageCount> heights_{};

	/// <summary>
	/// 初期状態は「通常」
	/// </summary>
	GameUiState game_ui_state_ = GameUiState::kNone;

	/// <summary>
	/// ゲームオーバー画像のx座標
	/// </summary>
	int x_game_over_ui_pos_ = 0;

	/// <summary>
	/// ゲームオーバー画像のy座標
	/// </summary>
	int y_game_over_ui_pos_ = 0;

	/// <summary>
	/// ゲームクリア画像のx座標
	/// </summary>
	int x_game_clear_ui_pos_ = 0;

	/// <summary>
	/// ゲームクリア画像のy座標
	/// </summary>
	int y_game_clear_ui_pos_ = 0;

	/// <summary>
	/// ゲームオーバー画像の横幅微調整用
	/// </summary>
	int game_over_width_adjust = 0;

	/// <summary>
	/// ゲームオーバー画像の高さ微調整用
	/// </summary>
	int game_over_height_adjust = 0;

	/// <summary>
	/// ゲームクリア画像の横幅微調整用
	/// </summary>
	int game_clear_width_adjust = 0;

	/// <summary>
	/// ゲームクリア画像の高さ微調整用
	/// </summary>
	int game_clear_height_adjust = 0;

	/// <summary>
	/// 経過時間
	/// </summary>
	float elapsed_time_ = 0.0f;

	/// <summary>
	/// ゲームオーバー選択表示に移行するまでの待機時間（秒）
	/// </summary>
	static constexpr float kGameOverSelectionWaitTime = 2.0f;

	/// <summary>
	/// 選択UIが表示されているか
	/// </summary>
	bool is_showing_selection_ = false;

	/// <summary>
	/// 選択が確定したか
	/// </summary>
	bool is_game_over_selection_confirmed_ = false;

	/// <summary>
	/// 現在の選択
	/// </summary>
	GameOverSelection game_over_selection_ = GameOverSelection::kRetry;

	/// <summary>
	/// リトライ／タイトルへ フォントハンドル
	/// </summary>
	int selection_font_handle_ = -1;

	/// <summary>
	/// ゲームオーバーテキスト表示用フォントハンドル
	/// </summary>
	int game_over_font_handle_ = -1;

	/// <summary>
	/// ゲームクリアテキスト表示用フォントハンドル
	/// </summary>
	int game_clear_font_handle_ = -1;

	/// <summary>
	/// クリアメッセージ表示用フォントハンドル
	/// </summary>
	int clear_message_font_handle_ = -1;

	/// <summary>
	/// クリア後に選択UIを表示するまでの経過時間
	/// </summary>
	bool is_showing_clear_selection_ = false;

	/// <summary>
	/// クリア後の選択が確定したか
	/// </summary>
	bool is_game_clear_selection_confirmed_ = false;

	/// <summary>
	/// クリアメッセージの種類
	/// </summary>
	ClearMessageKind clear_message_kind_ = ClearMessageKind::kNormalBoss;
};