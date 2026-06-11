#pragma once
#include "EnemyAnimSet.h"
#include "Collision/CollisionBase.h"

/// <summary>
/// 前方宣言
/// </summary>
class EffectManager;
class ScoreManager;
class GameLevel;
class Player;

/// <summary>
/// 敵の基本クラス
/// </summary>
class EnemyBase
{
public:
	/// <summary>
	/// 	コンストラクタ
	/// </summary>
	EnemyBase();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~EnemyBase() = default;

	/// <summary>
	/// 敵一体に含める情報
	/// </summary>
	struct EnemyInfo
	{
		/// <summary>
		/// 出現したか
		/// </summary>
		bool is_appeared = false;

		/// <summary>
		/// 左向きか?
		/// </summary>
		bool facing_left = true;

		/// <summary>
		/// X座標
		/// </summary>
		float pos_x = 0.0f;

		/// <summary>
		/// Y座標
		/// </summary>
		float pos_y = 0.0f;

		/// <summary>
		/// 画像の幅
		/// </summary>
		int width = 0;

		/// <summary>
		/// 画像の高さ
		/// </summary>
		int height = 0;

		/// <summary>
		/// 拡大率
		/// </summary>
		float scale = 1.0f;
	};

public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 毎フレームの更新処理
	/// </summary>
	/// <param name="delta_time">前回実行フレームからの経過時間（秒）</param>
	virtual void Update(float delta_time) = 0;

	/// <summary>
	/// 毎フレームの描画処理
	/// </summary>
	virtual void Render() = 0;

	/// <summary>
	/// 敵出現処理
	/// </summary>
	virtual void AppearEnemy(float pos_x, float pos_y) = 0;

	/// <summary>
	/// 音声リソースを解放する
	/// </summary>
	virtual void FinalizeAudio() = 0;

	/// <summary>
	/// クナイを発射する処理
	/// </summary>
	/// <param name="kunai_type">クナイの種類</param>
	/// <param name="pos_x">クナイのX座標</param>
	/// <param name="pos_y">クナイのY座標</param>
	virtual void Fire(float pos_x, float pos_y);

	/// <summary>
	/// 攻撃開始処理
	/// </summary>
	virtual void StartAttack();

	/// <summary>
	/// 遠距離攻撃準備処理
	/// </summary>
	virtual void PreparationLongRangeAttack();

	/// <summary>
	/// 遠距離攻撃開始処理
	/// </summary>
	virtual void StartLongRangeAttack();

	/// <summary>
	/// 攻撃処理
	/// </summary>
	virtual void Attack();

	/// <summary>
	/// 攻撃当たり判定円を作成する
	/// </summary>
	/// <param name="out_circle">当たり判定円のデータ格納先</param>
	/// <returns>成功・失敗</returns>
	virtual bool TryGetAttackHitCircle(CollisionBase::CircleF& out_circle);
	
	/// <summary>
	/// 攻撃終了処理
	/// </summary>
	virtual void FinishAttack();

	/// <summary>
	/// 死亡処理
	/// </summary>
	virtual void DeadEnemy();

	/// <summary>
	/// 被ダメージ処理
	/// </summary>
	virtual void OnHit();

	/// <summary>
	/// 敵本体の当たり判定円を作成する
	/// </summary>
	/// <returns>敵本体の当たり判定円</returns>
	CollisionBase::CircleF GetHitCircle();

	/// <summary>
	/// 足元タイルから地面判定を取得する
	/// </summary>
	void UpdateGroundYFromTiles();

	/// <summary>
	/// 座標をセットする
	/// </summary>
	/// <param name="x">X座標</param>
	/// <param name="y">Y座標</param>
	void SetPosition(float x, float y);

	/// <summary>
	/// 周囲を移動する処理
	/// </summary>
	/// <param name="range">移動範囲</param>
	/// <param name="speed">移動速度</param>
	/// <param name="delta_time">前回実行フレームからの経過時間（秒）</param>
	void Patrol(float range, float speed, float delta_time);

	/// <summary>
	/// 画面外に敵が出たら出現済フラグをFalseにする
	/// </summary>
	void EndEnemy();

	/// <summary>
	/// EnemyInfoのゲッタ
	/// </summary>
	/// <returns>EnemyInfo</returns>
	EnemyInfo& GetInfo()
	{
		return enemy_info_;
	}

	/// <summary>
	/// EnemyInfoのゲッタ（const版）
	/// </summary>
	/// <returns>EnemyInfo</returns>
	const EnemyInfo& GetInfo() const
	{
		return enemy_info_;
	}

	/// <summary>
	/// エフェクトマネージャーのポインタをセット
	/// </summary>
	/// <param name="mgr">ポインタ</param>
	void SetEffectManager(EffectManager* mgr)
	{
		effect_manager_ = mgr;
	}

	/// <summary>
	/// ScoreManagerのセット
	/// </summary>
	/// <param name="score_manager">ScoreManagerのポインタ</param>
	void SetScoreManager(ScoreManager* score_manager)
	{
		score_manager_ = score_manager;
	}

	/// <summary>
	/// EnemyAnimSetのセット
	/// </summary>
	/// <param name="anim_set">EnemyAnimSetのポインタ</param>
	void SetAnimSet(const EnemyAnimSet* anim_set)
	{
		anim_set_ = anim_set;
	}

	/// <summary>
	/// GameLevelのセット
	/// </summary>
	/// <param name="game_level">GameLevelのポインタ</param>
	void SetGameLevel(GameLevel* game_level)
	{
		game_level_ = game_level;
	}

	/// <summary>
	/// Playerのセット
	/// </summary>
	/// <param name="player">Playerのポインタ</param>
	void SetPlayer(Player* player)
	{
		player_ = player;
	}

protected:
	/// <summary>
	/// 敵の初期アニメーションは「アイドル状態」
	/// </summary>
	EnemyAnimSet::AnimKind current_anim_ = EnemyAnimSet::AnimKind::kIdle;

	/// <summary>
	/// 当たり判定円のX座標のオフセット
	/// </summary>
	float hit_offset_x_ = 0.0f;

	/// <summary>
	/// 当たり判定円のY座標のオフセット
	/// </summary>
	float hit_offset_y_ = 0.0f;
	
	/// <summary>
	/// 当たり判定円の半径
	/// </summary>
	float hit_radius_ = 0.0f;

	/// <summary>
	/// 地面の高さ
	/// </summary>
	float ground_y_ = 0.0f;
	
	/// <summary>
	/// 上方向へ働くジャンプ力
	/// </summary>
	float velocity_y_ = 0.0f;
	
	/// <summary>
	/// 地上にいるか？
	/// </summary>
	bool is_on_ground_ = false;

	/// <summary>
	/// 攻撃中か？
	/// </summary>
	bool is_attacking_ = false;

	/// <summary>
	/// 遠距離攻撃中か？
	/// </summary>
	bool is_long_range_attacking_ = false;

	/// <summary>
	/// 攻撃のクールタイム
	/// </summary>
	float attack_cooldown_seconds_ = 0.0f;

	/// <summary>
	/// 攻撃用当たり判定がONか？
	/// </summary>
	bool is_attack_hit_enabled_ = false;

	/// <summary>
	/// 巡回の目的地左端X座標
	/// </summary>
	float patrol_left_x_ = 0.0f;
	
	/// <summary>
	/// 巡回の目的地右端X座標
	/// </summary>
	float patrol_right_x_ = 0.0f;
	
	/// <summary>
	/// 巡回の初期化をしたか？
	/// </summary>
	bool is_patrol_initialized_ = false;

	/// <summary>
	/// 現在のフレーム
	/// </summary>
	int current_frame_ = 0;
    
	/// <summary>
	/// アニメーションの経過時間
	/// </summary>
	float anim_time_ = 0.0f;

    /// <summary>
    /// 参照する共有アニメセット
    /// </summary>
    const EnemyAnimSet* anim_set_ = nullptr;

	/// <summary>
	/// EnemyInfoのインスタンス
	/// </summary>
	EnemyInfo enemy_info_;

	/// <summary>
	/// EffectManagerポインタ
	/// </summary>
	EffectManager* effect_manager_ = nullptr;

	/// <summary>
	/// ScoreManagerのポインタ
	/// </summary>
	ScoreManager* score_manager_ = nullptr;

	/// <summary>
	/// GameLevelのポインタ
	/// </summary>
	GameLevel* game_level_ = nullptr;

	/// <summary>
	/// Playerのポインタ
	/// </summary>
	Player* player_ = nullptr;
};