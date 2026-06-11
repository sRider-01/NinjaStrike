#define NOMINMAX
#include "BossEnemy.h"
#include "DebugManager/DebugManager.h"
#include "TileMap/TileMap.h"
#include "TileMap/TileDatabase.h"
#include "TileMap/TileSet.h"
#include "Effect/EffectManager.h"
#include "Player/Player.h"
#include "Audio/AudioManager.h"
#include "GameInfo.h"
#include "DxLib.h"
#include <algorithm>


/// <summary>
/// BossEnemy.cppで使う定数
/// </summary>
namespace
{
	/// <summary>
	/// 値を半分にする
	/// </summary>
	const float kHalf = 0.5f;

	/// <summary>
	/// 値を四分の一にする
	/// </summary>
	const float kQuarter = 0.25f;

	/// <summary>
	/// 通常ボスの能力値
	/// </summary>
	const BossEnemy::BossParameter kNormalBossParameter =
	{
		50,     // max_hp
		0.8f,   // min_attack_interval
		2.0f,   // max_attack_interval
		0.2f,   // long_range_attack_chance
		150.0f, // attack_range
		0.20f,  // jump_chance
		800.0f, // jump_speed
		200.0f, // move_speed
		0.65f,  // move_chance
		0.10f,  // guard_chance
		0.60f,  // guard_cooldown_seconds
		1       // damage
	};

	/// <summary>
	/// 強化ボスの能力値
	/// </summary>
	const BossEnemy::BossParameter kEnhancedBossParameter =
	{
		60,     // max_hp
		0.5f,   // min_attack_interval
		1.2f,   // max_attack_interval
		0.35f,  // long_range_attack_chance
		200.0f, // attack_range
		0.40f,  // jump_chance
		1000.0f, // jump_speed
		500.0f, // move_speed
		0.75f,  // move_chance
		0.20f,  // guard_chance
		0.30f,  // guard_cooldown_seconds
		2       // damage
	};

	/// <summary>
	/// 攻撃ヒットSE
	/// </summary>
	const char* kAttackHitSePath = "Assets/Audio/SE/AttackHit.mp3";

	/// <summary>
	/// ガードSE
	/// </summary>
	const char* kGuardSePath = "Assets/Audio/SE/Guard.mp3";

	/// <summary>
	/// 敵撃破SE
	/// </summary>
	const char* kEnemyDefeatSePath = "Assets/Audio/SE/EnemyDefeat.mp3";

	/// <summary>
	/// 被ダメージ時のフラッシュ秒数
	/// </summary>
	const float kDamageFlashSeconds = 0.02f;

	/// <summary>
	/// 攻撃のクールタイム
	/// </summary>
	const float kAttackCoolDown = 1.0f;

	/// <summary>
	/// 敵の手の位置のオフセットX
	/// </summary>
	const float kHandOffsetX = 300.0f;

	/// <summary>
	/// 敵の手の位置のオフセットY
	/// </summary>
	const float kHandOffsetY = 75.0f;

	/// <summary>
	/// 遠距離攻撃でクナイを投げるフレーム
	/// </summary>
	const int kLongRangeFireFrame = 4;

	/// <summary>
	/// 近距離攻撃開始フレーム
	/// </summary>
	const int kCloseAttackStartFrame = 5;

	/// <summary>
	/// 攻撃終了フレーム
	/// </summary>
	const int kAttackFinishFrame = 7;

	/// <summary>
	/// 画面端にめり込まないためのマージン
	/// </summary>
	const float kMoveScreenMargin = 32.0f;

	/// <summary>
	/// 当たり判定四角形のX座標を縮める値
	/// </summary>
	const float kShrinkX = 80.0f;

	/// <summary>
	/// 当たり判定四角形の上部を縮める値
	/// </summary>
	const float kShrinkTop = 25.0f;

	/// <summary>
	/// 当たり判定四角形の下部を縮める値
	/// </summary>
	const float kShrinkBottom = 2.0f;

	/// <summary>
	/// 地面判定で使用する判定を下にずらす量
	/// </summary>
	const float kGroundProbe = 2.0f;

	/// <summary>
	/// 最低待機時間
	/// </summary>
	const float kMinIdleSeconds = 0.4f;

	/// <summary>
	/// 最大待機時間
	/// </summary>
	const float kMaxIdleSeconds = 1.2f;

	/// <summary>
	/// 最短移動時間(目的地に届かなくても切り替えるため)
	/// </summary>
	const float kMinMoveSeconds = 0.7f;

	/// <summary>
	/// 最長移動時間
	/// </summary>
	const float kMaxMoveSeconds = 1.5f;

	/// <summary>
	/// 目的地への到着判定に使用する値（到着とする誤差）
	/// </summary>
	const float kMoveArrive = 2.0f;

	/// <summary>
	/// 攻撃当たり判定円のオフセットX
	/// </summary>
	const float kHitCircleOffsetX = 50.0f;

	/// <summary>
	/// 攻撃当たり判定円のオフセットY
	/// </summary>
	const float kHitCircleOffsetY = 55.0f;

	/// <summary>
	/// 攻撃当たり判定円の半径
	/// </summary>
	const float kAttackHitCircleRadius = 35.0f;

	/// <summary>
	/// 敵の描画倍率
	/// </summary>
	const float kEnemyScale = 3.0f;

	/// <summary>
	/// 重力の値
	/// </summary>
	const float kGravity = 1700.0f;

	/// <summary>
	/// 敵の当たり判定円の半径縮める用の値
	/// </summary>
	const float kEnemyRadiusShrink = 7.0f;

	/// <summary>
	/// ボスが受けるダメージ
	/// </summary>
	const int kDamage = 1;
}

/// <summary>
/// 毎フレームの更新処理
/// </summary>
/// <param name="delta_time">前回実行フレームからの経過時間（秒）</param>
void BossEnemy::Update(float delta_time)
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

	//被ダメージフラッシュ時間を減らす
	damage_flash_seconds_ = std::max(0.0f, damage_flash_seconds_ - delta_time);

	const bool was_on_ground = is_on_ground_;
	if (was_on_ground)
	{
		is_on_ground_ = IsOnGroundByProbe(kGroundProbe); //Boss側の probe 値
	}

	if (was_on_ground && !is_on_ground_)
	{
		if (velocity_y_ < 0.0f)
		{
			velocity_y_ = 0.0f;
		}
	}

	if (!is_on_ground_)
	{
		velocity_y_ += kGravity * delta_time;
	}
	else
	{
		if (velocity_y_ > 0.0f)
		{
			velocity_y_ = 0.0f;
		}
	}


	const float prev_y = enemy_info_.pos_y;
	enemy_info_.pos_y += velocity_y_ * delta_time;
	const float moved_y = enemy_info_.pos_y - prev_y;

	const bool landed_this_frame = GroundCollision(moved_y);

	if (landed_this_frame)
	{
		if (!is_attacking_ && current_anim_ == EnemyAnimSet::AnimKind::kJump)
		{
			current_anim_ = EnemyAnimSet::AnimKind::kIdle;
			current_frame_ = 0;
			anim_time_ = 0.0f;
		}
	}

	guard_cooldown_seconds_ = std::max(0.0f, guard_cooldown_seconds_ - delta_time);


	//Damage中・攻撃中は動かさない
	const bool can_move =
		(enemy_info_.is_appeared) &&
		(!is_attacking_) &&
		(!is_guarding_) &&
		(is_alive_) &&
		(current_anim_ != EnemyAnimSet::AnimKind::kDamage) &&
		(is_on_ground_) &&
		(game_level_ != nullptr && !game_level_->IsInDialogue());

	if (can_move)
	{
		UpdateMove(delta_time);
	}


	attack_cooldown_seconds_ = std::max(0.0f, attack_cooldown_seconds_ - delta_time);

	//Damage中は攻撃開始しない（優先度：Damage > Attack）
	const bool can_start_attack =
		(!is_attacking_) &&
		(!is_guarding_) &&
		(is_alive_) &&
		(attack_cooldown_seconds_ <= 0.0f) &&
		(current_anim_ != EnemyAnimSet::AnimKind::kDamage) &&
		(game_level_ != nullptr && !game_level_->IsInDialogue());;

	if (can_start_attack)
	{
		//プレイヤーの当たり判定円
		const CollisionBase::CircleF player_circle = player_->GetHitCircle();
		const float player_center_x = player_circle.center_x;

		//ボスの中心X座標
		const float boss_center_x =
			enemy_info_.pos_x + (static_cast<float>(enemy_info_.width) * enemy_info_.scale) * kHalf;

		const float distance_x = boss_center_x - player_center_x;

		//distance_xの絶対値がboss\parameter_.attack_rangeより小さいなら
		if (std::fabs(distance_x) < boss_parameter_.attack_range)
		{
			StartAttack();
		}
		else
		{
			//遠いなら：遠距離攻撃を必ず開始（範囲に依存しない）
			PreparationLongRangeAttack();
		}

	}

	int anim_index = static_cast<int>(current_anim_);

	int frame_count = anim_set_->anim_frame_counts_[anim_index];
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

		//アニメがDamageの場合
		if (current_anim_ == EnemyAnimSet::AnimKind::kDamage)
		{
			//まだ最後のフレームではないなら進める
			if (current_frame_ < frame_count - 1)
			{
				++current_frame_;
			}
			else
			{
				//最後まで再生したらIdleにもどす
				current_anim_ = EnemyAnimSet::AnimKind::kIdle;
				current_frame_ = 0;
				anim_time_ = 0.0f;
				break;
			}
		}
		else if (current_anim_ == EnemyAnimSet::AnimKind::kGuard)
		{
			if (current_frame_ < frame_count - 1)
			{
				++current_frame_;
			}
			else
			{
				//最後まで再生したらガード終了
				FinishGuard();
				break;
			}
		}
		//アニメがJumpの場合
		else if (current_anim_ == EnemyAnimSet::AnimKind::kJump)
		{
			if (current_frame_ < frame_count - 1)
			{
				++current_frame_;
			}
			else
			{
				current_frame_ = frame_count - 1;
			}
		}
		else if (current_anim_ == EnemyAnimSet::AnimKind::kDead)
		{
			if (current_frame_ < frame_count - 1)
			{
				++current_frame_;
			}
			else
			{
				DeadEnemy();
				return;
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

	if (is_alive_ && is_attacking_)
	{
		Attack();
	}


	//アニメーションが変更した時のためanim_indexとframe_countを取り直す
	anim_index = static_cast<int>(current_anim_);
	frame_count = anim_set_->anim_frame_counts_[anim_index];
	if (frame_count <= 0)
	{
		return;
	}

	if (current_frame_ < 0)
	{
		current_frame_ = 0;
	}
	if (current_frame_ >= frame_count)
	{
		current_frame_ = frame_count - 1;
	}

	const int handle = anim_set_->anim_frames_[anim_index][current_frame_];
	if (handle >= 0)
	{
		//値が有効なときだけ更新(=切り替え時一瞬「-1」になっても大丈夫なように)
		image_handle_ = handle;
	}
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

	//DrawRotaGraph2F は描画位置が中心なので、中心座標に変換する
	center_x_ = (draw_x - camera_x_) + (width * scale) * kHalf;
	center_y_ = (draw_y - camera_y_) + (height * scale) * kHalf;

	//元の画像の中心（基準点）
	img_center_x_ = width * kHalf;
	img_center_y_ = height * kHalf;


	//エフェクトの描画位置の計算
	effect_draw_position_x_ = enemy_info_.pos_x + (enemy_info_.width * kHalf);
	effect_draw_position_y_ = enemy_info_.pos_y;

}

/// <summary>
/// 毎フレームの描画処理
/// </summary>
void BossEnemy::Render()
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

	if (damage_flash_seconds_ > 0.0f)
	{
		SetDrawBlendMode(DX_BLENDMODE_ADD, 255);

		//白さの重ね掛け数
		const int kFlashDrawCount = 5;

		for (int i = 0; i < kFlashDrawCount; ++i)
		{
			DrawRotaGraph2F(
				center_x_, center_y_,
				img_center_x_, img_center_y_,
				enemy_info_.scale,
				0.0f,
				image_handle_,
				TRUE,
				enemy_info_.facing_left
			);
		}

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	//デバッグ用当たり判定円
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
void BossEnemy::StartAttack()
{
	if (is_attacking_)
	{
		return;
	}

	is_attacking_ = true;
	is_attack_hit_enabled_ = false;
	is_long_range_attacking_ = false;

	const float random_num = GetRandomFloat(0.0f, 1.0f);

	if (random_num < boss_parameter_.long_range_attack_chance)
	{
		current_anim_ = EnemyAnimSet::AnimKind::kLongRangeAttack;
		current_frame_ = 0;
		anim_time_ = 0.0f;
	}
	else
	{
		current_anim_ = EnemyAnimSet::AnimKind::kAttack;
		current_frame_ = 0;
		anim_time_ = 0.0f;
	}

}

/// <summary>
/// 遠距離攻撃準備処理
/// </summary>
void BossEnemy::PreparationLongRangeAttack()
{
	if (is_attacking_)
	{
		return;
	}

	is_attacking_ = true;
	is_attack_hit_enabled_ = false;
	is_long_range_attacking_ = false;

	current_anim_ = EnemyAnimSet::AnimKind::kLongRangeAttack;
	current_frame_ = 0;
	anim_time_ = 0.0f;
}

/// <summary>
/// 遠距離攻撃開始処理
/// </summary>
void BossEnemy::StartLongRangeAttack()
{

	attack_facing_left_ = enemy_info_.facing_left;


	const float hand_x = enemy_info_.pos_x + kHandOffsetX;
	const float hand_y = enemy_info_.pos_y + kHandOffsetY;

	kunai_manager_->Fire(KunaiManager::KunaiKind::kEnemyKunai, hand_x, hand_y, attack_facing_left_);
}

/// <summary>
/// 攻撃処理
/// </summary>
void BossEnemy::Attack()
{
	if (!is_attacking_)
	{
		return;
	}

	if (current_anim_ == EnemyAnimSet::AnimKind::kLongRangeAttack)
	{
		if (!is_long_range_attacking_ && current_frame_ >= kLongRangeFireFrame)
		{
			StartLongRangeAttack();
			is_long_range_attacking_ = true;
		}

		//近距離攻撃当たり判定は出さない
		is_attack_hit_enabled_ = false;
	}
	else
	{
		//指定フレームから攻撃判定ON（近距離攻撃をしている場合）
		if (current_frame_ >= kCloseAttackStartFrame && current_frame_ <= kAttackFinishFrame && current_anim_ == EnemyAnimSet::AnimKind::kAttack)
		{
			is_attack_hit_enabled_ = true;
		}
		else
		{
			is_attack_hit_enabled_ = false;
		}
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
bool BossEnemy::TryGetAttackHitCircle(CollisionBase::CircleF& out_circle)
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
void BossEnemy::FinishAttack()
{
	is_attacking_ = false;
	is_attack_hit_enabled_ = false;
	is_long_range_attacking_ = false;

	attack_cooldown_seconds_ = GetRandomFloat(boss_parameter_.min_attack_interval, boss_parameter_.max_attack_interval);

	current_anim_ = EnemyAnimSet::AnimKind::kIdle;
	current_frame_ = 0;
	anim_time_ = 0.0f;
}

/// <summary>
/// 死亡処理
/// </summary>
void BossEnemy::DeadEnemy()
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
void BossEnemy::OnHit()
{
	//出現していなければ
	if (!enemy_info_.is_appeared)
	{
		//何もしない
		return;
	}

	//やられたら
	if (!is_alive_)
	{
		//何もしない
		return;
	}

	//ガード中はダメージ無効
	if (is_guarding_)
	{
		return;
	}
	
	//ガード抽選可能か？
	const bool can_try_guard =
		(!is_guarding_) &&
		(guard_cooldown_seconds_ <= 0.0f) &&
		(current_anim_ != EnemyAnimSet::AnimKind::kDamage) &&
		(is_on_ground_);

	if (can_try_guard)
	{
		const float random_num = GetRandomFloat(0.0f, 1.0f);
		if (random_num < boss_parameter_.guard_chance)
		{
			StartGuard();
			
			//ガード成功：ダメージ処理に入らない
			return; 
		}
	}

	if (enemy_hp_ > 0)
	{
		enemy_hp_ -= kDamage;
		damage_flash_seconds_ = kDamageFlashSeconds;

		AudioManager::GetInstance()->PlaySe(attack_hit_se_handle_);
	}

	if (enemy_hp_ <= 0)
	{
		is_alive_ = false;

		is_attacking_ = false;
		is_attack_hit_enabled_ = false;
		is_long_range_attacking_ = false;
		is_guarding_ = false;
		is_moving_ = false;

		current_anim_ = EnemyAnimSet::AnimKind::kDead;
		current_frame_ = 0;
		anim_time_ = 0.0f;

		AudioManager::GetInstance()->PlaySe(enemy_defeat_se_handle_);
		return;
	}
}

/// <summary>
/// 初期化処理
/// </summary>
void BossEnemy::Initialize()
{
	attack_hit_se_handle_ = AudioManager::GetInstance()->LoadSe(kAttackHitSePath);
	guard_se_handle_ = AudioManager::GetInstance()->LoadSe(kGuardSePath);
	enemy_defeat_se_handle_ = AudioManager::GetInstance()->LoadSe(kEnemyDefeatSePath);
}

/// <summary>
/// 画面外に敵が出たら出現済フラグをFalseにする
/// </summary>
void BossEnemy::EndEnemy()
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
void BossEnemy::FinalizeAudio()
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
void BossEnemy::AppearEnemy(float pos_x, float pos_y)
{
	if (boss_form_ == BossForm::kNormal)
	{
		boss_parameter_ = kNormalBossParameter;
	}
	else
	{
		boss_parameter_ = kEnhancedBossParameter;
	}

	is_alive_ = true;

	//HPをセット
	enemy_hp_ = boss_parameter_.max_hp;

	//出現したか？をtureにする
	enemy_info_.is_appeared = true;

	//左向きをtrueにする
	enemy_info_.facing_left = true;

	//座標のセット
	enemy_info_.pos_x = pos_x;

	//画面外から画面内に入ってくるようにする
	enemy_info_.pos_y = pos_y;

	//アニメーション情報をセット
	current_anim_ = EnemyAnimSet::AnimKind::kIdle;
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
	hit_offset_x_ = enemy_info_.width * kHalf;
	hit_offset_y_ = enemy_info_.height * kHalf;
	hit_radius_ = (enemy_info_.height * kHalf) - kEnemyRadiusShrink;

	random_engine_.seed(static_cast<unsigned int>(GetNowCount()));

	//最初の攻撃までの待ち時間をランダム化
	attack_cooldown_seconds_ = GetRandomFloat(
        boss_parameter_.min_attack_interval,
        boss_parameter_.max_attack_interval
    );


	//ガードに関する値をリセット
	is_guarding_ = false;
	guard_cooldown_seconds_ = 0.0f;

	//移動に関する値をリセット
	move_center_x_ = enemy_info_.pos_x;
	move_target_x_ = enemy_info_.pos_x;

	move_state_timer_ = 0.0f;

	is_moving_ = false;
	is_moving_initialized_ = true;

	ChooseNextAction();
}

/// <summary>
/// 移動処理の更新
/// </summary>
void BossEnemy::UpdateMove(float delta_time)
{
	if (!is_moving_initialized_)
	{
		move_center_x_ = enemy_info_.pos_x;
		move_target_x_ = enemy_info_.pos_x;

		move_state_timer_ = 0.0f;

		is_moving_ = false;
		is_moving_initialized_ = true;

		ChooseNextAction();
	}

	move_state_timer_ = std::max(0.0f, move_state_timer_ - delta_time);

	if (is_moving_)
	{
		const float distance_x = move_target_x_ - enemy_info_.pos_x;

		if (std::fabs(distance_x) <= kMoveArrive)
		{
			enemy_info_.pos_x = move_target_x_;

			is_moving_ = false;

			ChooseNextAction();
			return;
		}

		//distance_xが負か？負なら-1.0f違うなら1.0fをdirに代入
		const float dir = (distance_x < 0.0f) ? -1.0f : 1.0f;

		//向きを更新
		enemy_info_.facing_left = (dir < 0.0f);

		const float prev_x = enemy_info_.pos_x;

		//移動処理
		enemy_info_.pos_x += dir * boss_parameter_.move_speed * delta_time;

		const float moved_x = enemy_info_.pos_x - prev_x;

		//壁押し戻し
		const bool hit_wall = WallCollision(moved_x);
		if (hit_wall)
		{
			is_moving_ = false;
			ChooseNextAction();
			return;
		}

		//行き過ぎを防止する
		const float latest_distance_x = move_target_x_ - enemy_info_.pos_x;

		if ((distance_x > 0.0f && latest_distance_x < 0.0f) || (distance_x < 0.0f && latest_distance_x > 0.0f))
		{
			enemy_info_.pos_x = move_target_x_;

			is_moving_ = false;

			ChooseNextAction();
			return;
		}

		//時間切れなら
		if (move_state_timer_ <= 0.0f)
		{
			ChooseNextAction();
			return;
		}
	}
	else
	{
		//待機状態
		//時間切れなら
		if (move_state_timer_ <= 0.0f)
		{
			ChooseNextAction();
			return;
		}
	}


	UpdateMoveAnimation();
}

/// <summary>
/// 次の行動の選択
/// </summary>
void BossEnemy::ChooseNextAction()
{
	float camera_left_x = 0.0f;
	if (game_level_ != nullptr)
	{
		camera_left_x = static_cast<float>(game_level_->GetCameraLeftX());
	}

	const float screen_width = static_cast<float>(GameInfo::GetInstance()->GetResolutionX());
	const float camera_right_x = camera_left_x + screen_width;

	const float scaled_width = static_cast<float>(enemy_info_.width) * enemy_info_.scale;

	float min_x = camera_left_x + kMoveScreenMargin;
	float max_x = camera_right_x - scaled_width - kMoveScreenMargin;

	if (max_x < min_x)
	{
		min_x = enemy_info_.pos_x;
		max_x = enemy_info_.pos_x;
	}

	const float random_num = GetRandomFloat(0.0f, 1.0f);

	//ジャンプについて
	if (random_num < boss_parameter_.jump_chance)
	{
		is_moving_ = false;
		move_target_x_ = enemy_info_.pos_x;
		move_state_timer_ = GetRandomFloat(kMinIdleSeconds, kMaxIdleSeconds);

		StartJump();
		return;
	}

	//移動
	if (random_num < boss_parameter_.move_chance + boss_parameter_.jump_chance)
	{
		//移動状態にする
		is_moving_ = true;

		//目的地をランダムに設定
		move_target_x_ = GetRandomFloat(min_x, max_x);

		//移動時間をセット
		move_state_timer_ = GetRandomFloat(kMinMoveSeconds, kMaxMoveSeconds);
	}
	else
	{
		is_moving_ = false;

		move_target_x_ = enemy_info_.pos_x;

		move_state_timer_ = GetRandomFloat(kMinIdleSeconds, kMaxIdleSeconds);
	}
}

/// <summary>
/// ジャンプの開始
/// </summary>
void BossEnemy::StartJump()
{
	//空中ならジャンプ開始しない
	if (!is_on_ground_)
	{
		return;
	}

	is_on_ground_ = false;
	velocity_y_ = -boss_parameter_.jump_speed;

	 current_anim_ = EnemyAnimSet::AnimKind::kJump;
	 current_frame_ = 0;
	 anim_time_ = 0.0f;
}

/// <summary>
/// 防御開始処理
/// </summary>
void BossEnemy::StartGuard()
{
	is_guarding_ = true;
	guard_cooldown_seconds_ = boss_parameter_.guard_cooldown_seconds;

	//ガードが発生した瞬間に、攻撃中だったら止める（見た目・挙動が自然）
	is_attacking_ = false;
	is_attack_hit_enabled_ = false;
	is_long_range_attacking_ = false;

	current_anim_ = EnemyAnimSet::AnimKind::kGuard;
	current_frame_ = 0;
	anim_time_ = 0.0f;

	AudioManager::GetInstance()->PlaySe(guard_se_handle_);
}

/// <summary>
/// 防御終了処理
/// </summary>
void BossEnemy::FinishGuard()
{
	is_guarding_ = false;

	current_anim_ = EnemyAnimSet::AnimKind::kIdle;
	current_frame_ = 0;
	anim_time_ = 0.0f;
}

/// <summary>
/// 地面との当たり判定処理
/// </summary>
bool BossEnemy::GroundCollision(float move_y)
{
	if (move_y == 0.0f)
	{
		return false;
	}

	if (game_level_ == nullptr)
	{
		return false;
	}

	is_on_ground_ = false;

	TileMap* tile_map = game_level_->GetTileMap();
	TileDatabase* tile_db = game_level_->GetTileDatabase();
	TileSet* tile_set = game_level_->GetTileSet();

	if (tile_map == nullptr || tile_db == nullptr || tile_set == nullptr)
	{
		return false;
	}

	const int tile_width = tile_set->GetTileWidth();
	const int tile_height = tile_set->GetTileHeight();
	if (tile_width <= 0 || tile_height <= 0)
	{
		return false;
	}

	const int map_width = tile_map->GetWidth();
	const int map_height = tile_map->GetHeight();
	if (map_width <= 0 || map_height <= 0)
	{
		return false;
	}

	CollisionBase::RectF body = MakeBodyRect();

	int start_x = static_cast<int>(body.left) / tile_width;
	int end_x = static_cast<int>(body.right) / tile_width;
	int start_y = static_cast<int>(body.top) / tile_height;
	int end_y = static_cast<int>(body.bottom) / tile_height;

	start_x = std::clamp(start_x, 0, map_width - 1);
	end_x = std::clamp(end_x, 0, map_width - 1);
	start_y = std::clamp(start_y, 0, map_height - 1);
	end_y = std::clamp(end_y, 0, map_height - 1);

	const float prev_bottom = body.bottom - move_y;

	for (int tile_y = start_y; tile_y <= end_y; ++tile_y)
	{
		for (int tile_x = start_x; tile_x <= end_x; ++tile_x)
		{
			const int tile_id = tile_map->GetTileId(tile_x, tile_y);
			if (!tile_db->IsSolid(tile_id))
			{
				continue;
			}

			const float tile_left = static_cast<float>(tile_x * tile_width);
			const float tile_right = tile_left + static_cast<float>(tile_width);
			const float tile_top = static_cast<float>(tile_y * tile_height);
			const float tile_bottom = tile_top + static_cast<float>(tile_height);

			const bool overlap =
				(body.left < tile_right) && (body.right > tile_left) &&
				(body.top < tile_bottom) && (body.bottom > tile_top);

			if (!overlap)
			{
				continue;
			}

			if (move_y > 0.0f)
			{
				if (prev_bottom > tile_top)
				{
					continue;
				}

				const float body_height = body.bottom - body.top;
				enemy_info_.pos_y = tile_top - (body_height + kShrinkTop);

				velocity_y_ = 0.0f;
				is_on_ground_ = true;
				return true;
			}
			else
			{
				enemy_info_.pos_y = tile_bottom - kShrinkTop;
				velocity_y_ = 0.0f;
				return false;
			}
		}
	}

	return false;
}

/// <summary>
/// 今地面に接しているか？(probe_yだけ下にずらして判定をとる)
/// </summary>
bool BossEnemy::IsOnGroundByProbe(float probe_y)
{
	if (probe_y <= 0.0f)
	{
		return false;
	}

	if (game_level_ == nullptr)
	{
		return false;
	}

	TileMap* tile_map = game_level_->GetTileMap();
	TileDatabase* tile_db = game_level_->GetTileDatabase();
	TileSet* tile_set = game_level_->GetTileSet();

	if (tile_map == nullptr || tile_db == nullptr || tile_set == nullptr)
	{
		return false;
	}

	const int tile_width = tile_set->GetTileWidth();
	const int tile_height = tile_set->GetTileHeight();
	if (tile_width <= 0 || tile_height <= 0)
	{
		return false;
	}

	const int map_width = tile_map->GetWidth();
	const int map_height = tile_map->GetHeight();
	if (map_width <= 0 || map_height <= 0)
	{
		return false;
	}

	const CollisionBase::RectF body = MakeBodyRect();

	CollisionBase::RectF foot_probe{};
	foot_probe.left = body.left;
	foot_probe.right = body.right;
	foot_probe.top = body.bottom;
	foot_probe.bottom = body.bottom + probe_y;

	int start_x = static_cast<int>(foot_probe.left) / tile_width;
	int end_x = static_cast<int>(foot_probe.right) / tile_width;
	int start_y = static_cast<int>(foot_probe.top) / tile_height;
	int end_y = static_cast<int>(foot_probe.bottom + probe_y) / tile_height;

	start_x = std::clamp(start_x, 0, map_width - 1);
	end_x = std::clamp(end_x, 0, map_width - 1);
	start_y = std::clamp(start_y, 0, map_height - 1);
	end_y = std::clamp(end_y, 0, map_height - 1);

	for (int tile_y = start_y; tile_y <= end_y; ++tile_y)
	{
		for (int tile_x = start_x; tile_x <= end_x; ++tile_x)
		{
			const int tile_id = tile_map->GetTileId(tile_x, tile_y);
			if (!tile_db->IsSolid(tile_id))
			{
				continue;
			}

			const float tile_left = static_cast<float>(tile_x * tile_width);
			const float tile_right = tile_left + static_cast<float>(tile_width);
			const float tile_top = static_cast<float>(tile_y * tile_height);
			const float tile_bottom = tile_top + static_cast<float>(tile_height);

			const bool overlap =
				(foot_probe.left < tile_right) && (foot_probe.right > tile_left) &&
				(foot_probe.top < tile_bottom) && (foot_probe.bottom > tile_top);

			if (overlap)
			{
				return true;
			}
		}
	}

	return false;
}

/// <summary>
/// 移動状態のアニメーションの更新
/// </summary>
void BossEnemy::UpdateMoveAnimation()
{
	const EnemyAnimSet::AnimKind target_anim =
		is_moving_ ? EnemyAnimSet::AnimKind::kRun : EnemyAnimSet::AnimKind::kIdle;

	if (current_anim_ == target_anim)
	{
		return;
	}

	current_anim_ = target_anim;
	current_frame_ = 0;
	anim_time_ = 0.0f;
}

/// <summary>
/// 当たり判定四角形を作る
/// </summary>
CollisionBase::RectF BossEnemy::MakeBodyRect()
{
	const float body_width = static_cast<float>(enemy_info_.width) * enemy_info_.scale;
	const float body_height = static_cast<float>(enemy_info_.height) * enemy_info_.scale;

	CollisionBase::RectF rect{};
	rect.left = enemy_info_.pos_x + kShrinkX;
	rect.top = enemy_info_.pos_y + kShrinkTop;
	rect.right = enemy_info_.pos_x + body_width - kShrinkX;
	rect.bottom = enemy_info_.pos_y + body_height - kShrinkBottom;

	return rect;
}

/// <summary>
/// 壁との当たり判定処理
/// </summary>
bool BossEnemy::WallCollision(float move_x)
{
	if (move_x == 0.0f)
	{
		return false;
	}

	if (game_level_ == nullptr)
	{
		return false;
	}

	TileMap* tile_map = game_level_->GetTileMap();
	TileDatabase* tile_db = game_level_->GetTileDatabase();
	TileSet* tile_set = game_level_->GetTileSet();

	if (tile_map == nullptr || tile_db == nullptr || tile_set == nullptr)
	{
		return false;
	}

	const int tile_width = tile_set->GetTileWidth();
	const int tile_height = tile_set->GetTileHeight();
	if (tile_width <= 0 || tile_height <= 0)
	{
		return false;
	}

	const int map_width = tile_map->GetWidth();
	const int map_height = tile_map->GetHeight();
	if (map_width <= 0 || map_height <= 0)
	{
		return false;
	}

	CollisionBase::RectF body = MakeBodyRect();

	int start_x = static_cast<int>(body.left) / tile_width;
	int end_x = static_cast<int>(body.right) / tile_width;
	int start_y = static_cast<int>(body.top) / tile_height;
	int end_y = static_cast<int>(body.bottom) / tile_height;

	start_x = std::clamp(start_x, 0, map_width - 1);
	end_x = std::clamp(end_x, 0, map_width - 1);
	start_y = std::clamp(start_y, 0, map_height - 1);
	end_y = std::clamp(end_y, 0, map_height - 1);

	bool hit_any_wall = false;

	for (int tile_y = start_y; tile_y <= end_y; ++tile_y)
	{
		for (int tile_x = start_x; tile_x <= end_x; ++tile_x)
		{
			const int tile_id = tile_map->GetTileId(tile_x, tile_y);
			if (!tile_db->IsSolid(tile_id))
			{
				continue;
			}

			const float tile_left = static_cast<float>(tile_x * tile_width);
			const float tile_right = tile_left + static_cast<float>(tile_width);
			const float tile_top = static_cast<float>(tile_y * tile_height);
			const float tile_bottom = tile_top + static_cast<float>(tile_height);

			const bool overlap =
				(body.left < tile_right) && (body.right > tile_left) &&
				(body.top < tile_bottom) && (body.bottom > tile_top);

			if (!overlap)
			{
				continue;
			}

			hit_any_wall = true;

			//押し戻し
			if (move_x > 0.0f)
			{
				//右移動：タイルの左に押し戻す
				const float body_width = body.right - body.left;
				enemy_info_.pos_x = tile_left - (body_width + kShrinkX);
			}
			else
			{
				//左移動：タイルの右に押し戻す
				enemy_info_.pos_x = tile_right - kShrinkX;
			}

			//押し戻した後の四角形に更新
			body = MakeBodyRect();
		}
	}

	return hit_any_wall;
}