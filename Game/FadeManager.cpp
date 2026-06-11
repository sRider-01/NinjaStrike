#include "FadeManager.h"
#include "GameInfo.h"
#include "DxLib.h"
#include <algorithm>

/// <summary>
/// インスタンス初期化
/// </summary>
FadeManager* FadeManager::fade_instance_ = nullptr;

/// <summary>
/// FadeManager.cppで使う定数
/// </summary>
namespace
{
	/// <summary>
	/// フェードイン・アウト用黒画のパス
	/// </summary>
	const char* kFadeImagePath = "Assets/Image/Fade/FadeBlack.png";

	/// <summary>
	/// 画像のx座標
	/// </summary>
	const int kUiPosX = 1;

	/// <summary>
	/// 画像のy座標
	/// </summary>
	const int kUiPosY = 1;

	/// <summary>
	/// 画像の拡大率
	/// </summary>
	const int kScale = 1300;

	/// <summary>
	/// 最大のalpha値
	/// </summary>
	const float kMaxAlpha = 255;
}
/// <summary>
/// コンストラクタ
/// </summary>
FadeManager::FadeManager()
{
}

/// <summary>
/// デストラクタ 
/// </summary>
FadeManager::~FadeManager()
{
}

/// <summary>
/// フェードイン開始関数 黒画⇒通常
/// </summary>
/// <param name="time">フェードインの時間</param>
void FadeManager::StartFadeIn(float time)
{
	//フェードフラグをINに
	fade_mode_ = FadeMode::In;

	//引数を要求秒数に入れる
	fade_seconds_ = time;

	//経過時間をリセット
	alpha_elapsed_time_ = 0.0f;

	//割合値をリセット
	alpha_rate = 0.0f;

	//初期alphaを黒から開始
	fade_alpha = kMaxAlpha;

	//フェードがまだ終わってないと示す
	is_fade_finished_ = false;
}

/// <summary>
/// フェードアウト開始関数 通常⇒黒画
/// </summary>
/// <param name="time">フェードアウトの時間</param>
void FadeManager::StartFadeOut(float time)
{
	//フェードフラグをOutに
	fade_mode_ = FadeMode::Out;

	//引数を要求秒数に入れる
	fade_seconds_ = time;

	//経過時間をリセット
	alpha_elapsed_time_ = 0.0f;

	//割合値をリセット
	alpha_rate = 0.0f;

	//初期alphaを透明から開始
	fade_alpha = 0.0f;


	//フェードがまだ終わってないと示す
	is_fade_finished_ = false;
}

/// <summary>
/// 経過時間の更新処理
/// </summary>
/// <param name="delta_time">前回実行フレームからの経過時間（秒）</param>
void FadeManager::Update(float delta_time)
{
	//フェードをしない場合
	if (fade_mode_ == FadeMode::None)
	{
		//何もしない
		return;

	}

	//経過時間を計算
	alpha_elapsed_time_ += delta_time;

	//alpha値に掛ける値の算出（alpha_rate:0->1）
	alpha_rate = alpha_elapsed_time_ / fade_seconds_;
	alpha_rate = std::clamp(alpha_rate, 0.0f, 1.0f);


	if (fade_mode_ == FadeMode::In)
	{
		fade_alpha = ((1.0f - alpha_rate) * kMaxAlpha);
	}
	else if (fade_mode_ == FadeMode::Out)
	{
		fade_alpha = alpha_rate * kMaxAlpha;
	}

	//経過時間が要求を超えたとき
	if (alpha_elapsed_time_ >= fade_seconds_)
	{
		if (fade_mode_ == FadeMode::In)
		{
			fade_alpha = 0.0f;
		}
		else if (fade_mode_ == FadeMode::Out)
		{
			fade_alpha = kMaxAlpha;
		}

		//フェードフラグをOFFに
		fade_mode_ = FadeMode::None;

		//フェードが終わったことを示す
		is_fade_finished_ = true;

		//alpha値に掛ける値のリセット
		alpha_rate = 0.0f;

		//経過時間をリセット
		alpha_elapsed_time_ = 0.0f;
	}

}

/// <summary>
/// 画像のロード
/// </summary>
void FadeManager::LoadGraphics()
{
	//LoadGraphする
	image_handle_ = LoadGraph(kFadeImagePath);
}

/// <summary>
/// 画像の削除
/// </summary>
void FadeManager::DeleteGraphics()
{
	//DeleteGraphする
	DeleteGraph(image_handle_);
}

/// <summary>
/// 黒画の描画
/// </summary>
void FadeManager::DrawGraphics()
{
	//透明描画モード
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::clamp(static_cast<int>(fade_alpha), 0, 255));

	//画像中心基準で拡大描画
	DrawExtendGraph(
		0,
		0,
		GameInfo::GetInstance()->GetResolutionX(),
		GameInfo::GetInstance()->GetResolutionY(),
		image_handle_,
		TRUE
	);

	//透明化の後処理
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
