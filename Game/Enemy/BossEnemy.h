#pragma once
#include "EnemyBase.h"
#include "Level/GameLevel.h"
#include "Audio/AudioManager.h"
#include <random>

/// <summary>
/// ボスエネミーのクラス
/// </summary>
class BossEnemy :public EnemyBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	BossEnemy() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~BossEnemy() = default;

public:
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
	/// 攻撃開始処理
	/// </summary>
	void StartAttack() override;

	/// <summary>
	/// 遠距離攻撃準備処理
	/// </summary>
	void PreparationLongRangeAttack() override;

	/// <summary>
	/// 遠距離攻撃開始処理
	/// </summary>
	void StartLongRangeAttack() override;

	/// <summary>
	/// 攻撃処理
	/// </summary>
	void Attack() override;

	/// <summary>
	/// 攻撃当たり判定円を作成する
	/// </summary>
	bool TryGetAttackHitCircle(CollisionBase::CircleF& out_circle) override;

	/// <summary>
	/// 攻撃終了処理
	/// </summary>
	void FinishAttack() override;

	/// <summary>
	/// 死亡処理
	/// </summary>
	void DeadEnemy() override;

	/// <summary>
	/// 被ダメージ処理
	/// </summary>
	void OnHit() override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 画面外に敵が出たら出現済フラグをFalseにする
	/// </summary>
	void EndEnemy();

	/// <summary>
	/// 音声リソースを解放する
	/// EnemyManager::FinalizeEnemy() で delete する前に呼ぶ
	/// </summary>
	void FinalizeAudio();

	/// <summary>
	/// ボスの形態
	/// </summary>
	enum class BossForm
	{
		/// <summary>
		/// 通常
		/// </summary>
		kNormal,

		/// <summary>
		/// 強化
		/// </summary>
		kEnhanced
	};

	/// <summary>
	/// ボス状態をセット
	/// </summary>
	/// <param name="boss_form">ボスの状態</param>
	void SetBossForm(BossForm boss_form)
	{
		boss_form_ = boss_form;
	}

	/// <summary>
	/// 敵出現処理
	/// </summary>
	void AppearEnemy(float pos_x, float pos_y);

	/// <summary>
	/// 移動処理の更新
	/// </summary>
	/// <param name="delta_time">前回実行フレームからの経過時間（秒）</param>
	void UpdateMove(float delta_time);

	/// <summary>
	/// 次の行動の選択
	/// </summary>
	void ChooseNextAction();

	/// <summary>
	/// ジャンプの開始
	/// </summary>
	void StartJump();

	/// <summary>
	/// 防御開始処理
	/// </summary>
	void StartGuard();

	/// <summary>
	/// 防御終了処理
	/// </summary>
	void FinishGuard();

	/// <summary>
	/// 地面との当たり判定処理
	/// </summary>
	/// <param name="move_y">動く先のY座標</param>
	/// <returns>地面と接しているか</returns>
	bool GroundCollision(float move_y);

	/// <summary>
	/// 今地面に接しているか？(probe_yだけ下にずらして判定をとる)
	/// </summary>
	/// <param name="probe_y">判定を下にずらす量</param>
	/// <returns>今地面に接しているか</returns>
	bool IsOnGroundByProbe(float probe_y);

	/// <summary>
	/// 移動状態のアニメーションの更新
	/// </summary>
	void UpdateMoveAnimation();

	/// <summary>
	/// 当たり判定四角形を作る
	/// </summary>
	CollisionBase::RectF MakeBodyRect();

	/// <summary>
	/// 壁との当たり判定処理
	/// </summary>
	/// <param name="move_x">動く先のX座標</param>
	bool WallCollision(float move_x);

	/// <summary>
	/// ScoreManagerのセット
	/// </summary>
	/// <param name="score_manager">ScoreManagerのポインタ</param>
	void SetScoreManager(ScoreManager* score_manager)
	{
		score_manager_ = score_manager;
	}

	/// <summary>
	/// クナイマネージャーをセット
	/// </summary>
	/// <param name="kunai_manager">ポインタ</param>
	void SetKunaiManager(KunaiManager* kunai_manager)
	{
		kunai_manager_ = kunai_manager;
	}

	/// <summary>
	/// float型の乱数を取得する
	/// </summary>
	/// <param name="min_value">最低値</param>
	/// <param name="max_value">最大値</param>
	/// <returns>範囲内のfloat型の値</returns>
	float GetRandomFloat(float min_value, float max_value)
	{
		std::uniform_real_distribution<float> dist(min_value, max_value);
		return dist(random_engine_);
	}

	/// <summary>
	/// Int型の乱数を取得する
	/// </summary>
	/// <param name="min_value">最低値</param>
	/// <param name="max_value">最大値</param>
	/// <returns>範囲内のInt型の値</returns>
	int GetRandomInt(int min_value, int max_value)
	{
		std::uniform_int_distribution<int> dist(min_value, max_value);
		return dist(random_engine_);
	}

	/// <summary>
	/// ボスの能力値
	/// </summary>
	struct BossParameter
	{
		/// <summary>
		/// 最大HP
		/// </summary>
		int max_hp = 0;

		/// <summary>
		/// 最短攻撃間隔
		/// </summary>
		float min_attack_interval = 0.0f;

		/// <summary>
		/// 最長攻撃間隔
		/// </summary>
		float max_attack_interval = 0.0f;

		/// <summary>
		/// 遠距離攻撃をする確率
		/// </summary>
		float long_range_attack_chance = 0.0f;

		/// <summary>
		/// 近距離攻撃をする距離
		/// </summary>
		float attack_range = 0.0f;

		/// <summary>
		/// ジャンプする確率
		/// </summary>
		float jump_chance = 0.0f;

		/// <summary>
		/// ジャンプ速度
		/// </summary>
		float jump_speed = 0.0f;

		/// <summary>
		/// 移動速度
		/// </summary>
		float move_speed = 0.0f;

		/// <summary>
		/// 移動する確率
		/// </summary>
		float move_chance = 0.0f;

		/// <summary>
		/// ガードする確率
		/// </summary>
		float guard_chance = 0.0f;

		/// <summary>
		/// ガードのクールタイム
		/// </summary>
		float guard_cooldown_seconds = 0.0f;
		
		/// <summary>
		/// 接触ダメージ
		/// </summary>
		int damage = 0;
	};
private:
	/// <summary>
	/// 生きているか？
	/// </summary>
	bool is_alive_ = false;

	/// <summary>
	/// 被ダメージ時のフラッシュさせる時間
	/// </summary>
	float damage_flash_seconds_ = 0.0f;

	/// <summary>
	/// 乱数エンジン
	/// </summary>
	std::mt19937 random_engine_;

	/// <summary>
	/// 移動するときの中心座標
	/// </summary>
	float move_center_x_ = 0.0f;

	/// <summary>
	/// 目的地のX座標
	/// </summary>
	float move_target_x_ = 0.0f;

	/// <summary>
	/// 一回の移動時間
	/// </summary>
	float move_state_timer_ = 0.0f;

	/// <summary>
	/// 今は移動中か？
	/// </summary>
	bool is_moving_ = false;

	/// <summary>
	/// 移動に関する初期化をしたか？
	/// </summary>
	bool is_moving_initialized_ = false;

	/// <summary>
	/// ガード中か？
	/// </summary>
	bool is_guarding_ = false;

	/// <summary>
	/// 次のガード抽選が可能になるまでのクールタイム
	/// </summary>
	float guard_cooldown_seconds_ = 0.0f;

	/// <summary>
	/// エフェクトの描画位置X
	/// </summary>
	float effect_draw_position_x_ = 0.0f;

	/// <summary>
	/// エフェクトの描画位置Y
	/// </summary>
	float effect_draw_position_y_ = 0.0f;

	/// <summary>
	/// 画像ハンドル
	/// </summary>
	int image_handle_ = 0;

	/// <summary>
	/// カメラX座標
	/// </summary>
	float camera_x_ = 0.0f;

	/// <summary>
	/// カメラY座標
	/// </summary>
	float camera_y_ = 0.0f;

	/// <summary>
	/// 中心X座標
	/// </summary>
	float center_x_ = 0.0f;

	/// <summary>
	/// 中心Y座標
	/// </summary>
	float center_y_ = 0.0f;

	/// <summary>
	/// 元画像の中心X座標
	/// </summary>
	float img_center_x_ = 0.0f;

	/// <summary>
	/// 元画像の中心Y座標
	/// </summary>
	float img_center_y_ = 0.0f;

	/// <summary>
	/// ボスエネミーのHP
	/// </summary>
	int enemy_hp_ = 0;

	/// <summary>
	/// ボスの能力値
	/// </summary>
	BossParameter boss_parameter_;

	/// <summary>
	/// ボスの形態 初期値は通常
	/// </summary>
	BossForm boss_form_ = BossForm::kNormal;

	/// <summary>
	/// 今左を向いているか？
	/// </summary>
	bool facing_left_ = true;

	/// <summary>
	/// 攻撃開始時の向き
	/// </summary>
	bool attack_facing_left_ = false;

	/// <summary>
	/// クナイマネージャーのポインタ
	/// </summary>
	KunaiManager* kunai_manager_ = nullptr;

	/// <summary>
	/// 攻撃ヒットSEのハンドル
	/// </summary>
	int attack_hit_se_handle_ = -1;

	/// <summary>
	/// ガードSEのハンドル
	/// </summary>
	int guard_se_handle_ = -1;

	/// <summary>
	/// 撃破SEのハンドル
	/// </summary>
	int enemy_defeat_se_handle_ = -1;

};