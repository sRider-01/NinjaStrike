#include "HpBar.h"
#include "GameInfo.h"
#include "DxLib.h"
#include <algorithm>

/// <summary>
/// HpBar.cppで使う定数
/// </summary>
namespace
{
	/// <summary>
	/// HPバーの画像パス
	/// </summary>
	const char* kHpBarImagePath = "Assets/Image/UI/HpBar.png";

	/// <summary>
	/// 画像の分割数
	/// </summary>
	const int kFrameCount = 11;

	/// <summary>
	/// バーの表示位置のマージン
	/// </summary>
	const int kBarPositionMarginY = 50;
}

/// <summary>
/// 初期化処理
/// </summary>
void HpBar::Initialize(int max_hp)
{
	max_hp_ = max_hp;
	current_hp_ = max_hp_;

	hp_bar_image_handle_ = LoadGraph(kHpBarImagePath);

	GetGraphSize(hp_bar_image_handle_, &source_image_width_, &source_image_height_);

	frame_width_ = source_image_width_;
	frame_height_ = (source_image_height_ / kFrameCount);

	draw_pos_y_ = GameInfo::GetInstance()->GetResolutionY() - frame_height_ - kBarPositionMarginY;
}

/// <summary>
/// 後処理
/// </summary>
void HpBar::Finalize()
{
	if (hp_bar_image_handle_ != -1)
	{
		DeleteGraph(hp_bar_image_handle_);
		hp_bar_image_handle_ = -1;
	}
}

/// <summary>
/// 現在のHPをセット
/// </summary>
/// <param name="current_hp">現在のHP</param>
void HpBar::SetHp(int current_hp)
{
	current_hp_ = std::clamp(current_hp, 0, max_hp_);
}

/// <summary>
/// 毎フレームの更新処理
/// </summary>
void HpBar::Update(float delta_time)
{
	//画像のインデックスを現在のHPにする
	frame_index_ = current_hp_;

	//元画像から切り出す画像の座標を計算
	//X：0のまま　Y：インデックス * 切り出す画像一つ分の高さ
	src_x_ = 0;
	src_y_ = frame_index_ * frame_height_;


}

/// <summary>
/// 描画処理
/// </summary>
void HpBar::Render()
{
	if (hp_bar_image_handle_ == -1 || frame_height_ <= 0)
	{
		return;
	}


	DrawRectGraph(
		draw_pos_x_, draw_pos_y_,
		src_x_, src_y_,
		frame_width_, frame_height_,
		hp_bar_image_handle_,
		TRUE
	);
}