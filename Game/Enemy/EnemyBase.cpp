#define NOMINMAX //std::maxを使うためにWindowsヘッダのマクロ定義をしないようにさせる
#include "EnemyBase.h"
#include "GameInfo.h"
#include "Level/GameLevel.h"
#include "TileMap/TileMap.h"
#include "TileMap/TileDatabase.h"
#include "TileMap/TileSet.h"
#include "Effect/EffectManager.h"
#include "DxLib.h"
#include <algorithm>


/// <summary>
/// EnemyBase.cppでのみ使用する定数
/// </summary>
namespace
{
	/// <summary>
	/// 当たり判定円の大きさの倍率
	/// </summary>
	const float kHitRadiusRate = 0.4f;

	/// <summary>
	/// 足場判定に使う「左右2点」
	/// </summary>
	const int kTwoPoint = 2;

	/// <summary>
	/// 足場判定に使う画像の少し内側に判定を持ってくるマージン
	/// </summary>
	const float kMargin = 4.0f;

	/// <summary>
	/// 画面外判定に使うマージン
	/// </summary>
	const float kDespawnMargin = 100.0f;

}

/// <summary>
/// コンストラクタ
/// </summary>
EnemyBase::EnemyBase()
{
}


/// <summary>
/// クナイを発射する処理(ボス用)
/// </summary>
void EnemyBase::Fire(float pos_x, float pos_y)
{
	//空の仮想関数にしておく
}

/// <summary>
/// 敵出現処理
/// </summary>
void EnemyBase::AppearEnemy(float pos_x, float pos_y)
{
	//空の仮想関数にしておく
}

/// <summary>
/// 攻撃開始処理
/// </summary>
void EnemyBase::StartAttack()
{
	//空の仮想関数にしておく
}

/// <summary>
/// 遠距離攻撃準備処理
/// </summary>
void EnemyBase::PreparationLongRangeAttack()
{
	//空の仮想関数にしておく
}

/// <summary>
/// 遠距離攻撃開始処理
/// </summary>
void EnemyBase::StartLongRangeAttack()
{
	//空の仮想関数にしておく
}

/// <summary>
/// 攻撃当たり判定円を作成する
/// </summary>
bool EnemyBase::TryGetAttackHitCircle(CollisionBase::CircleF& out_circle)
{
	return false;
}

/// <summary>
/// 攻撃終了処理
/// </summary>
void EnemyBase::FinishAttack()
{
	//空の仮想関数にしておく
}

/// <summary>
/// 攻撃処理
/// </summary>
void EnemyBase::Attack()
{
	//空の仮想関数にしておく
}

/// <summary>
/// 死亡処理
/// </summary>
void EnemyBase::DeadEnemy()
{
	//空の仮想関数にしておく
}

/// <summary>
/// 被ダメージ処理
/// </summary>
void EnemyBase::OnHit()
{
	//空の仮想関数にしておく
}

/// <summary>
/// 敵本体の当たり判定円を作成する
/// </summary>
CollisionBase::CircleF EnemyBase::GetHitCircle()
{
	CollisionBase::CircleF circle{};
	const float scale = enemy_info_.scale;

	circle.center_x = enemy_info_.pos_x + (hit_offset_x_ * scale);
	circle.center_y = enemy_info_.pos_y + (hit_offset_y_ * scale);
	circle.radius = hit_radius_ * scale;

	return circle;
}


/// <summary>
/// 足元タイルから地面判定を取得する
/// </summary>
void EnemyBase::UpdateGroundYFromTiles()
{
	if (game_level_ == nullptr)
	{
		return;
	}

	TileMap* tile_map = game_level_->GetTileMap();
	TileDatabase* tile_db = game_level_->GetTileDatabase();
	TileSet* tile_set = game_level_->GetTileSet();

	if (tile_map == nullptr || tile_db == nullptr || tile_set == nullptr)
	{
		return;
	}

	const int tile_width = tile_set->GetTileWidth();
	const int tile_height = tile_set->GetTileHeight();
	if (tile_width <= 0 || tile_height <= 0)
	{
		return;
	}

	const int map_width = tile_map->GetWidth();
	const int map_height = tile_map->GetHeight();
	if (map_width <= 0 || map_height <= 0)
	{
		return;
	}

	//敵本体の当たり判定円を使う
	const CollisionBase::CircleF body_circle = GetHitCircle();

	//左右の足元2点
	float sample_x[kTwoPoint] =
	{
		body_circle.center_x - body_circle.radius + kMargin,
		body_circle.center_x + body_circle.radius - kMargin
	};

	//足元のY
	const float foot_y = body_circle.center_y + body_circle.radius;

	//pos_y から足元までの距離
	const float foot_offset_from_top = foot_y - enemy_info_.pos_y;

	bool found = false;
	float best_ground_y = 0.0f;

	for (int n = 0; n < kTwoPoint; ++n)
	{
		int tile_x = static_cast<int>(sample_x[n]) / tile_width;
		tile_x = std::clamp(tile_x, 0, map_width - 1);

		int start_tile_y = static_cast<int>(foot_y) / tile_height;
		start_tile_y = std::clamp(start_tile_y, 0, map_height - 1);

		for (int tile_y = start_tile_y; tile_y < map_height; ++tile_y)
		{
			const int tile_id = tile_map->GetTileId(tile_x, tile_y);
			if (!tile_db->IsSolid(tile_id))
			{
				continue;
			}

			//足元がタイル上面に合うように、左上Yを逆算
			const float ground_y = static_cast<float>(tile_y * tile_height) - foot_offset_from_top;

			if (!found || ground_y < best_ground_y)
			{
				best_ground_y = ground_y;
				found = true;
			}
			break;
		}
	}

	if (found)
	{
		ground_y_ = best_ground_y;
	}
}


/// <summary>
/// 座標をセットする
/// </summary>
void EnemyBase::SetPosition(float x, float y)
{
	enemy_info_.pos_x = x;
	enemy_info_.pos_y = y;
}

/// <summary>
/// 周囲を移動する処理
/// </summary>
void EnemyBase::Patrol(float range, float speed, float delta_time)
{
	//出現していないなら何もしない
	if (!enemy_info_.is_appeared)
	{
		return;
	}
	//範囲が不正なら何もしない
	if (range <= 0.0f || speed <= 0.0f || delta_time <= 0.0f)
	{
		return;
	}

	//初期設定をしていないならば
	if (!is_patrol_initialized_)
	{
		//巡回の中心を今自分がいるX座標を基準とする
		const float center_x = enemy_info_.pos_x;

		//巡回ルートの端座標を設定
		patrol_left_x_ = center_x - range;
		patrol_right_x_ = center_x + range;
		is_patrol_initialized_ = true;
	}

	const float move_speed = speed * delta_time;

	if (enemy_info_.facing_left)
	{
		//左へ移動
		enemy_info_.pos_x -= move_speed;

		//左端に到達したら反転
		if (enemy_info_.pos_x <= patrol_left_x_)
		{
			enemy_info_.pos_x = patrol_left_x_;
			enemy_info_.facing_left = false;
		}
	}
	else
	{
		enemy_info_.pos_x += move_speed;

		//右端に到達したら反転
		if (enemy_info_.pos_x >= patrol_right_x_)
		{
			enemy_info_.pos_x = patrol_right_x_;
			enemy_info_.facing_left = true;
		}
	}
}

/// <summary>
/// 画面外に敵が出たら出現済フラグをFalseにする
/// </summary>
void EnemyBase::EndEnemy()
{
	enemy_info_.is_appeared = false;
}