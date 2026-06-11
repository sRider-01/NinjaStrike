#include "TitleUI.h"
#include "Input/InputManager.h"
#include "GameInfo.h"
#include "DxLib.h"

/// <summary>
/// TitleUI.cppで使う定数
/// </summary>
namespace
{
	/// <summary>
	/// パッド用「Aボタンを押してください」UIのパス
	/// </summary>
	const char* kPressAUiPath = "Assets/Image/UI/TitleUI/TitleUiPressAforGamePad.png";

	/// <summary>
	/// 「Zを押してください」UIのパス
	/// </summary>
	const char* kPressZUiPath = "Assets/Image/UI/TitleUI/TitleUIPressZ.png";

	/// <summary>
	/// タイトルUIのパス
	/// </summary>
	const char* kTitleUiPath = "Assets/Image/UI/TitleUI/TitleImage.png";

	/// <summary>
	/// パスの配列
	/// </summary>
	const std::array<const char*, TitleUI::kImageCount> kPaths =
	{
		kTitleUiPath,
		kPressZUiPath,
		kPressAUiPath,
	};

	/// <summary>
	/// 点滅アニメーションの間隔
	/// </summary>
	const float kBlinkInterval = 5.0f;

	/// <summary>
	/// 画像拡大の時間
	/// </summary>
	const float kZoomTime = 1.2f;

	/// <summary>
	/// フェードアウトにかける時間
	/// </summary>
	const float kFadeTime = 1.2f;

	/// <summary>
	/// アルファの最大値
	/// </summary>
	const float kMaxAlpha = 255.0f;

	/// <summary>
	/// 横幅などを半分にするときに使う
	/// </summary>
	const int kHalf = 2;

	/// <summary>
	/// 数値を2倍にするときに使う
	/// </summary>
	const int kDouble = 2;

	/// <summary>
	/// 高さなどを1/4にするときに使う
	/// </summary>
	const int kQuarter= 4;

	/// <summary>
	/// 「画面上部」を指すための微調整用
	/// </summary>
	const float kWindowHigh = 3.0f / 4.0f;

	/// <summary>
	/// 最初の画像倍率
	/// </summary>
	const float kStartScale = 1.0f;

	/// <summary>
	/// 最終的な画像倍率
	/// </summary>
	const float kEndScale = 1.15f;

	/// <summary>
	/// FlashTimerなどのインクリメント用値
	/// </summary>
	const float kTimerIncNum = 0.1f;

	/// <summary>
	/// 「Zを押してください」のY座標オフセット（画面中央からの距離）
	/// </summary>
	const int kPressZOffsetY = 200;
}
/// <summary>
/// コンストラクタ
/// </summary>
TitleUI::TitleUI()
	:interval_(kBlinkInterval)
	, zoom_time_(kZoomTime)
	, fade_time_(kFadeTime)
	, alpha_(kMaxAlpha)
{
}

/// <summary>
/// デストラクタ
/// </summary>
TitleUI::~TitleUI()
{
}

/// <summary>
/// 画像の読み込み処理
/// </summary>
void TitleUI::LoadGraphics()
{
	for (int i = 0; i < kImageCount; ++i)
	{
		handles_[i] = LoadGraph(kPaths[i]);

		GetGraphSize(handles_[i], &widths_[i], &heights_[i]);
	}


	//「Zを押してください」の座標設定
	//表示する位置をx_centerに
	x_press_z_ui_pos_ = GameInfo::GetInstance()->GetCenterX();

	//表示する位置を画面中央からpress_z_height_adjust分だけ低い位置に
	y_press_z_ui_pos_ = GameInfo::GetInstance()->GetCenterY() + kPressZOffsetY;


	//タイトル画像の座標設定
	//画像の横幅半分のサイズ
	title_width_adjust = GameInfo::GetInstance()->GetCenterX() - widths_[kTitle] / kHalf;

	//画像の高さの半分のサイズ
	title_height_adjust = heights_[kTitle] / kHalf;

	//表示する位置をwidth_adjustに
	x_title_ui_pos_ = title_width_adjust;

	//表示する位置を画面中央の3/4からheight_adjust分だけ高い位置に
	y_title_ui_pos_ = static_cast<int>(GameInfo::GetInstance()->GetCenterY() * kWindowHigh) + title_height_adjust;
}

/// <summary>
/// 経過時間の更新処理
/// </summary>
/// <param name="delta_time">前回実行フレームからの経過時間（秒）</param>
void TitleUI::Update(float delta_time)
{
	//blink_timer_をkTimerIncNumづつインクリメント
	blink_timer_ += kTimerIncNum;

	//blink_timer_がinterval_以上なら
	if (blink_timer_ >= interval_)
	{
		//blink_timer_をリセット（超過してたらその分は保持）
		blink_timer_ -= interval_;

		//is_show_のboolを切り替え
		is_show_ = !is_show_;
	}

	//アニメーションモードが「通常」の時に
	if (anim_mode_state_ == AnimMode::kIdle)
	{
		//Zボタンorエンターキーが押されたら
		if (InputManager::GetInstance()->IsPushThisFrame(InputManager::GameKeyKind::kPlayerLongRangeAttack) ||
			InputManager::GetInstance()->IsPushThisFrame(InputManager::GameKeyKind::kDecide))
		{
			//フェードアウト状態を「拡大」にする
			anim_mode_state_ = AnimMode::kZoom;

			//アニメーション用タイマーをリセット
			anim_timer_ = 0.0f;

			//現在の拡大率を初期値にする
			now_scale_ = kStartScale;

			//alpha値を最大にする
			alpha_ = kMaxAlpha;
		}
	}

	// ↓ anim_timer_の加算は1回だけ
	anim_timer_ += delta_time;

	//拡大率の算出
	scale_rate = anim_timer_ / zoom_time_;

	//実際の拡大率の算出
	now_scale_ = kStartScale + ((kEndScale - kStartScale) * scale_rate);

	//透明率の算出
	alpha_rate = anim_timer_ / fade_time_;

	//alpha値の算出
	alpha_ = ((1.0f - alpha_rate) * kMaxAlpha);

	//経過時間が基準を超えたら
	if (anim_timer_ >= fade_time_)
	{
		//alpha値を最低にする 最低：完全に透明
		alpha_ = 0;
	}
}

/// <summary>
/// 毎フレームの描画処理
/// </summary>
void TitleUI::Render()
{

	//タイトル描画
	DrawGraph(0, 0, handles_[kTitle], TRUE);

	//パッド接続状態に応じて使う画像を切り替える
	const int press_handle_index =
		is_gamepad_connected_ ? kPressA : kPressZ;

	//待機中（点滅表示）
	if (is_show_ && anim_mode_state_ == AnimMode::kIdle)
	{
		DrawRotaGraph(
			x_press_z_ui_pos_, y_press_z_ui_pos_,
			kStartScale, 0.0,
			handles_[press_handle_index], TRUE
		);
	}

	//ズームフェードアウト中
	if (anim_mode_state_ != AnimMode::kIdle)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(alpha_));
		DrawRotaGraph(
			x_press_z_ui_pos_, y_press_z_ui_pos_,
			now_scale_, 0.0,
			handles_[press_handle_index], TRUE
		);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

/// <summary>
/// 画像を消す処理
/// </summary>
void TitleUI::DestroyGraphics()
{
	for (int i = 0; i < kImageCount; ++i)
	{
		if (handles_[i] > 0)
		{
			DeleteGraph(handles_[i]);
			handles_[i] = 0;
		}
	}
}

