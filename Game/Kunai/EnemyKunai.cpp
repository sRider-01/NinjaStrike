#define NOMINMAX //std::max,minを使うためにWindowsヘッダのマクロ定義をしないようにさせる
#include "EnemyKunai.h"
#include "DebugManager/DebugManager.h"
#include "KunaiManager.h"
#include "DxLib.h"
#include "GameInfo.h"
#include "Player/Player.h"
#include <algorithm>

/// <summary>
/// EnemyKunai.cppで使用する定数
/// </summary>
namespace
{
	/// <summary>
	/// クナイの速度
	/// </summary>
	const int kKunaiSpeed = 20;

	/// <summary>
	/// 自機からクナイが出るようにする微調整用値(左向き用)
	/// </summary>
	const int kLeftAdjustX = 250;

	/// <summary>
	/// 自機からクナイが出るようにする微調整用値
	/// </summary>
	const int kAdjustY = 30;

	/// <summary>
	/// エネミークナイの当たり判定四角形を大きくするための値
	/// </summary>
	const int kHitRectHeightPuls = 10;

	/// <summary>
	/// 値を半分にする
	/// </summary>
	const float kHalf = 0.5f;

	/// <summary>
	/// 値を四分の一にする
	/// </summary>
	const float kQuarter= 0.25f;

	/// <summary>
	/// クナイの画面外判定に使うマージン
	/// </summary>
	const float margin = 100.0f;
}


/// <summary>
/// 毎フレームの更新処理
/// </summary>
/// <param name="delta_time">前回実行フレームからの経過時間（秒）</param>
void EnemyKunai::Update(float delta_time)
{

	//発射されていなければ
	if (!kunai_info.is_fired)
	{
		//何もしない
		return;
	}

	if (kunai_info.is_left)
	{
		kunai_info.pos_x -= kKunaiSpeed;
	}
	else
	{
		kunai_info.pos_x += kKunaiSpeed;
	}

	//基準座標を更新
	base_center_x_ = kunai_info.pos_x + base_image_center_x_;
	base_center_y_ = kunai_info.pos_y + base_image_center_y_;


	const float screen_x = camera_->ToScreenX(base_center_x_);
	const float screen_width = static_cast<float>(GameInfo::GetInstance()->GetResolutionX());

	//クナイが画面外に出たら
	if (screen_x < -margin || screen_x >(screen_width + margin))
	{
		//発射済みフラグをFalseにする
		EndKunai();
		return;
	}

	//描画用座標を更新
	draw_center_x_ = camera_->ToScreenX(base_center_x_);
	draw_center_y_ = camera_->ToScreenY(base_center_y_);
}

/// <summary>
/// 毎フレームの描画処理
/// </summary>
void EnemyKunai::Render()
{
	//発射されていなければ
	if (!kunai_info.is_fired)
	{
		//何もしない
		return;
	}

	//画像描画
	DrawRotaGraph2F(
		draw_center_x_, draw_center_y_,
		base_image_center_x_, base_image_center_y_,
		1.0, 0.0,
		kunai_image_handle_,
		TRUE,
		kunai_info.is_left
	);


	//デバッグ用当たり判定円作成
	const CollisionBase::CircleF hit_circle = MakeHitCircle();

	float screen_x = hit_circle.center_x;
	float screen_y = hit_circle.center_y;

	if (camera_ != nullptr)
	{
		screen_x = camera_->ToScreenX(hit_circle.center_x);
		screen_y = camera_->ToScreenY(hit_circle.center_y);
	}

	if (DebugManager::IsDebugEnabled())
	{
		DrawCircle(
			static_cast<int>(screen_x),
			static_cast<int>(screen_y),
			static_cast<int>(hit_circle.radius),
			GetColor(255, 0, 0),
			FALSE
		);
	}
}

/// <summary>
/// クナイ発射処理
/// </summary>
void EnemyKunai::Fire(float pos_x, float pos_y, bool is_facing_left)
{

	//クナイの発射済みフラグをtureにする
	kunai_info.is_fired = true;

	//左向きかをセット
	kunai_info.is_left = is_facing_left;


	//座標のセット
	kunai_info.pos_x = pos_x;

	kunai_info.pos_y = pos_y - kAdjustY;

	if (is_facing_left)
	{
		kunai_info.pos_x = pos_x - kLeftAdjustX;
	}

	//画像の幅、高さを計算
	const float image_width = static_cast<float>(kunai_info.kunai_width);
	const float image_height = static_cast<float>(kunai_info.kunai_height);

	base_image_center_x_ = image_width * kHalf;
	base_image_center_y_ = image_height * kHalf;


	base_center_x_ = kunai_info.pos_x + base_image_center_x_;
	base_center_y_ = kunai_info.pos_y + base_image_center_y_;

	//カメラ座標へ変換
	draw_center_x_ = camera_->ToScreenX(base_center_x_);
	draw_center_y_ = camera_->ToScreenY(base_center_y_);

}

/// <summary>
/// 当たり判定円を作る
/// </summary>
CollisionBase::CircleF EnemyKunai::MakeHitCircle() const
{
	CollisionBase::CircleF circle{};
	circle.center_x = kunai_info.pos_x + kunai_info.kunai_width;
	circle.center_y = kunai_info.pos_y + kunai_info.kunai_height * kHalf;
	circle.radius = kunai_info.kunai_width * kQuarter;


	return circle;
}

/// <summary>
/// 攻撃の当たり判定円を取得する（外部クラス用）
/// </summary>
bool EnemyKunai::TryGetHitCircle(CollisionBase::CircleF& out_circle)
{
	//発射されていないなら当たり判定は無し
	if (!kunai_info.is_fired)
	{
		return false;
	}

	const float width = static_cast<float>(kunai_info.kunai_width);
	const float height = static_cast<float>(kunai_info.kunai_height);

	//pos_x,pos_y が「左上」前提で中心を作る
	const float center_x = kunai_info.pos_x + width * kHalf;
	const float center_y = kunai_info.pos_y + height * kHalf;

	out_circle.center_x = center_x;
	out_circle.center_y = center_y;
	out_circle.radius = width * kHalf;

	return true;
}

/// <summary>
/// 画面外にクナイが出たら発射済フラグをFalseにする
/// </summary>
void EnemyKunai::EndKunai()
{
	//発射されていなければ
	if (!kunai_info.is_fired)
	{
		//何もしない
		return;
	}
	else
	{
		//「発射済み」をfalseにする。つまり描画しなくする
		kunai_info.is_fired = false;
	}
}
