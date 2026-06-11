#define NOMINMAX //std::minを使うためにWindowsヘッダのマクロ定義をしないようにさせる
#include "PatrolEnemy.h"
#include "DebugManager/DebugManager.h"
#include "Effect/EffectManager.h"
#include "Audio/AudioManager.h"
#include "Player/Player.h"
#include "GameInfo.h"
#include "DxLib.h"
#include <algorithm>


/// <summary>
/// PatrolEnemy.cppで使う定数
/// </summary>
namespace
{
	/// <summary>
	/// 攻撃ヒットSE
	/// </summary>
	const char* kAttackHitSePath = "Assets/Audio/SE/AttackHit.mp3";

	/// <summary>
	/// 敵撃破SE
	/// </summary>
	const char* kEnemyDefeatSePath = "Assets/Audio/SE/EnemyDefeat.mp3";

	/// <summary>
	/// 値を半分にする
	/// </summary>
	const float kHalf = 0.5f;

	/// <summary>
	/// 値を四分の一にする
	/// </summary>
	const float kQuarter = 0.25f;

	/// <summary>
	/// 当たり判定円のオフセットY調整用
	/// </summary>
	const float kHitOffsetAdjustY = 5.0f;

	/// <summary>
	/// パトロールエネミーのHP
	/// </summary>
	const int kPatrolEnemyHp = 1;

	/// <summary>
	/// 被弾時のダメージ
	/// </summary>
	const int kDamage = 1;

	/// <summary>
	/// 重力の値
	/// </summary>
	const float kGravity = 1700.0f;

	/// <summary>
	/// 巡回する範囲
	/// </summary>
	const int kPatrolRange = 100;

	/// <summary>
	/// 移動スピード
	/// </summary>
	const int kPatrolSpeed = 200;

	/// <summary>
	/// 攻撃開始フレーム
	/// </summary>
	const int kAttackStartFrame = 2;

	/// <summary>
	/// 攻撃終了フレーム
	/// </summary>
	const int kAttackFinishFrame = 3;

	/// <summary>
	/// 攻撃当たり判定円のオフセットX
	/// </summary>
	const float kHitCircleOffsetX = 5.0f;

	/// <summary>
	/// 攻撃当たり判定円のオフセットY
	/// </summary>
	const float kHitCircleOffsetY = 5.0f;

	/// <summary>
	/// 攻撃当たり判定円の半径
	/// </summary>
	const float kAttackHitCircleRadius = 35.0f;

	/// <summary>
	/// 攻撃のクールタイム
	/// </summary>
	const float kAttackCoolDown = 1.0f;

	/// <summary>
	/// 敵が攻撃を始める射程距離
	/// </summary>
	const float kEnemyAttackRange = 250.0f;
}

/// <summary>
/// 毎フレームの更新処理
/// </summary>
/// <param name="delta_time">前回実行フレームからの経過時間（秒）</param>
void PatrolEnemy::Update(float delta_time)
{
	//出現していなければ
	if (!enemy_info_.is_appeared)
	{
		//何もしない
		return;
	}

	//アニメセットがなければ
	if (anim_set_ == nullptr)
	{
		//何もしない
		return;
	}
	
	//地面判定を更新
	UpdateGroundYFromTiles();

	//空中処理
	if (!is_on_ground_)
	{
		velocity_y_ += kGravity * delta_time;
		enemy_info_.pos_y += velocity_y_ * delta_time;

		//着地処理
		if (enemy_info_.pos_y >= ground_y_)
		{
			enemy_info_.pos_y = ground_y_;
			velocity_y_ = 0.0f;

			is_on_ground_ = true;
		}
	}

	attack_cooldown_seconds_ = std::max(0.0f, attack_cooldown_seconds_ - delta_time);

	const bool can_start_attack =
		(!is_attacking_) &&
		(attack_cooldown_seconds_ <= 0.0f);

	if (can_start_attack)
	{
		//プレイヤーの当たり判定円
		const CollisionBase::CircleF player_circle = player_->GetHitCircle();
		const float player_center_x = player_circle.center_x;

		//エネミーの中心X座標
		const float enemy_center_x =
			enemy_info_.pos_x + (static_cast<float>(enemy_info_.width) * enemy_info_.scale) * kHalf;

		const float distance_x = enemy_center_x - player_center_x;

		//distance_xの絶対値がkEnemyAttackRangeより小さいなら
		if (std::fabs(distance_x) < kEnemyAttackRange)
		{
			StartAttack();
		}
	}

	const int anim_index = static_cast<int>(current_anim_);

	int frame_count = anim_set_->anim_frame_counts_[anim_index];
	frame_count = std::min(frame_count, EnemyAnimSet::kMaxAnimFrameCount);
	if (frame_count <= 0)
	{
		return;
	}

	const float interval = anim_set_->anim_interval_[anim_index];
	if (interval <= 0.0f)
	{
		return;
	}

	//経過時間を積む
	anim_time_ += delta_time;

	//interval秒ごとにフレームを進める
	while (anim_time_ >= interval)
	{
		anim_time_ -= interval;

		//アニメがAttackの場合
		if (current_anim_ == EnemyAnimSet::AnimKind::kAttack)
		{
			//まだ最後のフレームではないなら進める
			if (current_frame_ < frame_count - 1)
			{
				++current_frame_;
			}
			else
			{
				current_frame_ = frame_count - 1;
			}
		}
		else
		{
			++current_frame_;
			if (current_frame_ >= frame_count)
			{
				//フレームを0に戻してループさせる
				current_frame_ = 0;
			}
		}
	}


	if (is_attacking_)
	{
		Attack();
	}

	if (current_frame_ < 0)
	{
		current_frame_ = 0;
	}
	if (current_frame_ >= frame_count)
	{
		current_frame_ = frame_count - 1;
	}

	image_handle_ = anim_set_->anim_frames_[anim_index][current_frame_];

	const int width = anim_set_->anim_width_[anim_index];
	const int height = anim_set_->anim_height_[anim_index];
	const float scale = enemy_info_.scale;

	camera_x_ = 0.0f;
	camera_y_ = 0.0f;
	if (game_level_ != nullptr)
	{
		camera_x_ = static_cast<float>(game_level_->GetCameraLeftX());
		camera_y_ = static_cast<float>(game_level_->GetCameraTopY());
	}

	const float draw_x = enemy_info_.pos_x;
	const float draw_y = enemy_info_.pos_y;

	//基準にするアニメ
	const int base_anim_index = static_cast<int>(EnemyAnimSet::AnimKind::kRun);

	const int base_height = anim_set_->anim_height_[base_anim_index];
	const int current_height = anim_set_->anim_height_[anim_index];

	//攻撃などで縦が大きいぶんだけ上にずらす（足元を揃える）
	const float height_diff = static_cast<float>(current_height - base_height);
	const float corrected_draw_y = enemy_info_.pos_y - (height_diff * scale);

	//DrawRotaGraph2F は描画位置が中心なので、中心座標に変換する
	center_x_ = (draw_x - camera_x_) + (width * scale) * kHalf;
	center_y_ = (corrected_draw_y - camera_y_) + (static_cast<float>(current_height) * scale) * kHalf;

	//元の画像の中心（基準点）
	img_center_x_ = width * kHalf;
	img_center_y_ = height * kHalf;


	//エフェクトの描画位置の計算
	effect_draw_position_x_ = enemy_info_.pos_x + (enemy_info_.width * kHalf);
	effect_draw_position_y_ = enemy_info_.pos_y;

	if (!is_attacking_)
	{
		//巡回させるための処理
		Patrol(kPatrolRange, kPatrolSpeed, delta_time);
	}
	
}

/// <summary>
/// 毎フレームの描画処理
/// </summary>
void PatrolEnemy::Render()
{
	//出現していなければ
	if (!enemy_info_.is_appeared)
	{
		//何もしない
		return;
	}
	
	//アニメセットがなければ
	if (anim_set_ == nullptr)
	{
		//何もしない
		return;
	}

	//画像ハンドルがロードされてないなら
	if (image_handle_ < 0)
	{
		//何もしない
		return;
	}

	DrawRotaGraph2F(
		center_x_, center_y_,
		img_center_x_, img_center_y_,
		enemy_info_.scale,
		0.0f,
		image_handle_,
		TRUE,
		enemy_info_.facing_left
	);

	//デバッグ用当たり判定円を作る
	if (DebugManager::IsDebugEnabled())
	{
		const float hit_center_world_x = enemy_info_.pos_x + (hit_offset_x_ * enemy_info_.scale);
		const float hit_center_world_y = enemy_info_.pos_y + (hit_offset_y_ * enemy_info_.scale);
		const float hit_radius_world = hit_radius_ * enemy_info_.scale;

		const int hit_draw_x = static_cast<int>(hit_center_world_x - camera_x_);
		const int hit_draw_y = static_cast<int>(hit_center_world_y - camera_y_);
		const int hit_r = static_cast<int>(hit_radius_world);

		DrawCircle(hit_draw_x, hit_draw_y, hit_r, GetColor(255, 0, 0), FALSE);

		CollisionBase::CircleF attack_circle{};
		if (TryGetAttackHitCircle(attack_circle))
		{
			const int draw_x = static_cast<int>(attack_circle.center_x - camera_x_);
			const int draw_y = static_cast<int>(attack_circle.center_y - camera_y_);
			const int r = static_cast<int>(attack_circle.radius);
			DrawCircle(draw_x, draw_y, r, GetColor(0, 255, 0), FALSE);
		}
	}
}

/// <summary>
/// 攻撃開始処理
/// </summary>
void PatrolEnemy::StartAttack()
{
	if (is_attacking_)
	{
		return;
	}

	is_attacking_ = true;
	is_attack_hit_enabled_ = false;

	attack_facing_left_ = enemy_info_.facing_left;

	current_anim_ = EnemyAnimSet::AnimKind::kAttack;
	current_frame_ = 0;
	anim_time_ = 0.0f;
}

/// <summary>
/// 攻撃処理
/// </summary>
void PatrolEnemy::Attack()
{
	if (!is_attacking_)
	{
		return;
	}

	//指定フレームから攻撃判定ON
	if (current_frame_ >= kAttackStartFrame && current_frame_ <= kAttackFinishFrame)
	{
		is_attack_hit_enabled_ = true;
	}
	else
	{
		is_attack_hit_enabled_ = false;
	}

	//最後まで行ったら終了
	const int anim_index = static_cast<int>(current_anim_);
	const int frame_count = anim_set_->anim_frame_counts_[anim_index];
	if (frame_count > 0 && current_frame_ >= frame_count - 1)
	{
		FinishAttack();
	}
}

/// <summary>
/// 攻撃当たり判定円を作成する
/// </summary>
bool PatrolEnemy::TryGetAttackHitCircle(CollisionBase::CircleF& out_circle)
{
	if (!is_attack_hit_enabled_)
	{
		return false;
	}

	const int anim_index = static_cast<int>(current_anim_);
	const int width = anim_set_->anim_width_[anim_index];
	const float scaled_width = static_cast<float>(width) * enemy_info_.scale;

	const float local_x =
		attack_facing_left_
		? (kHitCircleOffsetX * enemy_info_.scale)
		: (scaled_width - (kHitCircleOffsetX * enemy_info_.scale));

	out_circle.center_x = enemy_info_.pos_x + local_x;
	out_circle.center_y = enemy_info_.pos_y + kHitCircleOffsetY;
	out_circle.radius = kAttackHitCircleRadius;
	return true;
}

/// <summary>
/// 攻撃終了処理
/// </summary>
void PatrolEnemy::FinishAttack()
{
	is_attacking_ = false;
	is_attack_hit_enabled_ = false;

	attack_cooldown_seconds_ = kAttackCoolDown;

	current_anim_ = EnemyAnimSet::AnimKind::kRun;
	current_frame_ = 0;
	anim_time_ = 0.0f;
}

/// <summary>
/// 死亡処理
/// </summary>
void PatrolEnemy::DeadEnemy()
{
	effect_manager_->EffectAct(EffectManager::EffectOwner::kEnemy,
		EffectManager::EffectId::kExplosion,
		effect_draw_position_x_, effect_draw_position_y_, true);

	//画面外に出たのと同じ処理をして描画を消す
	EndEnemy();
}

/// <summary>
/// 被ダメージ処理
/// </summary>
void PatrolEnemy::OnHit()
{
	//出現していなければ
	if (!enemy_info_.is_appeared)
	{
		//何もしない
		return;
	}
	else
	{
		//HPを1減らす
		if (enemy_hp_ > 0)
		{
			enemy_hp_ -= kDamage;
			AudioManager::GetInstance()->PlaySe(attack_hit_se_handle_);
		}

		//HPが0になったら
		if (enemy_hp_ <= 0)
		{
			AudioManager::GetInstance()->PlaySe(enemy_defeat_se_handle_);
			DeadEnemy();
		}
	}
}

/// <summary>
/// 初期化処理
/// </summary>
void PatrolEnemy::Initialize()
{
	attack_hit_se_handle_ = AudioManager::GetInstance()->LoadSe(kAttackHitSePath);
	enemy_defeat_se_handle_ = AudioManager::GetInstance()->LoadSe(kEnemyDefeatSePath);

}

/// <summary>
/// 画面外に敵が出たら出現済フラグをFalseにする
/// </summary>
void PatrolEnemy::EndEnemy()
{
	//出現していなければ
	if (!enemy_info_.is_appeared)
	{
		//何もしない
		return;
	}
	else
	{
		//「出現済み」をfalseにする。つまり描画しなくする
		enemy_info_.is_appeared = false;
	}
}

/// <summary>
/// 音声リソースを解放する
/// </summary>
void PatrolEnemy::FinalizeAudio()
{
	if (attack_hit_se_handle_ != -1)
	{
		AudioManager::GetInstance()->DeleteSe(attack_hit_se_handle_);
		attack_hit_se_handle_ = -1;
	}
	if (enemy_defeat_se_handle_ != -1)
	{
		AudioManager::GetInstance()->DeleteSe(enemy_defeat_se_handle_);
		enemy_defeat_se_handle_ = -1;
	}
}


/// <summary>
/// 敵出現処理
/// </summary>
/// <param name="pos_x">X座標</param>
/// <param name="pos_y">Y座標</param>
void PatrolEnemy::AppearEnemy(float pos_x, float pos_y)
{
	//HPのセット
	enemy_hp_ = kPatrolEnemyHp;

	//出現したか？をtureにする
	enemy_info_.is_appeared = true;

	//左向きをtrueにする
	enemy_info_.facing_left = true;

	//座標のセット
	enemy_info_.pos_x = pos_x;

	const float enemy_height_world =
		static_cast<float>(enemy_info_.height) * enemy_info_.scale;

	enemy_info_.pos_y = pos_y - enemy_height_world;

	//アニメーション情報をセット。初期状態をRunにする
	current_anim_ = EnemyAnimSet::AnimKind::kRun;
	current_frame_ = 0;
	anim_time_ = 0.0f;

	//落下処理ができるようにする
	is_on_ground_ = false;
	velocity_y_ = 0.0f;

	//画像の幅、高さをセット
	if (anim_set_ != nullptr)
	{
		const int anim_index = static_cast<int>(current_anim_);
		enemy_info_.width = anim_set_->anim_width_[anim_index];
		enemy_info_.height = anim_set_->anim_height_[anim_index];
	}

	//当たり判定値をセット
	hit_radius_ = enemy_info_.height * kHalf;
	hit_offset_x_ = enemy_info_.width * kHalf;
	hit_offset_y_ = enemy_info_.height - hit_radius_;

	//攻撃関連の値の初期化
	attack_cooldown_seconds_ = 0.0f;
	is_attacking_ = false;
	is_attack_hit_enabled_ = false;

	//当たり判定を設定した後に正確な ground_y を計算する
	UpdateGroundYFromTiles();
}

