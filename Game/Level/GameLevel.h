#pragma once
#include "LevelBase.h"
#include "UI/GameUI/GameUI.h"
#include "Dialogue//DialogueManager.h"
#include "Tutorial/TutorialManager.h"
#include "EnemySpawnMap/EnemySpawnMap.h"
#include "UI/HpBar/HpBar.h"
#include "Camera/Camera2D.h"
#include "Audio/AudioManager.h"
#include "Stage/BackGround.h"
#include "TileMap/TileSet.h"
#include "TileMap/TileDataBase.h"
#include "TileMap/TileMap.h"
#include "Stage/TileMapRenderer.h"
#include <array>

/// <summary>
/// 前方宣言
/// </summary>
class Player;        
class KunaiManager; 	
class BossEnemy;    
class PatrolEnemy;  
class EnemyManager; 
class EffectManager;
class EnemyBase;   
class PauseManager;

/// <summary>
/// バトル画面のレベル
/// </summary>
class GameLevel : public LevelBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	explicit GameLevel();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~GameLevel();

public:

	/// <summary>
	/// ゲームレベルの状態
	/// </summary>
	enum class GameLevelState
	{
		/// <summary>
		/// 処理なし
		/// </summary>
		kNone,

		/// <summary>
		/// 通常処理
		/// </summary>
		kPlay,
	};

	/// <summary>
	/// ステージチェンジ機能の状態
	/// </summary>
	enum class StageChangeState
	{
		/// <summary>
		/// 処理なし
		/// </summary>
		kNone,

		/// <summary>
		/// フェードアウト
		/// </summary>
		kFadeOut,

		/// <summary>
		/// ステージチェンジ
		/// </summary>
		kChanging,

		/// <summary>
		/// フェードイン
		/// </summary>
		kFadeIn,

		/// <summary>
		/// FadeIn前に1フレーム待つ
		/// </summary>
		kWait,
	};

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
	/// レベルの開始処理
	/// </summary>
	void BeginLevel() override;

	/// <summary>
	/// ゲームレベルの管理するものをタスクマネージャーから解放する
	/// </summary>
	void ReleaseLevel(TaskId task_id) override;

	/// <summary>
	/// ゲームレベルの管理するものを破棄
	/// </summary>
	void DestroyLevel() override;

	/// <summary>
	/// 破棄前処理
	/// </summary>
	void OnDestroy() override;

	/// <summary>
	/// バレットマネージャーのポインタを返すメソッド
	/// </summary>
	/// <returns>バレットマネージャーのポインタ</returns>
	KunaiManager* GetKunaiPointer() const
	{
		return kunai_manager_;
	}

	/// <summary>
	/// GameLevelStateの状態を返す処理
	/// </summary>
	/// <returns>GameLevelStateの状態</returns>
	GameLevelState GetState() const
	{
		return battle_level_state_;
	}

	/// <summary>
	/// タイルセットを取得する
	/// </summary>
	/// <returns>タイルセット</returns>
	TileSet* GetTileSet()
	{
		return &tile_set_; 
	}

	/// <summary>
	/// タイルマップを取得する
	/// </summary>
	/// <returns>タイルマップ</returns>
	TileMap* GetTileMap() 
	{
		return &tile_map_; 
	}

	/// <summary>
	/// タイルデータベースを取得する
	/// </summary>
	/// <returns>タイルデータベース</returns>
	TileDatabase* GetTileDatabase()
	{ 
		return &tile_database_; 
	}

	/// <summary>
	/// カメラの左上X座標を返す
	/// </summary>
	float GetCameraLeftX() const
	{
		return static_cast<float>(camera_x_);
	}

	/// <summary>
	/// カメラの上端Y座標を返す
	/// </summary>
	/// <returns></returns>
	float GetCameraTopY() const
	{
		return static_cast<float>(camera_y_);
	}

	/// <summary>
	/// ワールドマップの幅を計算して返す
	/// </summary>
	float GetWorldWidth() const;

	/// <summary>
	/// ワールドマップの高さを計算して返す
	/// </summary>
	float GetWorldHeight() const;


	/// <summary>
	/// スクロール処理
	/// </summary>
	/// <param name="bg_pos">画像の位置</param>
	/// <param name="scroll_speed">スクロールスピード</param>
	/// <returns>スクロール後の画像の位置</returns>
	int UpdateScroll(int bg_pos, int scroll_speed);

	/// <summary>
	/// スクロール描画処理
	/// </summary>
	/// <param name="start_pos_x">開始位置X</param>
	/// <param name="start_pos_y">開始位置Y</param>
	/// <param name="image_handle">画像のハンドル</param>
	void RenderScroll(int start_pos_x, int start_pos_y, int image_handle);

	/// <summary>
	/// ゲームオーバーにする
	/// </summary>
	void OnGameOver();

	/// <summary>
	/// ゲームクリアにする
	/// </summary>
	void OnGameClear();

	/// <summary>
	/// スポーンする座標を計算する
	/// </summary>
	/// <param name="spawn_tile_x">スポーン先のタイルのX座標</param>
	/// <param name="out_spawn_x">スポーンさせるX座標の入れ先</param>
	/// <param name="out_spawn_y">スポーンさせるY座標の入れ先</param>
	/// <returns></returns>
	bool FindSpawnPositionOnGround(int spawn_tile_x, float& out_spawn_x, float& out_spawn_y) const;

	/// <summary>
	/// 敵を床の上にスポーンさせる座標を計算する
	/// </summary>
	/// <param name="spawn_tile_x">スポーンさせるタイルX座標</param>
	/// <param name="spawn_tile_y">スポーンさせるタイルY座標</param>
	/// <param name="enemy_width">敵の幅</param>
	/// <param name="enemy_height">敵の高さ</param>
	/// <param name="enemy_scale">敵の拡大率</param>
	/// <param name="out_spawn_x">スポーンさせるX座標の保存先</param>
	/// <param name="out_spawn_y">スポーンさせるY座標の保存先</param>
	/// <returns></returns>
	bool FindEnemySpawnPositionOnGround(
		int spawn_tile_x,
		int spawn_tile_y,
		int enemy_width,
		int enemy_height,
		float enemy_scale,
		float& out_spawn_x,
		float& out_spawn_y) const;

	/// <summary>
	/// プレイヤーをスポーンさせる
	/// </summary>
	void SpawnPlayerOnGround(int spawn_tile_x);

	/// <summary>
	/// チュートリアルの領域を踏んでいるか確認する
	/// </summary>
	void CheckTutorialTriggers();

	/// <summary>
	/// ステージチェンジの領域を踏んでいるか確認する
	/// </summary>
	void CheckStageChangeTrigger();

	/// <summary>
	/// チュートリアルトリガーCSVを読み込む
	/// </summary>
	bool LoadTutorialTriggerCSV(const char* file_path);

	/// <summary>
	/// ステージチェンジCSVを読み込む
	/// </summary>
	bool LoadStageChangeTriggerCSV(const char* file_path);

	/// <summary>
	/// チュートリアルIDをタイプへ変換する
	/// </summary>
	TutorialManager::TutorialType ConvertTutorialTypeFromId(int id) const;

	/// <summary>
	/// 読み込んだCSVからチュートリアルトリガーを作る
	/// </summary>
	void BuildTutorialTriggersFromCSV();

	/// <summary>
	/// ステージチェンジを始める
	/// </summary>
	void StartStageChange(int next_stage_id, int spawn_column);

	/// <summary>
	/// 切り替え状態の更新
	/// </summary>
	void UpdateStageChange();

	/// <summary>
	/// 実際のステージ変更処理
	/// </summary>
	/// <param name="stage_id">次のステージID</param>
	void ChangeStage(int stage_id);

	/// <summary>
	/// 今ステージチェンジしているか？
	/// </summary>
	/// <returns>Yes/No</returns>
	bool IsStageChanging() const
	{
		return is_stage_changing_;
	}

	/// <summary>
	/// ゲームが終了状態（クリア or ゲームオーバー）か
	/// </summary>
	bool IsGameEnded() const
	{
		return is_game_clear_ || is_game_over_;
	}

	/// <summary>
	/// 今会話パート中か？
	/// </summary>
	bool IsInDialogue() const
	{
		return is_in_dialogue_ || is_waiting_dialogue_on_land_;
	}

	/// <summary>
	/// ボス入場時の会話データ
	/// </summary>
	struct BossDialogueData
	{
		/// <summary>
		/// 表示するメッセージのリスト
		/// </summary>
		std::vector<std::string> messages;
	};

	/// <summary>
	/// ゲームUI を最前面に描画する
	/// TaskManager::RenderTask() の後に Main.cpp から呼ぶ
	/// </summary>
	void RenderUI()
	{
		if (game_ui_ != nullptr)
		{
			game_ui_->Render();
		}
	}
private:

	/// <summary>
	/// 現在のステージID
	/// </summary>
	int current_stage_id_ = -1;

	/// <summary>
	/// タイルセット
	/// </summary>
	TileSet tile_set_;

	/// <summary>
	/// タイルセットのデータベース
	/// </summary>
	TileDatabase tile_database_;

	/// <summary>
	/// 敵のスポーンマップ
	/// </summary>
	EnemySpawnMap enemy_spawn_map_;

	/// <summary>
	/// チュートリアルマネージャー
	/// </summary>
	TutorialManager tutorial_manager_;

	/// <summary>
	/// チュートリアル発生地点のリスト
	/// </summary>
	std::vector<TutorialManager::TutorialTrigger> tutorial_trigger_list_;

	/// <summary>
   /// チュートリアルトリガーIDマップ
   /// </summary>
	std::vector<std::vector<int>> tutorial_trigger_id_map_;

	/// <summary>
	/// ステージチェンジトリガーIDマップ
	/// </summary>
	std::vector<std::vector<int>> stage_change_trigger_id_map_;

	/// <summary>
	/// タイルマップ
	/// </summary>
	TileMap tile_map_;

	/// <summary>
	/// タイルのレンダー
	/// </summary>
	TileMapRenderer tile_renderer_;

	/// <summary>
	/// 背景
	/// </summary>
	BackGround back_ground_;

	/// <summary>
	/// カメラ
	/// </summary>
	Camera2D camera_;

	/// <summary>
	/// カメラX座標
	/// </summary>
	int camera_x_ = 0;
	
	/// <summary>
	/// カメラY座標
	/// </summary>
	int camera_y_ = 0;

	/// <summary>
	/// ゲームレベルの状態
	/// </summary>
	GameLevelState battle_level_state_;
	
	/// <summary>
	/// ダイアログマネージャー
	/// </summary>
	DialogueManager dialogue_manager_;

	/// <summary>
	/// 今は会話中か？
	/// </summary>
	bool is_in_dialogue_ = false;

	/// <summary>
	/// 着地待ちダイアログ開始待機中か
	/// </summary>
	bool is_waiting_dialogue_on_land_ = false;

	/// <summary>
	/// 着地後に表示する会話データ
	/// </summary>
	BossDialogueData pending_dialogue_data_;

	/// <summary>
	/// 現在の経過時間
	/// </summary>
	float elapsed_time_ = 0.0f;

	/// <summary>
	/// HPバークラスのポインタ
	/// </summary>
	HpBar* hp_bar_ = nullptr;

	/// <summary>
	/// プレイヤークラスのポインタ
	/// </summary>
	Player* player_ = nullptr;

	/// <summary>
	/// バレットマネージャーのポインタ
	/// </summary>
	KunaiManager* kunai_manager_ = nullptr;

	/// <summary>
	/// ゲームレベルUIのポインタ
	/// </summary>
	GameUI* game_ui_ = nullptr;

	/// <summary>
	/// エネミーマネージャーのポインタ
	/// </summary>
	EnemyManager* enemy_manager_ = nullptr;

	/// <summary>
	/// エフェクトマネージャーのポインタ
	/// </summary>
	EffectManager* effect_manager_ = nullptr;


	/// <summary>
	/// PauseManagerのポインタ
	/// </summary>
	PauseManager* pause_manager_ = nullptr;

	/// <summary>
	/// BGM用のハンドル
	/// </summary>
	int audio_handle_ = 0;

	/// <summary>
	/// SE用のハンドル
	/// </summary>
	int se_handle_ = 0;

	/// <summary>
	/// アイテム取得時SE用のハンドル
	/// </summary>
	int item_get_se_handle_ = 0;

	/// <summary>
	/// ゲームオーバーか？
	/// </summary>
	bool is_game_over_ = false;

	/// <summary>
	/// ゲームクリアか？
	/// </summary>
	bool is_game_clear_ = false;

	/// <summary>
	/// ゲームオーバーになってからの経過時間
	/// </summary>
	float game_over_elapsed_ = 0.0f;

	/// <summary>
	/// ゲームオーバー時からの画面遷移時間
	/// </summary>
	float game_over_wait_time_ = 0.0f;

	/// <summary>
	/// ゲームクリアになってからの経過時間
	/// </summary>
	float game_clear_elapsed_ = 0.0f;

	/// <summary>
	/// ゲームクリア時からの画面遷移時間
	/// </summary>
	float game_clear_wait_time_ = 0.0f;

	/// <summary>
	/// フェードアウトは開始されているか？
	/// </summary>
	bool is_fade_out_start_ = false;

	/// <summary>
	/// クリアSEを再生したか？
	/// </summary>
	bool is_play_se_clear_ = false;

	/// <summary>
	/// ステージチェンジ機能の状態の初期値は処理なし
	/// </summary>
	StageChangeState stage_change_state_ = StageChangeState::kNone;
	
	/// <summary>
	/// 今ステージチェンジしているか？
	/// </summary>
	bool is_stage_changing_ = false;

	/// <summary>
	/// 次のステージのID
	/// </summary>
	int next_stage_id_ = -1;

	/// <summary>
	/// 次のスポーン列
	/// </summary>
	int next_spawn_column_ = 0;

	/// <summary>
	/// BGMハンドル
	/// </summary>
	int bgm_handle_ = -1;

	/// <summary>
	/// ボス撃破SEハンドル
	/// </summary>
	int boss_defeat_se_handle_ = -1;

	/// <summary>
	/// クリアSEハンドル
	/// </summary>
	int game_clear_bgm_handle_ = -1;
};
