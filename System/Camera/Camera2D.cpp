#include "Camera2D.h"
#include <algorithm>

/// <summary>
/// Camera2Dで使う定数
/// </summary>
namespace
{
	/// <summary>
	/// 値を半分にする
	/// </summary>
	const float kHalf = 0.5f;

	/// <summary>
	/// カメラの追従強度
	/// </summary>
	const float kFollowStrength = 25.0f;
}

/// <summary>
/// 初期化処理
/// </summary>
void Camera2D::Initialize(int screen_width, int screen_height)
{
	screen_width_ = screen_width;
	screen_height_ = screen_height;

	camera_left_x_ = 0.0f;
	camera_top_y_ = 0.0f;
}

/// <summary>
/// ワールドマップの外を移さないための処理
/// </summary>
void Camera2D::ClampToWorld()
{
	//ワールドマップ未設定なら何もしない
	if (world_width_ <= 0.0f || world_height_ <= 0.0f)
	{
		return;
	}

	//カメラ座標の最大値(ワールドマップの端座標)をとる
	const float max_camera_left_x = std::max(0.0f, world_width_ - static_cast<float>(screen_width_));
	const float max_camera_top_y = std::max(0.0f, world_height_ - static_cast<float>(screen_height_));

	//カメラ座標を0～ワールドマップの端座標の値の中でクランプ
	camera_left_x_ = std::clamp(camera_left_x_, 0.0f, max_camera_left_x);
	camera_top_y_ = std::clamp(camera_top_y_, 0.0f, max_camera_top_y);

}

/// <summary>
/// ワールドマップの範囲をセット
/// </summary>
void Camera2D::SetWorldSize(float world_width, float world_height)
{
	world_width_ = world_width;
	world_height_ = world_height;

	ClampToWorld();
}

/// <summary>
/// ターゲット(プレイヤー)を中心にカメラ座標を更新する
/// </summary>
void Camera2D::Update(float target_center_x, float target_center_y, float delta_time)
{
	//描画範囲未設定なら何もしない
	if (screen_width_ <= 0.0f || screen_height_ <= 0.0f)
	{
		return;
	}

	//カメラの描画目標地点を画面中央に置く
	const float desired_camera_left_x = target_center_x - (static_cast<float>(screen_width_) * kHalf);
	const float desired_camera_top_y = target_center_y - (static_cast<float>(screen_height_) * kHalf);

	//追従を滑らかに
	float tmp = kFollowStrength * delta_time;
	tmp = std::clamp(tmp, 0.0f, 1.0f);

	camera_left_x_ += (desired_camera_left_x - camera_left_x_) * tmp;
	camera_top_y_ += (desired_camera_top_y - camera_top_y_) * tmp;

	ClampToWorld();

}

/// <summary>
/// カメラをターゲット位置に瞬時に移動させる
/// </summary>
void Camera2D::SnapToTarget(float target_center_x, float target_center_y)
{
	if (screen_width_ <= 0.0f || screen_height_ <= 0.0f)
	{
		return;
	}

	//補間なしで目標座標に直接セット
	camera_left_x_ = target_center_x - (static_cast<float>(screen_width_) * kHalf);
	camera_top_y_ = target_center_y - (static_cast<float>(screen_height_) * kHalf);

	//ワールド外にはみ出さないようにクランプ
	ClampToWorld();
}