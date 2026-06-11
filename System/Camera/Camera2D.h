#pragma once

/// <summary>
/// 2D横スクロールカメラに関するクラス
/// </summary>
class Camera2D
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Camera2D() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Camera2D() = default;

public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize(int screen_width, int screen_height);

	/// <summary>
	/// ワールドマップの外を移さないための処理
	/// </summary>
	void ClampToWorld();

	/// <summary>
	/// ワールドマップの範囲をセット
	/// </summary>
	/// <param name="world_width">ワールドマップの幅</param>
	/// <param name="world_height">ワールドマップの高さ</param>
	void SetWorldSize(float world_width, float world_height);

	/// <summary>
	/// ターゲット(プレイヤー)を中心にカメラ座標を更新する
	/// </summary>
	/// <param name="target_center_x">ターゲットのX座標</param>
	/// <param name="target_center_y">ターゲットのY座標</param>
	/// <param name="delta_time">前回実行フレームからの経過時間（秒）</param>
	void Update(float target_center_x, float target_center_y, float delata_time);

	/// <summary>
	/// カメラをターゲット位置に瞬時に移動させる
	/// ステージ切り替え時など補間なしで即座に合わせたいときに使う
	/// </summary>
	/// <param name="target_center_x">ターゲットの中心X座標</param>
	/// <param name="target_center_y">ターゲットの中心Y座標</param>
	void SnapToTarget(float target_center_x, float target_center_y);

	/// <summary>
	/// ワールドマップの座標をカメラ座標に変換(X座標)
	/// </summary>
	/// <param name="world_x">ワールドマップX座標</param>
	/// <returns>カメラ座標のX座標</returns>
	float ToScreenX(float world_x) const
	{
		return world_x - camera_left_x_;
	}

	/// <summary>
	/// ワールドマップの座標をカメラ座標に変換(Y座標)
	/// </summary>
	/// <param name="world_y">ワールドマップY座標</param>
	/// <returns>カメラ座標のY座標</returns>
	float ToScreenY(float world_y) const
	{
		return world_y - camera_top_y_;
	}

	/// <summary>
	/// カメラ座標の左端X座標を取得
	/// </summary>
	/// <returns>カメラ座標の左端X座標</returns>
	float GetCameraLeftX() const
	{
		return camera_left_x_;
	}

	/// <summary>
	/// カメラ座標の上端Y座標を取得
	/// </summary>
	/// <returns>カメラ座標の上端Y座標</returns>
	float GetCameraTopY() const
	{
		return camera_top_y_;
	}

private:

	/// <summary>
	/// カメラ座標X
	/// </summary>
	float camera_left_x_ = 0.0f;

	/// <summary>
	/// カメラ座標Y
	/// </summary>
	float camera_top_y_ = 0.0f;

	/// <summary>
	/// 描画範囲幅
	/// </summary>
	int screen_width_ = 0;

	/// <summary>
	/// 描画範囲高さ
	/// </summary>
	int screen_height_ = 0;

	/// <summary>
	/// ワールドマップの幅
	/// </summary>
	float world_width_ = 0.0f;

	/// <summary>
	/// ワールドマップの高さ
	/// </summary>
	float world_height_ = 0.0f;
};