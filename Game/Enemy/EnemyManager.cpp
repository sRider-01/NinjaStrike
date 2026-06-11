#define NOMINMAX //std::minを使うためにWindowsヘッダのマクロ定義をしないようにさせる
#include "DxLib.h"
#include "Player/Player.h"
#include "EnemyManager.h"
#include "EnemyBase.h"
#include "PatrolEnemy.h"
#include "LongRangeEnemy.h"
#include "BossEnemy.h"
#include "GameInfo.h"
#include "Kunai/KunaiManager.h"
#include "Kunai/PlayerKunai.h"
#include "EnemySpawnMap/EnemySpawnMap.h"
#include "TileMap/TileSet.h"
#include "Level/GameLevel.h"
#include "Effect/EffectManager.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>


/// <summary>
/// EnemyManager.cppで使う定数
/// </summary>
namespace
{
	/// <summary>
	/// ID:何も配置しない
	/// </summary>
	const int kEnemySpawnNone = 0;

	/// <summary>
	/// ID：パトロールエネミー
	/// </summary>
	const int kEnemySpawnPatrol = 1;

	/// <summary>
	/// ID:遠距離攻撃エネミー
	/// </summary>
	const int kEnemySpawnRanged = 2;

	/// <summary>
	/// ID:ボスエネミー
	/// </summary>
	const int kEnemySpawnBoss = 3;

	/// <summary>
	/// 接触ダメージ
	/// </summary>
	const int kContactDamage = 1;

	/// <summary>
	/// 攻撃ダメージ
	/// </summary>
	const int kAttackDamage = 3;

	/// <summary>
	/// ゲーム画面から敵がはみ出ないようにする定数
	/// </summary>
	const int kAdjustEdge = 50;

	/// <summary>
	/// 未ロードであることを示す
	/// </summary>
	const int kUnLoaded = -1;

	/// <summary>
	/// パトロールエネミーの出現数
	/// </summary>
	const int kMaxPatrolEnemies = 5;

	/// <summary>
	/// 遠距離攻撃エネミーの出現数
	/// </summary>
	const int kMaxAttackEnemies = 3;
	
	/// <summary>
	/// ボスエネミーの出現数
	/// </summary>
	const int kMaxBossEnemies = 1;

	/// <summary>
	/// すべての敵の数
	/// </summary>
	const int kAllEnemies = kMaxPatrolEnemies + kMaxAttackEnemies + kMaxBossEnemies;

	/// <summary>
	/// パトロールエネミーのアイドル状態の画像パス
	/// </summary>
	const char* kPatrolEnemyIdleImagePath = "Assets/Image/Enemy/PatrolEnemy/Idle.png";

	/// <summary>
	/// 敵の描画倍率
	/// </summary>
	const float kPatrolEnemyScale = 2.0f;

	/// <summary>
	/// アイドル状態画像を横に何等分するか
	/// </summary>
	const int kIdleDivX = 4;

	/// <summary>
	/// アイドル状態画像を縦に何等分するか
	/// </summary>
	const int kIdleDivY = 1;

	/// <summary>
	/// アイドル状態のフレームの数
	/// </summary>
	const int kIdleFrameCount = kIdleDivX * kIdleDivY;

	/// <summary>
	/// アイドル状態の1フレームの幅
	/// </summary>
	const int kIdleFrameWidth = 200;

	/// <summary>
	/// アイドル状態の1フレームの高さ
	/// </summary>
	const int kIdleFrameHeight = 59;

	/// <summary>
	/// アイドル状態の一コマに使用する秒数
	/// </summary>
	const float kAnimIdleInterval = 0.12f;

	/// <summary>
	/// パトロールエネミーの移動状態の画像パス
	/// </summary>
	const char* kPatrolEnemyRunImagePath = "Assets/Image/Enemy/PatrolEnemy/Run.png";

	/// <summary>
	/// 移動状態画像を横に何等分するか
	/// </summary>
	const int kRunDivX = 8;

	/// <summary>
	/// 移動状態画像を縦に何等分するか
	/// </summary>
	const int kRunDivY = 1;

	/// <summary>
	/// 移動状態のフレームの数
	/// </summary>
	const int kRunFrameCount = kRunDivX * kRunDivY;

	/// <summary>
	/// 移動状態の1フレームの幅
	/// </summary>
	const int kRunFrameWidth = 200;

	/// <summary>
	/// 移動状態の1フレームの高さ
	/// </summary>
	const int kRunFrameHeight = 49;

	/// <summary>
	/// 移動状態の一コマに使用する秒数
	/// </summary>
	const float kAnimRunInterval = 0.08f;

	/// <summary>
	/// パトロールエネミーの攻撃状態の画像パス
	/// </summary>
	const char* kPatrolEnemyAttackImagePath = "Assets/Image/Enemy/PatrolEnemy/Attack2.png";

	/// <summary>
	/// 攻撃状態画像を横に何等分するか
	/// </summary>
	const int kAttackDivX = 4;

	/// <summary>
	/// 攻撃状態画像を縦に何等分するか
	/// </summary>
	const int kAttackDivY = 1;

	/// <summary>
	/// 攻撃状態のフレームの数
	/// </summary>
	const int kAttackFrameCount = kAttackDivX * kAttackDivY;

	/// <summary>
	/// 攻撃状態の1フレームの幅
	/// </summary>
	const int kAttackFrameWidth = 200;

	/// <summary>
	/// 攻撃状態の1フレームの高さ
	/// </summary>
	const int kAttackFrameHeight = 92;

	/// <summary>
	/// 攻撃状態の一コマに使用する秒数
	/// </summary>
	const float kAnimAttackInterval = 0.10f;

	/// <summary>
	/// パトロールエネミーのアニメーションの配列
	/// </summary>
	const EnemyAnimSet::AnimDef kPatrolAnimDefs[] =
	{
		{ EnemyAnimSet::AnimKind::kIdle, kPatrolEnemyIdleImagePath, kIdleFrameCount, kIdleDivX, kIdleDivY, kIdleFrameWidth, kIdleFrameHeight, kAnimIdleInterval },
		{ EnemyAnimSet::AnimKind::kRun, kPatrolEnemyRunImagePath, kRunFrameCount, kRunDivX, kRunDivY, kRunFrameWidth, kRunFrameHeight, kAnimRunInterval },
		{ EnemyAnimSet::AnimKind::kAttack, kPatrolEnemyAttackImagePath, kAttackFrameCount, kAttackDivX, kAttackDivY, kAttackFrameWidth, kAttackFrameHeight, kAnimAttackInterval },
	};

	/// <summary>
	/// パトロールエネミーのアニメーション配列のサイズ
	/// </summary>
	const int kPatrolAnimDefCount = static_cast<int>(sizeof(kPatrolAnimDefs) / sizeof(kPatrolAnimDefs[0]));


	/// <summary>
	/// 遠距離攻撃エネミーのアイドル状態の画像パス
	/// </summary>
	const char* kLongRangeEnemyIdleImagePath = "Assets/Image/Enemy/LongRangeEnemy/yellowNinja - idle.png";

	/// <summary>
	/// 遠距離攻撃エネミーの描画倍率
	/// </summary>
	const float kLongRangeEnemyScale = 3.0f;

	/// <summary>
	/// アイドル状態画像を横に何等分するか
	/// </summary>
	const int kLongRangeIdleDivX = 8;

	/// <summary>
	/// アイドル状態画像を縦に何等分するか
	/// </summary>
	const int kLongRangeIdleDivY = 1;

	/// <summary>
	/// アイドル状態のフレームの数
	/// </summary>
	const int kLongRangeIdleFrameCount = kLongRangeIdleDivX * kLongRangeIdleDivY;

	/// <summary>
	/// アイドル状態の1フレームの幅
	/// </summary>
	const int kLongRangeIdleFrameWidth = 128;

	/// <summary>
	/// アイドル状態の1フレームの高さ
	/// </summary>
	const int kLongRangeIdleFrameHeight = 37;

	/// <summary>
	/// アイドル状態の一コマに使用する秒数
	/// </summary>
	const float kAnimLongRangeIdleInterval = 0.12f;

	/// <summary>
	/// 遠距離攻撃エネミーのダメージ状態の画像パス
	/// </summary>
	const char* kLongRangeEnemyDamageImagePath = "Assets/Image/Enemy/LongRangeEnemy/yellowNinja - Whitehit.png";

	/// <summary>
	/// ダメージ状態画像を横に何等分するか
	/// </summary>
	const int kLongRangeDamageDivX = 4;

	/// <summary>
	/// ダメージ状態画像を縦に何等分するか
	/// </summary>
	const int kLongRangeDamageDivY = 1;

	/// <summary>
	/// ダメージ状態のフレームの数
	/// </summary>
	const int kLongRangeDamageFrameCount = kLongRangeDamageDivX * kLongRangeDamageDivY;

	/// <summary>
	/// ダメージ状態の1フレームの幅
	/// </summary>
	const int kLongRangeDamageFrameWidth = 128;

	/// <summary>
	/// ダメージ状態の1フレームの高さ
	/// </summary>
	const int kLongRangeDamageFrameHeight = 37;

	/// <summary>
	/// ダメージ状態の一コマに使用する秒数
	/// </summary>
	const float kAnimLongRangeDamageInterval = 0.12f;

	/// <summary>
	/// 遠距離攻撃エネミーの攻撃状態の画像パス
	/// </summary>
	const char* kLongRangeEnemyAttackImagePath = "Assets/Image/Enemy/LongRangeEnemy/yellowNinja - attack.png";

	/// <summary>
	/// 攻撃状態画像を横に何等分するか
	/// </summary>
	const int kLongRangeAttackDivX = 20;

	/// <summary>
	/// 攻撃状態画像を縦に何等分するか
	/// </summary>
	const int kLongRangeAttackDivY = 1;

	/// <summary>
	/// 攻撃状態のフレームの数
	/// </summary>
	const int kLongRangeAttackFrameCount = kLongRangeAttackDivX * kLongRangeAttackDivY;

	/// <summary>
	/// 攻撃状態の1フレームの幅
	/// </summary>
	const int kLongRangeAttackFrameWidth = 128;

	/// <summary>
	/// 攻撃状態の1フレームの高さ
	/// </summary>
	const int kLongRangeAttackFrameHeight = 37;

	/// <summary>
	/// 攻撃状態の一コマに使用する秒数
	/// </summary>
	const float kAnimLongRangeAttackInterval = 0.12f;

	/// <summary>
	/// 遠距離攻撃エネミーのアニメーションの配列
	/// </summary>
	const EnemyAnimSet::AnimDef kLongRangeAnimDefs[] =
	{
		{ EnemyAnimSet::AnimKind::kIdle, kLongRangeEnemyIdleImagePath, kLongRangeIdleFrameCount, kLongRangeIdleDivX, kLongRangeIdleDivY, kLongRangeIdleFrameWidth, kLongRangeIdleFrameHeight, kAnimLongRangeIdleInterval },
		{ EnemyAnimSet::AnimKind::kDamage, kLongRangeEnemyDamageImagePath, kLongRangeDamageFrameCount, kLongRangeDamageDivX, kLongRangeDamageDivY, kLongRangeDamageFrameWidth, kLongRangeDamageFrameHeight, kAnimLongRangeDamageInterval },
		{ EnemyAnimSet::AnimKind::kAttack, kLongRangeEnemyAttackImagePath, kLongRangeAttackFrameCount, kLongRangeAttackDivX, kLongRangeAttackDivY, kLongRangeAttackFrameWidth, kLongRangeAttackFrameHeight, kAnimLongRangeAttackInterval },
	};

	/// <summary>
	/// 遠距離攻撃エネミーのアニメーション配列のサイズ
	/// </summary>
	const int kLongRangeAnimDefCount = static_cast<int>(sizeof(kLongRangeAnimDefs) / sizeof(kLongRangeAnimDefs[0]));


	/// <summary>
	/// ボスエネミーのアイドル状態の画像パス
	/// </summary>
	const char* kBossEnemyIdleImagePath = "Assets/Image/Enemy/BossEnemy/IDLE.png";

	/// <summary>
	/// ボスエネミーの描画倍率
	/// </summary>
	const float kBossEnemyScale = 3.0f;

	/// <summary>
	/// アイドル状態画像を横に何等分するか
	/// </summary>
	const int kBossIdleDivX = 10;

	/// <summary>
	/// アイドル状態画像を縦に何等分するか
	/// </summary>
	const int kBossIdleDivY = 1;

	/// <summary>
	/// アイドル状態のフレームの数
	/// </summary>
	const int kBossIdleFrameCount = kBossIdleDivX * kBossIdleDivY;

	/// <summary>
	/// アイドル状態の1フレームの幅
	/// </summary>
	const int kBossIdleFrameWidth = 96;

	/// <summary>
	/// アイドル状態の1フレームの高さ
	/// </summary>
	const int kBossIdleFrameHeight = 34;

	/// <summary>
	/// アイドル状態の一コマに使用する秒数
	/// </summary>
	const float kAnimBossIdleInterval = 0.12f;

	/// <summary>
	/// ボスエネミーの攻撃状態の画像パス
	/// </summary>
	const char* kBossEnemyAttackImagePath = "Assets/Image/Enemy/BossEnemy/ATTACK 1.png";

	/// <summary>
	/// 攻撃状態画像を横に何等分するか
	/// </summary>
	const int kBossAttackDivX = 7;

	/// <summary>
	/// 攻撃状態画像を縦に何等分するか
	/// </summary>
	const int kBossAttackDivY = 1;

	/// <summary>
	/// 攻撃状態のフレームの数
	/// </summary>
	const int kBossAttackFrameCount = kBossAttackDivX * kBossAttackDivY;

	/// <summary>
	/// 攻撃状態の1フレームの幅
	/// </summary>
	const int kBossAttackFrameWidth = 96;

	/// <summary>
	/// 攻撃状態の1フレームの高さ
	/// </summary>
	const int kBossAttackFrameHeight = 36;

	/// <summary>
	/// 攻撃状態の一コマに使用する秒数
	/// </summary>
	const float kAnimBossAttackInterval = 0.10f;

	/// <summary>
	/// ボスエネミーの遠距離攻撃状態の画像パス
	/// </summary>
	const char* kBossEnemyLongRangeAttackImagePath = "Assets/Image/Enemy/BossEnemy/THROW.png";

	/// <summary>
	/// 遠距離攻撃状態画像を横に何等分するか
	/// </summary>
	const int kBossLongRangeAttackDivX = 7;

	/// <summary>
	/// 遠距離攻撃状態画像を縦に何等分するか
	/// </summary>
	const int kBossLongRangeAttackDivY = 1;

	/// <summary>
	/// 遠距離攻撃状態のフレームの数
	/// </summary>
	const int kBossLongRangeAttackFrameCount = kBossLongRangeAttackDivX * kBossLongRangeAttackDivY;

	/// <summary>
	/// 遠距離攻撃状態の1フレームの幅
	/// </summary>
	const int kBossLongRangeAttackFrameWidth = 96;

	/// <summary>
	/// 遠距離攻撃状態の1フレームの高さ
	/// </summary>
	const int kBossLongRangeAttackFrameHeight = 38;

	/// <summary>
	/// 遠距離攻撃状態の一コマに使用する秒数
	/// </summary>
	const float kAnimBossLongRangeAttackInterval = 0.10f;

	/// <summary>
	/// ボスエネミーのダメージ状態の画像パス
	/// </summary>
	const char* kBossEnemyDamageImagePath = "Assets/Image/Enemy/BossEnemy/HURT.png";

	/// <summary>
	/// ダメージ状態画像を横に何等分するか
	/// </summary>
	const int kBossDamageDivX = 4;

	/// <summary>
	/// ダメージ状態画像を縦に何等分するか
	/// </summary>
	const int kBossDamageDivY = 1;

	/// <summary>
	/// ダメージ状態のフレームの数(先頭フレームのみ使用)
	/// </summary>
	const int kBossDamageFrameCount = 1;

	/// <summary>
	/// ダメージ状態の1フレームの幅
	/// </summary>
	const int kBossDamageFrameWidth = 96;

	/// <summary>
	/// ダメージ状態の1フレームの高さ
	/// </summary>
	const int kBossDamageFrameHeight = 38;

	/// <summary>
	/// ダメージ状態の一コマに使用する秒数
	/// </summary>
	const float kAnimBossDamageInterval = 0.10f;

	/// <summary>
	/// ボスエネミーの移動状態の画像パス
	/// </summary>
	const char* kBossEnemyRunImagePath = "Assets/Image/Enemy/BossEnemy/RUN.png";

	/// <summary>
	/// 移動状態画像を横に何等分するか
	/// </summary>
	const int kBossRunDivX = 16;

	/// <summary>
	/// 移動状態画像を縦に何等分するか
	/// </summary>
	const int kBossRunDivY = 1;

	/// <summary>
	/// 移動状態のフレームの数
	/// </summary>
	const int kBossRunFrameCount = kBossRunDivX * kBossRunDivY;

	/// <summary>
	/// 移動状態の1フレームの幅
	/// </summary>
	const int kBossRunFrameWidth = 96;

	/// <summary>
	/// 移動状態の1フレームの高さ
	/// </summary>
	const int kBossRunFrameHeight = 35;

	/// <summary>
	/// 移動状態の一コマに使用する秒数
	/// </summary>
	const float kAnimBossRunInterval = 0.07f;

	/// <summary>
	/// ボスエネミーのジャンプ状態の画像パス
	/// </summary>
	const char* kBossEnemyJumpImagePath = "Assets/Image/Enemy/BossEnemy/JUMP.png";

	/// <summary>
	/// ジャンプ状態画像を横に何等分するか
	/// </summary>
	const int kBossJumpDivX = 3;

	/// <summary>
	/// ジャンプ状態画像を縦に何等分するか
	/// </summary>
	const int kBossJumpDivY = 1;

	/// <summary>
	/// ジャンプ状態のフレームの数
	/// </summary>
	const int kBossJumpFrameCount = kBossJumpDivX * kBossJumpDivY;

	/// <summary>
	/// ジャンプ状態の1フレームの幅
	/// </summary>
	const int kBossJumpFrameWidth = 96;

	/// <summary>
	/// ジャンプ状態の1フレームの高さ
	/// </summary>
	const int kBossJumpFrameHeight = 35;

	/// <summary>
	/// ジャンプ状態の一コマに使用する秒数
	/// </summary>
	const float kAnimBossJumpInterval = 0.10f;

	/// <summary>
	/// ボスエネミーのガード状態の画像パス
	/// </summary>
	const char* kBossEnemyGuardImagePath = "Assets/Image/Enemy/BossEnemy/DEFEND.png";

	/// <summary>
	/// ガード状態画像を横に何等分するか
	/// </summary>
	const int kBossGuardDivX = 6;

	/// <summary>
	/// ガード状態画像を縦に何等分するか
	/// </summary>
	const int kBossGuardDivY = 1;

	/// <summary>
	/// ガード状態のフレームの数
	/// </summary>
	const int kBossGuardFrameCount = kBossGuardDivX * kBossGuardDivY;

	/// <summary>
	/// ガード状態の1フレームの幅
	/// </summary>
	const int kBossGuardFrameWidth = 96;

	/// <summary>
	/// ガード状態の1フレームの高さ
	/// </summary>
	const int kBossGuardFrameHeight = 34;

	/// <summary>
	/// ガード状態の一コマに使用する秒数
	/// </summary>
	const float kAnimBossGuardInterval = 0.10f;

	/// <summary>
	/// ボスエネミーのやられ状態の画像パス
	/// </summary>
	const char* kBossEnemyDeadImagePath = "Assets/Image/Enemy/BossEnemy/DEATH.png";

	/// <summary>
	/// やられ状態画像を横に何等分するか
	/// </summary>
	const int kBossDeadDivX = 9;

	/// <summary>
	/// やられ状態画像を縦に何等分するか
	/// </summary>
	const int kBossDeadDivY = 1;

	/// <summary>
	/// やられ状態のフレームの数
	/// </summary>
	const int kBossDeadFrameCount = kBossDeadDivX * kBossDeadDivY;

	/// <summary>
	/// やられ状態の1フレームの幅
	/// </summary>
	const int kBossDeadFrameWidth = 96;

	/// <summary>
	/// やられ状態の1フレームの高さ
	/// </summary>
	const int kBossDeadFrameHeight = 32;

	/// <summary>
	/// やられ状態の一コマに使用する秒数
	/// </summary>
	const float kAnimBossDeadInterval = 0.10f;

	/// <summary>
	/// ボスエネミーのアニメーションの配列
	/// </summary>
	const EnemyAnimSet::AnimDef kBossAnimDefs[] =
	{
		{ EnemyAnimSet::AnimKind::kIdle, kBossEnemyIdleImagePath, kBossIdleFrameCount, kBossIdleDivX, kBossIdleDivY, kBossIdleFrameWidth, kBossIdleFrameHeight, kAnimBossIdleInterval },
		{ EnemyAnimSet::AnimKind::kRun, kBossEnemyRunImagePath, kBossRunFrameCount, kBossRunDivX, kBossRunDivY, kBossRunFrameWidth, kBossRunFrameHeight, kAnimBossRunInterval },
		{ EnemyAnimSet::AnimKind::kJump, kBossEnemyJumpImagePath, kBossJumpFrameCount, kBossJumpDivX, kBossJumpDivY, kBossJumpFrameWidth, kBossJumpFrameHeight, kAnimBossJumpInterval },
		{ EnemyAnimSet::AnimKind::kGuard, kBossEnemyGuardImagePath, kBossGuardFrameCount, kBossGuardDivX, kBossGuardDivY, kBossGuardFrameWidth, kBossGuardFrameHeight, kAnimBossGuardInterval },
		{ EnemyAnimSet::AnimKind::kAttack, kBossEnemyAttackImagePath, kBossAttackFrameCount, kBossAttackDivX, kBossAttackDivY, kBossAttackFrameWidth, kBossAttackFrameHeight, kAnimBossAttackInterval },
		{ EnemyAnimSet::AnimKind::kLongRangeAttack, kBossEnemyLongRangeAttackImagePath, kBossLongRangeAttackFrameCount, kBossLongRangeAttackDivX, kBossLongRangeAttackDivY, kBossLongRangeAttackFrameWidth, kBossLongRangeAttackFrameHeight, kAnimBossLongRangeAttackInterval },
		{ EnemyAnimSet::AnimKind::kDamage, kBossEnemyDamageImagePath, kBossDamageFrameCount, kBossDamageDivX, kBossDamageDivY, kBossDamageFrameWidth, kBossDamageFrameHeight, kAnimBossDamageInterval },
		{ EnemyAnimSet::AnimKind::kDead, kBossEnemyDeadImagePath, kBossDeadFrameCount, kBossDeadDivX, kBossDeadDivY, kBossDeadFrameWidth, kBossDeadFrameHeight, kAnimBossDeadInterval },

	};

	/// <summary>
	/// ボスエネミーのアニメーション配列のサイズ
	/// </summary>
	const int kBossAnimDefCount = static_cast<int>(sizeof(kBossAnimDefs) / sizeof(kBossAnimDefs[0]));
}

/// <summary>
/// コンストラクタ
/// </summary>
EnemyManager::EnemyManager()
{
}

/// <summary>
/// デストラクタ
/// </summary>
EnemyManager::~EnemyManager()
{
}

/// <summary>
/// 毎フレームの更新処理
/// </summary>
/// <param name="delta_time">前回実行フレームからの経過時間（秒）</param>
void EnemyManager::Update(float delta_time)
{
	if (game_level_ != nullptr && game_level_->IsStageChanging())
	{
		return;
	}

	//経過時間を計算
	enemy_elapsed_time_ += delta_time;

	//enemy_listの大きさ分だけの数値
	const std::size_t count = enemy_list_.size();
	//すべての敵データについて
	for (std::size_t i = 0; i < count; i++)
	{

		EnemyBase* enemy = enemy_list_[i];

		//参照できなければ
		if (enemy == nullptr)
		{
			//何もしない
			continue;
		}

		//出現していないなら
		if (!enemy->GetInfo().is_appeared)
		{
			//何もしない
			continue;
		}

		//出現中の敵についてUpdateをする
		enemy->Update(delta_time);

		
		const CollisionBase::CircleF player_circle = player_->GetHitCircle();
		const CollisionBase::CircleF enemy_circle = enemy->GetHitCircle();

		//プレイヤーが敵に接触したら
		if (CollisionBase::IsCircleOverlap(player_circle, enemy_circle))
		{
			//被ダメージ処理
			player_->TakeDamage(kContactDamage);

			//if (game_level_ != nullptr)
			//{
			//	//UIを「ゲームオーバー」状態へ
			//	game_level_->OnGameOver();

			//	enemy_manager_state_ = EnemyManagerState::kNone;
			//}
		}

		CollisionBase::CircleF enemy_attack_circle{};
		if (enemy->TryGetAttackHitCircle(enemy_attack_circle))
		{
			if (CollisionBase::IsCircleOverlap(player_circle, enemy_attack_circle))
			{
				player_->TakeDamage(kAttackDamage);

			}
		}


		CollisionBase::CircleF player_attack_circle{};

		//敵がプレイヤーの近距離攻撃に接触したら
		if (player_->TryGetCloseAttackHitCircle(player_attack_circle))
		{
			if (CollisionBase::IsCircleOverlap(player_attack_circle, enemy_circle))
			{
				enemy->OnHit();
			}
		}

		//プレイヤークナイのプールを取得
		const std::vector<PlayerKunai*>& pool = kunai_manager_->GetPlayerKunaiPool();

		const std::size_t pool_count = pool.size();
		for (std::size_t k = 0; k < pool_count; ++k)
		{
			//クナイそれぞれに対して発射されているかを見る
			PlayerKunai* kunai = pool[k];
			if (kunai == nullptr)
			{
				continue;
			}

			CollisionBase::CircleF kunai_circle{};
			//発射されていなければ次へ
			if (!kunai->TryGetHitCircle(kunai_circle))
			{
				continue;
			}

			//発射されていれば敵との接触判定をする
			//当たっていれば
			if (CollisionBase::IsCircleOverlap(kunai_circle, enemy_circle))
			{
				//ヒット処理
				enemy->OnHit();
				kunai->Deactivate();
				break;
			}
		}
	}

	//状態が「kNone」なら
	if (enemy_manager_state_ == EnemyManagerState::kNone)
	{
		//なにもしない(出現後の敵にUpdateはさせるためこの位置)
		return;
	}
}

/// <summary>
/// 毎フレームの描画処理
/// </summary>
void EnemyManager::Render()
{
	//enemy_listの大きさ分だけの数値
	const std::size_t count = enemy_list_.size();

	//生成した敵一体ずつについて
	for (int i = 0; i < count; i++)
	{
		//敵一体分の情報をとる
		EnemyBase* enemy = enemy_list_[i];

		//出現していないならば
		if (!enemy->GetInfo().is_appeared)
		{
			//何もせず次の敵の情報へ
			continue;
		}

		//敵の情報についてRenderを実施する
		enemy->Render();
	}


}

/// <summary>
/// 破棄前処理
/// </summary>
void EnemyManager::OnDestroy()
{
	FinalizeEnemy();
}


/// <summary>
/// アニメーションセットのリセット
/// </summary>
/// <param name="anim_set">リセットしたいアニメーションセット</param>
void EnemyManager::ResetAnimSet(EnemyAnimSet& anim_set)
{
	for (int i = 0; i < EnemyAnimSet::kAnimCount; ++i)
	{
		anim_set.anim_frame_counts_[i] = 0;
		anim_set.anim_width_[i] = 0;
		anim_set.anim_height_[i] = 0;
		anim_set.anim_interval_[i] = 0.0f;

		for (int j = 0; j < EnemyAnimSet::kMaxAnimFrameCount; ++j)
		{
			anim_set.anim_frames_[i][j] = kUnLoaded;
		}
	}
}

/// <summary>
/// アニメーションセットのセット
/// </summary>
void EnemyManager::LoadAnimSet(EnemyAnimSet& anim_set, const EnemyAnimSet::AnimDef* defs,int tile_defs_count)
{
	ResetAnimSet(anim_set);

	for (int i = 0; i < tile_defs_count; ++i)
	{
		//アニメーションの種類をインデックスにする
		const int anim_index = GetAnimIndex(defs[i].kind);

		//アニメーションフレームの最大値にロードする数を丸める
		const int load_count = std::min(defs[i].frame_count, EnemyAnimSet::kMaxAnimFrameCount);

		//フレームの数、一コマに使用する秒数をセット
		anim_set.anim_frame_counts_[anim_index] = load_count;
		anim_set.anim_interval_[anim_index] = defs[i].interval;

		//pathの画像をdiv_x * div_yで分割、frame_count個のハンドルをanim_frames_[anim_index][0]から順に書き込む
		LoadDivGraph(
			defs[i].path,
			load_count,
			defs[i].div_x,
			defs[i].div_y,
			defs[i].frame_width,
			defs[i].frame_height,
			&anim_set.anim_frames_[anim_index][0]
		);


		if (anim_set.anim_frames_[anim_index][0] >= 0)
		{
			GetGraphSize(anim_set.anim_frames_[anim_index][0],
				&anim_set.anim_width_[anim_index],
				&anim_set.anim_height_[anim_index]);
		}
	
	}
}

/// <summary>
/// アニメーションセットのアンロード
/// </summary>
void EnemyManager::UnloadAnimSet(EnemyAnimSet& anim_set)
{
	for (int i = 0; i < EnemyAnimSet::kAnimCount; ++i)
	{
		for (int j = 0; j < EnemyAnimSet::kMaxAnimFrameCount; ++j)
		{
			if (anim_set.anim_frames_[i][j] != kUnLoaded)
			{
				DeleteGraph(anim_set.anim_frames_[i][j]);
				anim_set.anim_frames_[i][j] = kUnLoaded;
			}
		}
	}
}


/// <summary>
/// 敵の初期化
/// </summary>
void EnemyManager::InitializeEnemy()
{
	//エネミーマネージャーを通常状態に
	enemy_manager_state_ = EnemyManagerState::kPlay;

	//パトロールエネミーのアニメセットを読み込み
	LoadAnimSet(patrol_anim_set_, kPatrolAnimDefs, kPatrolAnimDefCount);

	//遠距離攻撃エネミーのアニメセットを読み込み
	LoadAnimSet(long_range_anim_set_, kLongRangeAnimDefs, kLongRangeAnimDefCount);

	//ボスエネミーのアニメセットを読み込み
	LoadAnimSet(boss_anim_set_, kBossAnimDefs, kBossAnimDefCount);

	//領域確保
	enemy_list_.reserve(kAllEnemies);
}

/// <summary>
/// 敵の後処理
/// </summary>
void EnemyManager::FinalizeEnemy()
{
	const std::size_t count = enemy_list_.size();
	for (std::size_t i = 0; i < count; ++i)
	{
		if (enemy_list_[i] != nullptr)
		{
			//delete前に音声リソースを解放
			enemy_list_[i]->FinalizeAudio();

			delete enemy_list_[i];
			enemy_list_[i] = nullptr;
		}
	}
	enemy_list_.clear();

	UnloadAnimSet(patrol_anim_set_);
	UnloadAnimSet(long_range_anim_set_);
	UnloadAnimSet(boss_anim_set_);

}

/// <summary>
/// エネミーを全員倒したことにする
/// </summary>
void EnemyManager::DeleteAllEnemy()
{
	for (EnemyBase* enemy : enemy_list_)
	{
		delete enemy;
	}
	enemy_list_.clear();

}
/// <summary>
/// エフェクトマネージャーのポインタをセット
/// </summary>
void EnemyManager::SetEffectManager(EffectManager* effect_manager)
{
	effect_manager_ = effect_manager;

	const std::size_t count = enemy_list_.size();
	for (std::size_t i = 0; i < count; ++i)
	{
		EnemyBase* enemy = enemy_list_[i];
		if (enemy != nullptr)
		{
			enemy->SetEffectManager(effect_manager);
		}
	}
}

/// <summary>
/// 敵をCSVをもとにスポーンさせる
/// </summary>
void EnemyManager::SpawnEnemiesFromMap(
	const EnemySpawnMap& enemy_spawn_map,
	const TileSet& tile_set,
	GameLevel* game_level,
	EffectManager* effect_manager,
	KunaiManager* kunai_manager)
{
	const int map_width = enemy_spawn_map.GetWidth();
	const int map_height = enemy_spawn_map.GetHeight();

	for (int tile_y = 0; tile_y < map_height; ++tile_y)
	{
		for (int tile_x = 0; tile_x < map_width; ++tile_x)
		{
			const int spawn_id = enemy_spawn_map.GetSpawnId(tile_x, tile_y);
			if (spawn_id == kEnemySpawnNone)
			{
				continue;
			}

			SpawnOneEnemy(
				spawn_id,
				tile_x,
				tile_y,
				tile_set,
				game_level,
				effect_manager,
				kunai_manager);
		}
	}
}

/// <summary>
/// 実際に敵をスポーンさせる
/// </summary>
void EnemyManager::SpawnOneEnemy(
	int spawn_id,
	int tile_x,
	int tile_y,
	const TileSet& tile_set,
	GameLevel* game_level,
	EffectManager* effect_manager,
	KunaiManager* kunai_manager)
{
	const int tile_width = tile_set.GetTileWidth();
	const int tile_height = tile_set.GetTileHeight();

	const float spawn_x = static_cast<float>(tile_x * tile_width);
	const float spawn_y = static_cast<float>(tile_y * tile_height);

	if (spawn_id == kEnemySpawnPatrol)
	{
		PatrolEnemy* patrol_enemy = new PatrolEnemy();
		patrol_enemy->SetAnimSet(&patrol_anim_set_);
		patrol_enemy->SetEffectManager(effect_manager_);
		patrol_enemy->SetScoreManager(score_manager_);
		patrol_enemy->SetGameLevel(game_level_);
		patrol_enemy->SetPlayer(player_);

		patrol_enemy->Initialize();

		EnemyBase::EnemyInfo& info = patrol_enemy->GetInfo();
		const int idle_index = static_cast<int>(EnemyAnimSet::AnimKind::kRun);

		info.scale = kPatrolEnemyScale;
		info.width = patrol_anim_set_.anim_width_[idle_index];
		info.height = patrol_anim_set_.anim_height_[idle_index];

		float adjusted_spawn_x = 0.0f;
		float adjusted_spawn_y = 0.0f;

		game_level_->FindEnemySpawnPositionOnGround(
			tile_x,
			tile_y,
			info.width,
			info.height,
			info.scale,
			adjusted_spawn_x,
			adjusted_spawn_y
		);


		patrol_enemy->AppearEnemy(adjusted_spawn_x, adjusted_spawn_y);

		AddEnemy(patrol_enemy);
	}
	else if (spawn_id == kEnemySpawnRanged)
	{
		LongRangeEnemy* long_range_enemy = new LongRangeEnemy();
		long_range_enemy->SetAnimSet(&long_range_anim_set_);
		long_range_enemy->SetEffectManager(effect_manager_);
		long_range_enemy->SetScoreManager(score_manager_);
		long_range_enemy->SetGameLevel(game_level_);
		long_range_enemy->SetPlayer(player_);

		long_range_enemy->Initialize();

		EnemyBase::EnemyInfo& info = long_range_enemy->GetInfo();
		const int idle_index = static_cast<int>(EnemyAnimSet::AnimKind::kIdle);

		info.scale = kLongRangeEnemyScale;
		info.width = long_range_anim_set_.anim_width_[idle_index];
		info.height = long_range_anim_set_.anim_height_[idle_index];

		float adjusted_spawn_x = 0.0f;
		float adjusted_spawn_y = 0.0f;

		game_level_->FindEnemySpawnPositionOnGround(
			tile_x,
			tile_y,
			info.width,
			info.height,
			info.scale,
			adjusted_spawn_x,
			adjusted_spawn_y
		);

		long_range_enemy->AppearEnemy(adjusted_spawn_x, adjusted_spawn_y);

		AddEnemy(long_range_enemy);
	}
	else if (spawn_id == kEnemySpawnBoss)
	{
		BossEnemy* boss_enemy = new BossEnemy();
		boss_enemy->SetAnimSet(&boss_anim_set_);
		boss_enemy->SetEffectManager(effect_manager_);
		boss_enemy->SetScoreManager(score_manager_);
		boss_enemy->SetGameLevel(game_level_);
		boss_enemy->SetPlayer(player_);
		boss_enemy->SetKunaiManager(kunai_manager_);

		boss_enemy->Initialize();
		const BossEnemy::BossForm boss_form =
			is_enhanced_boss_
			? BossEnemy::BossForm::kEnhanced
			: BossEnemy::BossForm::kNormal;

		boss_enemy->SetBossForm(boss_form);

		EnemyBase::EnemyInfo& info = boss_enemy->GetInfo();
		const int idle_index = static_cast<int>(EnemyAnimSet::AnimKind::kIdle);

		info.scale = kBossEnemyScale;
		info.width = boss_anim_set_.anim_width_[idle_index];
		info.height = boss_anim_set_.anim_height_[idle_index];

		float adjusted_spawn_x = 0.0f;
		float adjusted_spawn_y = 0.0f;

		game_level_->FindEnemySpawnPositionOnGround(
			tile_x,
			tile_y,
			info.width,
			info.height,
			info.scale,
			adjusted_spawn_x,
			adjusted_spawn_y
		);

		boss_enemy->AppearEnemy(adjusted_spawn_x, adjusted_spawn_y);
		AddEnemy(boss_enemy);
	}
}

/// <summary>
/// 敵をリストに加える
/// </summary>
void EnemyManager::AddEnemy(EnemyBase* enemy)
{
	if (enemy == nullptr)
	{
		return;
	}

	enemy_list_.push_back(enemy);
}

/// <summary>
/// ボスが撃破されたか
/// </summary>
bool EnemyManager::IsBossDefeated() const
{
	for (std::size_t i = 0; i < enemy_list_.size(); ++i)
	{
		const EnemyBase* enemy = enemy_list_[i];
		if (enemy == nullptr)
		{
			continue;
		}

		//is_appeared が false になった = 撃破済み
		const EnemyBase::EnemyInfo& info = enemy->GetInfo();
		if (!info.is_appeared)
		{
			return true;
		}
	}
	return false;
}