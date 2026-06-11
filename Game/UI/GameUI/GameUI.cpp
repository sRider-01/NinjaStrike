#include "GameUI.h"
#include "Input/InputManager.h"
#include "DxLib.h"

/// <summary>
/// GameUI.cppで使用する定数
/// </summary>
namespace
{
	/// <summary>
	/// 自機やられからUIを表示させるまでの時間
	/// </summary>
	const float kUiRenderTime = 1.0f;

	/// <summary>
	/// 横幅などを半分にするときに使う
	/// </summary>
	const int kHalf = 2;

	/// <summary>
	/// ゲームオーバー後に選択UIを表示するまでの待機時間（秒）
	/// </summary>
	const float kGameOverSelectionWaitTime = 2.0f;

	/// <summary>
	/// 選択中の項目に使う色のアルファ値
	/// </summary>
	const int kSelectedAlpha = 255;

	/// <summary>
	/// 非選択の項目に使う色のアルファ値
	/// </summary>
	const int kUnselectedAlpha = 128;

	/// <summary>
	/// 選択UIのフォントサイズ
	/// </summary>
	const int kSelectionFontSize = 36;

	/// <summary>
	/// 選択UIのフォントの太さ
	/// </summary>
	const int kSelectionFontThick = 6;

	/// <summary>
	/// 「リトライ」テキスト
	/// </summary>
	const char* kRetryText = "リトライ";

	/// <summary>
	/// 「タイトルへ」テキスト
	/// </summary>
	const char* kReturnToTitleText = "タイトルへ";

	/// <summary>
	/// 非選択の項目のRGB値（グレー）
	/// </summary>
	const int kUnselectedColorLevel = 100;

	/// <summary>
	/// 選択肢を表示するY座標のオフセット（画面中央からの距離）
	/// </summary>
	const int kSelectionItemOffsetY = 100;

	/// <summary>
	/// 選択肢1（リトライ）の中心からのXオフセット（左側）
	/// </summary>
	const int kSelectionItem1OffsetX = 200;

	/// <summary>
	/// 選択肢2（タイトルへ）の中心からのXオフセット（右側）
	/// </summary>
	const int kSelectionItem2OffsetX = 200;

	/// <summary>
	/// 色の度合い
	/// </summary>
	const int kColorLevel = 255;

	/// <summary>
	/// ゲームオーバー時の画面暗転アルファ値
	/// </summary>
	const int kDarkenAlpha = 120;

	/// <summary>
	/// ゲームオーバーテキストのフォント名
	/// </summary>
	const char* kGameOverFontName = "Yu Mincho Demibold";

	/// <summary>
	/// ゲームオーバーテキストのフォントサイズ
	/// </summary>
	const int kGameOverFontSize = 60;

	/// <summary>
	/// ゲームオーバーテキストのフォントの太さ
	/// </summary>
	const int kGameOverFontThick = 6;

	/// <summary>
	/// ゲームオーバーテキスト
	/// </summary>
	const char* kGameOverText = "任務失敗...";

	/// <summary>
	/// ゲームオーバー背景ボックスのパディングX
	/// </summary>
	const int kGameOverBgPaddingX = 60;

	/// <summary>
	/// ゲームオーバー背景ボックスのパディングY
	/// </summary>
	const int kGameOverBgPaddingY = 30;

	/// <summary>
	/// ゲームオーバー背景の枠線の色のアルファ値
	/// </summary>
	const int kGameOverBgAlpha = 210;

	/// <summary>
	/// ゲームオーバーテキストのY座標オフセット（画面中央からの距離）
	/// </summary>
	const int kGameOverTextOffsetY = 80;


	/// <summary>
	/// ゲームクリアテキストのフォント名
	/// </summary>
	const char* kGameClearFontName = "Yu Mincho Demibold";

	/// <summary>
	/// ゲームクリアテキスト
	/// </summary>
	const char* kGameClearText = "任務完了";

	/// <summary>
	/// ゲームクリアテキストのフォントサイズ
	/// </summary>
	const int kGameClearFontSize = 60;

	/// <summary>
	/// ゲームクリアテキストのフォントの太さ
	/// </summary>
	const int kGameClearFontThick = 6;

	/// <summary>
	/// ゲームクリア背景ボックスのパディングX
	/// </summary>
	const int kGameClearBgPaddingX = 60;

	/// <summary>
	/// ゲームクリア背景ボックスのパディングY
	/// </summary>
	const int kGameClearBgPaddingY = 30;

	/// <summary>
	/// ゲームクリア背景のアルファ値
	/// </summary>
	const int kGameClearBgAlpha = 210;

	/// <summary>
	/// ゲームクリアテキストのY座標オフセット（画面中央からの距離）
	/// </summary>
	const int kGameClearTextOffsetY = 150;

	/// <summary>
	/// クリア後に選択UIを表示するまでの待機時間（秒）
	/// </summary>
	const float kGameClearSelectionWaitTime = 3.0f;

	/// <summary>
	/// クリア選択肢テキスト
	/// </summary>
	const char* kReturnToTitleFromClearText = "タイトルへ";

	/// <summary>
	/// 通常ボスクリア時のメッセージ
	/// </summary>
	const char* kNormalBossClearMessage =
		"おめでとう！さらに強いボスがいる場所のカギは「フタツノアイダ」だ！";

	/// <summary>
	/// 強化ボスクリア時のメッセージ
	/// </summary>
	const char* kEnhancedBossClearMessage =
		"プレイありがとう！隠し要素まで見つけてくれるなんて作者はとっても嬉しい！";

	/// <summary>
	/// クリアメッセージのフォントサイズ
	/// </summary>
	const int kClearMessageFontSize = 22;

	/// <summary>
	/// クリアメッセージのフォントの太さ
	/// </summary>
	const int kClearMessageFontThick = 4;

	/// <summary>
	/// クリアメッセージのY座標オフセット（任務完了より下）
	/// </summary>
	const int kClearMessageOffsetY = 100;

	/// <summary>
	/// クリアメッセージ背景のパディングX
	/// </summary>
	const int kClearMessagePaddingX = 20;

	/// <summary>
	/// クリアメッセージ背景のパディングY
	/// </summary>
	const int kClearMessagePaddingY = 12;


}
/// <summary>
/// コンストラクタ
/// </summary>
GameUI::GameUI()
{
}

/// <summary>
/// デストラクタ
/// </summary>
GameUI::~GameUI()
{
}

/// <summary>
/// 画像の読み込み処理
/// </summary>
void GameUI::LoadGraphics()
{
	// ゲームオーバーテキスト用フォント生成
	game_over_font_handle_ = CreateFontToHandle(
		kGameOverFontName,
		kGameOverFontSize,
		kGameOverFontThick,
		DX_FONTTYPE_ANTIALIASING_4X4
	);

	//ゲームオーバーフォントを作成
	selection_font_handle_ = CreateFontToHandle(
		kGameOverFontName,
		kSelectionFontSize,
		kSelectionFontThick,
		DX_FONTTYPE_ANTIALIASING_4X4
	);

	//ゲームクリアフォントを作成
	game_clear_font_handle_ = CreateFontToHandle(
		kGameClearFontName,
		kGameClearFontSize,
		kGameClearFontThick,
		DX_FONTTYPE_ANTIALIASING_4X4
	);

	//クリアメッセージフォントを作成
	clear_message_font_handle_ = CreateFontToHandle(
		kGameClearFontName,
		kClearMessageFontSize,
		kClearMessageFontThick,
		DX_FONTTYPE_ANTIALIASING_4X4
	);

	//ゲームクリア画像の座標設定
	//画像の横幅半分のサイズ
	game_clear_width_adjust = GameInfo::GetInstance()->GetCenterX() - widths_[kClear] / kHalf;

	//画像の高さの半分のサイズ
	game_clear_height_adjust = heights_[kClear] / kHalf;

	//表示する位置をwidth_adjustに
	x_game_clear_ui_pos_ = game_clear_width_adjust;

	//表示する位置を画面中央のheight_adjust分だけ高い位置に
	y_game_clear_ui_pos_ = GameInfo::GetInstance()->GetCenterY() - game_clear_height_adjust;
}

/// <summary>
/// 毎フレームの更新処理
/// </summary>
/// <param name="delta_time">前回実行フレームからの経過時間（秒）</param>
void GameUI::Update(float delta_time)
{
	if (game_ui_state_ == GameUiState::kNone)
	{
		return;
	}

	elapsed_time_ += delta_time;

	//ゲームオーバー時の処理
	if (game_ui_state_ == GameUiState::kOver)
	{
		if (elapsed_time_ >= kGameOverSelectionWaitTime)
		{
			is_showing_selection_ = true;
		}

		if (!is_showing_selection_ || is_game_over_selection_confirmed_)
		{
			return;
		}

		if (InputManager::GetInstance()->IsPushThisFrame(InputManager::GameKeyKind::kLeft) ||
			InputManager::GetInstance()->IsPushThisFrame(InputManager::GameKeyKind::kRight))
		{
			game_over_selection_ =
				(game_over_selection_ == GameOverSelection::kRetry)
				? GameOverSelection::kReturnToTitle
				: GameOverSelection::kRetry;
		}

		if (InputManager::GetInstance()->IsPushThisFrame(InputManager::GameKeyKind::kDecide) ||
			InputManager::GetInstance()->IsPushThisFrame(InputManager::GameKeyKind::kPlayerLongRangeAttack))
		{
			is_game_over_selection_confirmed_ = true;
		}
	}

	//クリア時の処理
	if (game_ui_state_ == GameUiState::kClear)
	{
		if (elapsed_time_ >= kGameClearSelectionWaitTime)
		{
			is_showing_clear_selection_ = true;
		}

		if (!is_showing_clear_selection_ || is_game_clear_selection_confirmed_)
		{
			return;
		}

		if (InputManager::GetInstance()->IsPushThisFrame(InputManager::GameKeyKind::kDecide) ||
			InputManager::GetInstance()->IsPushThisFrame(InputManager::GameKeyKind::kPlayerLongRangeAttack))
		{
			is_game_clear_selection_confirmed_ = true;
		}
	}
}

/// <summary>
/// 毎フレームの描画処理
/// </summary>
void GameUI::Render()
{
	//何もしない状態なら何もしない
	if (game_ui_state_ == GameUiState::kNone)
	{
		return;
	}

	//ゲームクリア時
	if (game_ui_state_ == GameUiState::kClear && elapsed_time_ > kUiRenderTime)
	{
		const int center_x = GameInfo::GetInstance()->GetCenterX();
		const int center_y = GameInfo::GetInstance()->GetCenterY();

		const int text_width = GetDrawStringWidthToHandle(
			kGameClearText,
			static_cast<int>(strlen(kGameClearText)),
			game_clear_font_handle_
		);

		const int text_x = center_x - text_width / kHalf;
		const int text_y = center_y - kGameClearTextOffsetY;
		const int bg_left = text_x - kGameClearBgPaddingX;
		const int bg_top = text_y - kGameClearBgPaddingY;
		const int bg_right = text_x + text_width + kGameClearBgPaddingX;
		const int bg_bottom = text_y + kGameClearFontSize + kGameClearBgPaddingY;

		// 半透明の黒背景
		SetDrawBlendMode(static_cast<int>(DX_BLENDMODE_ALPHA), static_cast<int>(kGameClearBgAlpha));
		DrawBox(bg_left, bg_top, bg_right, bg_bottom, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(static_cast<int>(DX_BLENDMODE_NOBLEND), 0);

		// 白枠線
		DrawBox(bg_left, bg_top, bg_right, bg_bottom, GetColor(kColorLevel, kColorLevel, kColorLevel), FALSE);

		// 「任務完了」テキスト
		DrawStringToHandle(
			text_x,
			text_y,
			kGameClearText,
			GetColor(kColorLevel, kColorLevel, kColorLevel),
			game_clear_font_handle_
		);
		
		//選択UIが表示タイミングになったら「タイトルへ」を描画
		if (is_showing_clear_selection_)
		{
			const int center_x = GameInfo::GetInstance()->GetCenterX();
			const int center_y = GameInfo::GetInstance()->GetCenterY();

			const int prompt_text_width = GetDrawStringWidthToHandle(
				kReturnToTitleFromClearText,
				static_cast<int>(strlen(kReturnToTitleFromClearText)),
				selection_font_handle_
			);

			const int prompt_draw_x = center_x - prompt_text_width / kHalf;
			const int prompt_draw_y = center_y + kSelectionItemOffsetY;

			SetDrawBlendMode(static_cast<int>(DX_BLENDMODE_ALPHA), static_cast<int>(kSelectedAlpha));
			DrawStringToHandle(
				prompt_draw_x,
				prompt_draw_y,
				kReturnToTitleFromClearText,
				GetColor(kColorLevel, kColorLevel, kColorLevel),
				selection_font_handle_
			);
			SetDrawBlendMode(static_cast<int>(DX_BLENDMODE_NOBLEND), 0);
		}

		//クリアメッセージを種類に応じて決める
		const char* clear_message = nullptr;
		if (clear_message_kind_ == ClearMessageKind::kNormalBoss)
		{
			clear_message = kNormalBossClearMessage;
		}
		else
		{
			clear_message = kEnhancedBossClearMessage;
		}

		//メッセージの文字幅を取得して中央揃えで配置
		const int msg_width = GetDrawStringWidthToHandle(
			clear_message,
			static_cast<int>(strlen(clear_message)),
			clear_message_font_handle_
		);

		const int msg_x = center_x - msg_width / kHalf;
		const int msg_y = text_y + kGameClearFontSize + kClearMessageOffsetY;
		const int msg_bg_left = msg_x - kClearMessagePaddingX;
		const int msg_bg_top = msg_y - kClearMessagePaddingY;
		const int msg_bg_right = msg_x + msg_width + kClearMessagePaddingX;
		const int msg_bg_bottom = msg_y + kClearMessageFontSize + kClearMessagePaddingY;

		//半透明の黒背景
		SetDrawBlendMode(static_cast<int>(DX_BLENDMODE_ALPHA), static_cast<int>(kGameClearBgAlpha));
		DrawBox(msg_bg_left, msg_bg_top, msg_bg_right, msg_bg_bottom, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(static_cast<int>(DX_BLENDMODE_NOBLEND), 0);

		//枠線
		DrawBox(msg_bg_left, msg_bg_top, msg_bg_right, msg_bg_bottom,
			GetColor(kColorLevel, kColorLevel, kColorLevel), FALSE);

		//メッセージ本文
		DrawStringToHandle(
			msg_x,
			msg_y,
			clear_message,
			GetColor(kColorLevel, kColorLevel, kColorLevel),
			clear_message_font_handle_
		);

		return;
	}

	//ゲームオーバー時
	if (game_ui_state_ == GameUiState::kOver && elapsed_time_ > kUiRenderTime)
	{
		const int center_x = GameInfo::GetInstance()->GetCenterX();
		const int center_y = GameInfo::GetInstance()->GetCenterY();

		//テキストの幅を取得
		const int text_width = GetDrawStringWidthToHandle(
			kGameOverText,
			static_cast<int>(strlen(kGameOverText)),
			game_over_font_handle_
		);

		//背景ボックスの座標を計算
		const int text_x = center_x - text_width / kHalf;
		const int text_y = center_y - kGameOverTextOffsetY;
		const int bg_left = text_x - kGameOverBgPaddingX;
		const int bg_top = text_y - kGameOverBgPaddingY;
		const int bg_right = text_x + text_width + kGameOverBgPaddingX;
		const int bg_bottom = text_y + kGameOverFontSize + kGameOverBgPaddingY;

		//半透明の黒背景
		SetDrawBlendMode(static_cast<int>(DX_BLENDMODE_ALPHA), static_cast<int>(kGameOverBgAlpha));
		DrawBox(bg_left, bg_top, bg_right, bg_bottom, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(static_cast<int>(DX_BLENDMODE_NOBLEND), 0);

		//白枠線
		DrawBox(bg_left, bg_top, bg_right, bg_bottom, GetColor(kColorLevel, kColorLevel, kColorLevel), FALSE);

		//テキスト
		DrawStringToHandle(
			text_x,
			text_y,
			kGameOverText,
			GetColor(kColorLevel, kColorLevel, kColorLevel),
			game_over_font_handle_
		);
	}

	if (!is_showing_selection_)
	{
		return;
	}

	if (game_ui_state_ == GameUiState::kOver && elapsed_time_ > kUiRenderTime)
	{
		DrawGraph(x_game_over_ui_pos_, y_game_over_ui_pos_, handles_[kOver], TRUE);

		//画面全体を暗くする
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, kDarkenAlpha);
		DrawBox(
			0, 0,
			GameInfo::GetInstance()->GetResolutionX(),
			GameInfo::GetInstance()->GetResolutionY(),
			GetColor(0, 0, 0),
			TRUE
		);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	const int center_x = GameInfo::GetInstance()->GetCenterX();
	const int center_y = GameInfo::GetInstance()->GetCenterY();

	//リトライの文字幅を取得して左側に中央揃えで配置
	const int retry_text_width = GetDrawStringWidthToHandle(
		kRetryText,
		static_cast<int>(strlen(kRetryText)),
		selection_font_handle_
	);
	const int retry_draw_x = (center_x - kSelectionItem1OffsetX) - retry_text_width / kHalf;
	const int retry_draw_y = center_y + kSelectionItemOffsetY;

	//タイトルへの文字幅を取得して右側に中央揃えで配置
	const int title_text_width = GetDrawStringWidthToHandle(
		kReturnToTitleText,
		static_cast<int>(strlen(kReturnToTitleText)),
		selection_font_handle_
	);
	const int title_draw_x = (center_x + kSelectionItem2OffsetX) - title_text_width / kHalf;
	const int title_draw_y = center_y + kSelectionItemOffsetY;

	//リトライの色を選択状態に応じて決める
	unsigned int retry_color = 0;
	if (game_over_selection_ == GameOverSelection::kRetry)
	{
		retry_color = GetColor(kColorLevel, 0, 0);
	}
	else
	{
		retry_color = GetColor(kUnselectedColorLevel, kUnselectedColorLevel, kUnselectedColorLevel);
	}

	//タイトルへの色を選択状態に応じて決める
	unsigned int title_color = 0;
	if (game_over_selection_ == GameOverSelection::kReturnToTitle)
	{
		title_color = GetColor(kColorLevel, 0, 0);
	}
	else
	{
		title_color = GetColor(kUnselectedColorLevel, kUnselectedColorLevel, kUnselectedColorLevel);
	}

	// 非選択は少し暗くする
	const int retry_alpha = (game_over_selection_ == GameOverSelection::kRetry)
		? static_cast<int>(kSelectedAlpha)
		: static_cast<int>(kUnselectedAlpha);

	SetDrawBlendMode(static_cast<int>(DX_BLENDMODE_ALPHA), retry_alpha);
	DrawStringToHandle(retry_draw_x, retry_draw_y, kRetryText, retry_color, selection_font_handle_);

	const int title_alpha = (game_over_selection_ == GameOverSelection::kReturnToTitle)
		? static_cast<int>(kSelectedAlpha)
		: static_cast<int>(kUnselectedAlpha);

	SetDrawBlendMode(static_cast<int>(DX_BLENDMODE_ALPHA), title_alpha);
	DrawStringToHandle(title_draw_x, title_draw_y, kReturnToTitleText, title_color, selection_font_handle_);

	SetDrawBlendMode(static_cast<int>(DX_BLENDMODE_NOBLEND), 0);
}

/// <summary>
/// 画像を消す処理
/// </summary>
void GameUI::DestroyGraphics()
{
	for (int i = 0; i < kImageCount; ++i)
	{
		if (handles_[i] > 0)
		{
			DeleteGraph(handles_[i]);
			handles_[i] = 0;
		}
	}

	if (game_over_font_handle_ != -1)
	{
		DeleteFontToHandle(game_over_font_handle_);
		game_over_font_handle_ = -1;
	}

	if (selection_font_handle_ != -1)
	{
		DeleteFontToHandle(selection_font_handle_);
		selection_font_handle_ = -1;
	}


	if (game_clear_font_handle_ != -1)
	{
		DeleteFontToHandle(game_clear_font_handle_);
		game_clear_font_handle_ = -1;
	}

	if (clear_message_font_handle_ != -1)
	{
		DeleteFontToHandle(clear_message_font_handle_);
		clear_message_font_handle_ = -1;
	}
}