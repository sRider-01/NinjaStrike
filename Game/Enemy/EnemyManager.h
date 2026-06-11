#pragma once
#include "EnemyAnimSet.h"
#include "Task\Task.h"
#include "EnemySpawnMap/EnemySpawnMap.h"
#include "Kunai/KunaiManager.h"
#include "Effect/EffectManager.h"
#include "TileMap/TileSet.h"
#include <vector>
#include <iostream>
#include <functional>


/// <summary>
/// 前方宣言
/// </summary>
class PatrolEnemy;
class EnemyBase;
class KunaiManager;
class ScoreManager;
class ItemManager;
class Player;


/// <summary>
/// 敵の情報を管理するクラス
/// </summary>
class EnemyManager : public Task
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	EnemyManager();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyManager();

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
	/// 破棄前処理
	/// </summary>
	void OnDestroy() override;

	/// <summary>
	/// アニメーションのインデックスを取得
	/// </summary>
	/// <param name="kind">アニメーションの種類</param>
	int GetAnimIndex(EnemyAnimSet::AnimKind kind)
	{
		return static_cast<int>(kind);
	}

	/// <summary>
	/// アニメーションセットのリセット
	/// </summary>
	/// <param name="anim_set">リセットしたいアニメーションセット</param>
	void ResetAnimSet(EnemyAnimSet& anim_set);

	/// <summary>
	/// アニメーションセットのセット
	/// </summary>
	/// <param name="anim_set">セット先のアニメーションセット</param>
	/// <param name="defs">セットしたいアニメーションの情報内訳</param>
	/// <param name="tile_defs_count">defsがいくつかるかを示す</param>
	void LoadAnimSet(EnemyAnimSet& anim_set, const EnemyAnimSet::AnimDef* defs, int tile_defs_count);

	/// <summary>
	/// アニメーションセットのアンロード
	/// </summary>
	/// <param name="anim_set">アンロードしたいアニメーションセット</param>
	void UnloadAnimSet(EnemyAnimSet& anim_set);

	/// <summary>
	/// 敵の初期化
	/// </summary>
	void InitializeEnemy();

	/// <summary>
	/// 敵の後処理
	/// </summary>
	void FinalizeEnemy();

	/// <summary>
	/// 敵をCSVをもとにスポーンさせる
	/// </summary>
	/// <param name="enemy_spawn_map">敵のスポーン情報のマップ</param>
	/// <param name="tile_set">タイルセット</param>
	/// <param name="game_level">スポーン先のレベル</param>
	/// <param name="effect_manager">エフェクトマネージャー</param>
	/// <param name="kunai_manager">クナイマネージャー</param>
	void SpawnEnemiesFromMap(
		const EnemySpawnMap& enemy_spawn_map,
		const TileSet& tile_set,
		GameLevel* game_level,
		EffectManager* effect_manager,
		KunaiManager* kunai_manager);

	/// <summary>
	/// 実際に敵をスポーンさせる
	/// </summary>
	/// <param name="spawn_id">スポーンID</param>
	/// <param name="tile_x">タイルX座標</param>
	/// <param name="tile_y">タイルY座標</param>
	/// <param name="tile_set">タイルセット</param>
	/// <param name="game_level">ゲームレベル</param>
	/// <param name="effect_manager">エフェクトマネージャー</param>
	/// <param name="kunai_manager">クナイマネージャー</param>
	void SpawnOneEnemy(
		int spawn_id,
		int tile_x,
		int tile_y,
		const TileSet& tile_set,
		GameLevel* game_level,
		EffectManager* effect_manager,
		KunaiManager* kunai_manager);

	/// <summary>
	/// 敵をリストに加える
	/// </summary>
	/// <param name="enemy">敵の情報</param>
	void AddEnemy(EnemyBase* enemy);


	/// <summary>
	/// 敵の出現情報
	/// </summary>
	struct EnemySpawnInfo
	{
		/// <summary>
		/// 敵の種類
		/// </summary>
		std::string enemy_name;

		/// <summary>
		/// 出現時間
		/// </summary>
		float spawn_time;

		/// <summary>
		/// X座標
		/// </summary>
		float spawn_x;

		/// <summary>
		/// Y座標
		/// </summary>
		float spawn_y;

		/// <summary>
		/// 出現済か？
		/// </summary>
		bool is_appeared = false;
	};

	/// <summary>
	/// 敵を全員倒したことにする
	/// </summary>
	void DeleteAllEnemy();

	/// <summary>
	/// ボスが撃破されたか（出現済みかつ死亡済みの場合 true）
	/// </summary>
	/// <returns>ボスが撃破されたか</returns>
	bool IsBossDefeated() const;

	/// <summary>
	/// プレイヤーのポインタを引数の内容にセットする
	/// </summary>
	/// <param name="player">プレイヤーのポインタ</param>
	void SetPlayer(Player* player)
	{
		player_ = player;
	}

	/// <summary>
	/// EffectManagerのポインタをセット
	/// </summary>
	/// <param name="effect_manager">ポインタ</param>
	void SetEffectManager(EffectManager* effect_manager);

	/// <summary>
	/// GameLevelのポインタをセット
	/// </summary>
	void SetGameLevel(GameLevel* battle_level)
	{
		game_level_ = battle_level;
	}

	/// <summary>
	/// KunaiManagerのポインタをセット
	/// </summary>
	/// <param name="kunai_manager">KunaiManagerのポインタ</param>
	void SetKunaiManager(KunaiManager* kunai_manager)
	{
		kunai_manager_ = kunai_manager;
	}

	/// <summary>
	/// 強化ボスフラグをセットする
	/// </summary>
	/// <param name="is_enhanced">強化ボスならtrue</param>
	void SetEnhancedBoss(bool is_enhanced)
	{
		is_enhanced_boss_ = is_enhanced;
	}
private:
	/// <summary>
	/// エネミーマネージャーの状態
	/// </summary>
	enum class EnemyManagerState
	{
		kNone, //なにもしない
		kPlay, //通常
	};

	/// <summary>
	/// CSVの情報をまとめたリスト
	/// </summary>
	std::vector<EnemySpawnInfo> spawn_list_;

	/// <summary>
	/// CSV読み込み用次に読む行のインデックス
	/// </summary>
	int spawn_index_ = 0;

	/// <summary>
	/// パトロールエネミーのアニメーションセット
	/// </summary>
	EnemyAnimSet patrol_anim_set_;

	/// <summary>
	/// 遠距離攻撃エネミーのアニメーションセット
	/// </summary>
	EnemyAnimSet long_range_anim_set_;

	/// <summary>
	/// ボスエネミーのアニメーションセット
	/// </summary>
	EnemyAnimSet boss_anim_set_;

	/// <summary>
	/// 強化ボスとしてスポーンするか
	/// </summary>
	bool is_enhanced_boss_ = false;


	/// <summary>
	/// プレイヤーのアドレスを覚えておくため
	/// </summary>
	Player* player_ = nullptr;

	/// <summary>
	/// クナイマネージャーのポインタ
	/// </summary>
	KunaiManager* kunai_manager_ = nullptr;

	/// <summary>
	/// GameLevelのポインタ
	/// </summary>
	GameLevel* game_level_ = nullptr;

	/// <summary>
	/// 敵の情報を入れるリスト
	/// </summary>
	std::vector<EnemyBase*> enemy_list_;

	/// <summary>
	/// 累計時間
	/// </summary>
	float enemy_elapsed_time_ = 0.0f;

	/// <summary>
	/// EffectManagerのポインタ
	/// </summary>
	EffectManager* effect_manager_ = nullptr;

	/// <summary>
	/// ScoreManagerのポインタ
	/// </summary>
	ScoreManager* score_manager_ = nullptr;

	/// <summary>
	/// ItemManagerのポインタ
	/// </summary>
	ItemManager* item_manager_ = nullptr;

	/// <summary>
	/// 初期状態は「なにもしない」
	/// </summary>
	EnemyManagerState enemy_manager_state_ = EnemyManagerState::kNone;
};
