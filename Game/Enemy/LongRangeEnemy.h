#pragma once
#include "EnemyBase.h"
#include "Level/GameLevel.h"
#include "Audio/AudioManager.h"


/// <summary>
/// 遠距離攻撃エネミーのクラス
/// </summary>
class LongRangeEnemy :public EnemyBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	LongRangeEnemy() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~LongRangeEnemy() = default;

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
	/// 敵出現処理
	/// </summary>
	void AppearEnemy(float pos_x, float pos_y);

	/// <summary>
	/// 音声リソースを解放する
	/// EnemyManager::FinalizeEnemy() で delete する前に呼ぶ
	/// </summary>
	void FinalizeAudio();

	/// <summary>
	/// ScoreManagerのセット
	/// </summary>
	/// <param name="score_manager">ScoreManagerのポインタ</param>
	void SetScoreManager(ScoreManager* score_manager)
	{
		score_manager_ = score_manager;
	}

private:
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
	/// 遠距離攻撃エネミーのHP
	/// </summary>
	int enemy_hp_ = 0;

	/// <summary>
	/// 攻撃ヒットSEのハンドル
	/// </summary>
	int attack_hit_se_handle_ = -1;

	/// <summary>
	/// 敵撃破SEのハンドル
	/// </summary>
	int enemy_defeat_se_handle_ = -1;
};