#pragma once
#include "KunaiBase.h"
#include "KunaiManager.h"
#include "Collision/CollisionBase.h"
#include "Audio/AudioManager.h"

/// <summary>
/// 敵のクナイに関するクラス
/// </summary>
class EnemyKunai : public KunaiBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	EnemyKunai() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyKunai() = default;

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
	/// クナイ発射処理
	/// </summary>
	/// <param name="pos_x">クナイのX座標</param>
	/// <param name="pos_y">クナイのY座標</param>
	void Fire(float pos_x, float pos_y, bool is_facing_left) override;

	/// <summary>
	/// 当たり判定円を作る
	/// </summary>
	/// <returns>当たり判定円</returns>
	CollisionBase::CircleF MakeHitCircle() const;

	/// <summary>
	/// 攻撃の当たり判定円を取得する（外部クラス用）
	/// </summary>
	/// <param name="out_circle">攻撃の当たり判定円</param>
	bool TryGetHitCircle(CollisionBase::CircleF& out_circle);

	/// <summary>
	/// 画面外にクナイが出たら発射済フラグをFalseにする
	/// </summary>
	void EndKunai();

	/// <summary>
	/// カメラをセットする
	/// </summary>
	/// <param name="camera">カメラのポインタ</param>
	void SetCamera(Camera2D* camera)
	{
		camera_ = camera;
	}

protected:

	/// <summary>
	/// 基準となる中心X座標
	/// </summary>
	float base_center_x_ = 0.0f;

	/// <summary>
	/// 基準となる中心Y座標
	/// </summary>
	float base_center_y_ = 0.0f;

	/// <summary>
	/// 画像の中心X座標
	/// </summary>
	float base_image_center_x_ = 0.0f;

	/// <summary>
	/// 画像の中心Y座標
	/// </summary>
	float base_image_center_y_ = 0.0f;

	/// <summary>
	/// 描画する中心X座標
	/// </summary>
	float draw_center_x_ = 0.0f;

	/// <summary>
	/// 描画する中心Y座標
	/// </summary>
	float draw_center_y_ = 0.0f;

	/// <summary>
	/// クナイの情報への参照をとる
	/// </summary>
	KunaiInfo& kunai_info = kunai_info_;

	/// <summary>
	/// カメラのポインタ
	/// </summary>
	Camera2D* camera_ = nullptr;
};