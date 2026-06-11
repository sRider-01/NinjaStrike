#include "DxLib.h"
#include "GameLevel.h"
#include "UI/GameUI/GameUI.h"
#include "LevelChanger.h"
#include "FadeManager.h"
#include "Player/Player.h"
#include "GameInfo.h"
#include "Pause/PauseManager.h"
#include "Task/TaskManager.h"
#include "Kunai/KunaiManager.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/EnemyBase.h"
#include "Effect/EffectManager.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>


/// <summary>
/// GameLevel.cppのみで使用する定数
/// </summary>
namespace
{

	/// <summary>
	///チュートリアルステージBGM
	/// </summary>
	const char* kTutorialBgmPath = "Assets/Audio/BGM/Tutorial.mp3";

	/// <summary> 
	///ステージ01BGM
	/// </summary>
	const char* kStage01BgmPath = "Assets/Audio/BGM/Stage01.mp3";

	/// <summary>
	///通常ボスBGM
	/// </summary>
	const char* kNormalBossBgmPath = "Assets/Audio/BGM/BossBattle.mp3";

	/// <summary>
	///強化ボスBGM
	///</summary>
	const char* kEnhancedBossBgmPath = "Assets/Audio/BGM/BossBattle.mp3";

	/// <summary>
	///プレイヤー被ダメージSE
	/// </summary>
	const char* kPlayerDamageSePath = "Assets/Audio/SE/PlayerDamage.mp3";

	/// <summary>
	//ボス撃破SE
	// </summary>
	const char* kBossDefeatSePath = "Assets/Audio/SE/BossBattle.mp3";

	/// <summary>
	///ゲームクリアBGM
	/// </summary>
	const char* kGameClearBgmPath = "Assets/Audio/BGM/GameClear.mp3";

	/// <summary>
	/// 値を四分の一にする
	/// </summary>
	const float kQuarter = 0.25f;

	/// <summary>
	/// 値を半分にする
	/// </summary>
	const float kHalf = 0.5f;

	/// <summary>
	/// 足元判定に使うオフセット
	/// </summary>
	const float kFootOffsetY = 4.0f;

	/// <summary>
	/// フェードインにかける時間
	/// </summary>
	const float kFadeInTime = 1.0f;

	/// <summary>
	/// フェードアウトにかける時間
	/// </summary>
	const float kFadeOutTime = 2.0f;

	/// <summary>
	/// タイルの大きさ（○×○）
	/// </summary>
	const int kTileSize = 16;

	/// <summary>
	/// タイルセットのパス
	/// </summary>
	const char* kTileSetPath = "Assets/Image/TileSet/Snow platform tileset.png";

	/// <summary>
	/// CSVのパス
	/// </summary>
	const char* kTestMapCSVPath = "Assets/CSV/TestMap.csv";

	/// <summary>
	/// 本番用チュートリアルステージCSVのパス
	/// </summary>
	const char* kTutorialMapCSVPath = "Assets/CSV/Tutorial.csv";

	/// <summary>
	/// 本番用チュートリアルステージ敵スポーン情報CSVのパス
	/// </summary>
	const char* kTutorialMapEnemySpawnCSVPath = "Assets/CSV/TutorialEnemySpawn.csv";

	/// <summary>
	/// 本番用チュートリアルステージのチュートリアルトリガー情報CSVのパス
	/// </summary>
	const char* kTutorialTriggerCSVPath = "Assets/CSV/TutorialTrigger.csv";

	/// <summary>
	/// 本番用チュートリアルステージのステージチェンジ情報CSVのパス
	/// </summary>
	const char* kTutorialStageChangeTriggerCSVPath = "Assets/CSV/TutorialStageChangeTrigger.csv";

	/// <summary>
	/// ステージ１のマップID
	/// </summary>
	const int kStage01MapID = 101;

	/// <summary>
	/// ステージ１のタイルID
	/// </summary>
	const int kStage01TileID = 31;

	/// <summary>
	/// 本番用ステージ01CSVのパス
	/// </summary>
	const char* kStage01MapCSVPath = "Assets/CSV/Stage01.csv";

	/// <summary>
	/// 本番用ステージ01CSVのパス
	/// </summary>
	const char* kStage01MapEnemySpawnCSVPath = "Assets/CSV/Stage01_EnemySpawn.csv";

	/// <summary>
	/// 本番用ステージ01のステージチェンジ情報CSVのパス
	/// </summary>
	const char* kStage01StageChangeTriggerCSVPath = "Assets/CSV/Stage01_StageChangeTrigger.csv";

	/// <summary>
	/// 本番用ステージ01のチュートリアルトリガーCSVのパス
	/// </summary>
	const char* kStage01TutorialTriggerCSVPath = "Assets/CSV/Stage01_TutorialTrigger.csv";

	/// <summary>
	/// 通常ボスの入場メッセージ
	/// </summary>
	const GameLevel::BossDialogueData kNormalBossDialogue =
	{
		{
			"よく来たな。。",
			"どこからでもかかってくるがいい。。！"
		}
	};

	/// <summary>
	/// 強化ボスの入場メッセージ
	/// </summary>
	const GameLevel::BossDialogueData kEnhancedBossDialogue =
	{
		{
			"よくここがわかったな。。",
			"お望み通り全霊をもってお相手しよう！！"
		}
	};
	/// <summary>
	/// 本番用ノーマルボスステージのマップID
	/// </summary>
	const int kNormalBossMapID = 102;

	/// <summary>
	/// 本番用ノーマルボスステージのタイルID
	/// </summary>
	const int kNormalBossTileID = 32;

	/// <summary>
	/// 本番用ノーマルボスステージCSVのパス
	/// </summary>
	const char* kNormalBossMapCSVPath = "Assets/CSV/NormalBossStage.csv";

	/// <summary>
	/// 本番用ノーマルボスステージの敵スポーン情報CSVのパス
	/// </summary>
	const char* kNormalBossMapEnemySpawnCSVPath = "Assets/CSV/NormalBossStage_EnemySpawn.csv";

	/// <summary>
	/// 本番用強化ボスステージのマップID
	/// </summary>
	const int kEnhancedBossMapID = 103;

	/// <summary>
	/// 本番用強化ボスステージのタイルID
	/// </summary>
	const int kEnhancedBossTileID = 33;

	/// <summary>
	/// 本番用強化ボスステージのCSVのパス
	/// </summary>
	const char* kEnhancedBossMapCSVPath = "Assets/CSV/EnhancedBossStage.csv";

	/// <summary>
	/// 本番用強化ボスステージの敵スポーン情報CSVのパス
	/// </summary>
	const char* kEnhancedBossMapEnemySpawnCSVPath = "Assets/CSV/EnhancedBossStage_EnemySpawn.csv";

	/// <summary>
	/// スポーンする列
	/// </summary>
	const int kSpawnColumn = 3;

	/// <summary>
	/// 値を二倍にする
	/// </summary>
	const float kTwoTimes = 2.0f;

	/// <summary>
	/// 値を十倍する
	/// </summary>
	const int kTenTimes = 10;

	/// <summary>
	/// 領域にヒットしているか
	/// </summary>
	bool IsRectHit(
		float ax, float ay, float aw, float ah,
		float bx, float by, float bw, float bh)
	{
		if (ax + aw < bx) return false;
		if (bx + bw < ax) return false;
		if (ay + ah < by) return false;
		if (by + bh < ay) return false;
		return true;
	}

	/// <summary>
	/// ステージチェンジのフェードイン時間
	/// </summary>
	const float kStageChangeFadeInTime = 0.5f;
}

/// <summary>
/// コンストラクタ クラスの初期化
/// </summary>
GameLevel::GameLevel()
	: battle_level_state_(GameLevelState::kNone)
	, elapsed_time_(0.0f)
	, player_(nullptr)
{
}

/// <summary>
/// デストラクタ
/// </summary>
GameLevel::~GameLevel()
{
}

/// <summary>
/// 毎フレームの更新処理
/// </summary>
/// <param name="delta_time">前回実行フレームからの経過時間（秒）</param>
void GameLevel::Update(float delta_time)
{
	//何もしない状態なら実行しない
	if (battle_level_state_ == GameLevelState::kNone)
	{
		return;
	}


	if (game_ui_ != nullptr)
	{
		game_ui_->Update(delta_time);
	}

	//ゲームパッドが接続されているかをセット
	tutorial_manager_.SetGamepadConnected(
		InputManager::GetInstance()->IsGamepadConnected()
	);
	dialogue_manager_.SetGamepadConnected(
		InputManager::GetInstance()->IsGamepadConnected()
	);

	if (pause_manager_ != nullptr)
	{
		pause_manager_->SetGamepadConnected(
			InputManager::GetInstance()->IsGamepadConnected()
		);
	}

	//フェード更新
	FadeManager::GetInstance()->Update(delta_time);

	tile_map_.UpdateHitTiles(delta_time);

	if (hp_bar_ != nullptr && player_ != nullptr)
	{
		hp_bar_->SetHp(player_->GetCurrentHp());
	}

	if (!is_stage_changing_)
	{
		if (is_waiting_dialogue_on_land_ && player_ != nullptr)
		{
			if (player_->IsOnGround())
			{
				dialogue_manager_.StartDialogue(pending_dialogue_data_.messages);
				is_in_dialogue_ = true;
				is_waiting_dialogue_on_land_ = false;
			}
		}

		//会話中は更新をスキップ
		if (is_in_dialogue_)
		{
			dialogue_manager_.Update();

			//会話終了したら戦闘開始
			if (dialogue_manager_.IsFinished())
			{
				is_in_dialogue_ = false;
			}
			return;
		}

		// ボスステージのみクリア検知する
		const bool is_boss_stage =
			(current_stage_id_ == kNormalBossMapID ||
				current_stage_id_ == kEnhancedBossMapID);

		if (player_ != nullptr)
		{
			if (!player_->IsAlive() && !is_game_over_)
			{
				OnGameOver();
			}

			if (!is_game_clear_ && is_boss_stage && enemy_manager_ != nullptr)
			{
				if (enemy_manager_->IsBossDefeated())
				{
					OnGameClear();
				}
			}

			//カメラのターゲットとなるPlayerを中心に置く
			const float player_center_x = player_->GetPlayerPositionX() + player_->GetPlayerImageWidth() * kHalf;
			const float player_center_y = player_->GetPlayerPositionY() + player_->GetPlayerImageHeight() * kHalf;

			camera_.Update(player_center_x, player_center_y, delta_time);

			camera_x_ = static_cast<int>(camera_.GetCameraLeftX());
			camera_y_ = static_cast<int>(camera_.GetCameraTopY());

			CheckTutorialTriggers();
			CheckStageChangeTrigger();
		}
	}

	UpdateStageChange();

	// ゲームオーバー選択が確定したら遷移
	if (is_game_over_ && game_ui_ != nullptr)
	{
		if (game_ui_->IsGameOverSelectionConfirmed())
		{
			if (game_ui_->GetGameOverSelection() == GameUI::GameOverSelection::kRetry)
			{
				LevelChanger::GetInstance()->SetLevelChangerState(
					LevelChanger::LevelChangerState::kFinGameLevelToRestart);
			}
			else
			{
				// タイトルへ
				LevelChanger::GetInstance()->SetLevelChangerState(
					LevelChanger::LevelChangerState::kFinGameLevel);
			}
		}
	}

	//クリア選択が確定したらタイトルへ
	if (is_game_clear_ && game_ui_ != nullptr)
	{
		if (game_ui_->IsGameClearSelectionConfirmed())
		{
			LevelChanger::GetInstance()->SetLevelChangerState(
				LevelChanger::LevelChangerState::kFinGameLevel);
		}
	}
}

/// <summary>
/// 毎フレームの描画処理
/// </summary>
void GameLevel::Render()
{
	const int screen_width = GameInfo::GetInstance()->GetResolutionX();
	const int screen_height = GameInfo::GetInstance()->GetResolutionY();

	back_ground_.Draw(screen_width, screen_height);

	tile_renderer_.Draw(tile_map_, tile_database_, tile_set_,
		static_cast<int>(camera_x_), static_cast<int>(camera_y_));

	tutorial_manager_.Render();

	dialogue_manager_.Render();

	FadeManager::GetInstance()->DrawGraphics();
}

/// <summary>
/// レベルの開始処理
/// </summary>
void GameLevel::BeginLevel()
{
	//フェード画像読み込み
	FadeManager::GetInstance()->LoadGraphics();

	//フェードインさせる
	FadeManager::GetInstance()->StartFadeIn(kFadeInTime);

	//GameUIをnew
	game_ui_ = new GameUI();

	//画像の読み込み
	game_ui_->LoadGraphics();

	//状態を通常へ
	battle_level_state_ = GameLevelState::kPlay;

	//KunaiManagerをnewしてGameLevelが所持
	kunai_manager_ = new KunaiManager();

	//クナイを初期化する処理を呼び出す
	kunai_manager_->InitializeKunai();

	//タスクマネージャーに追加
	TaskManager::GetInstance()->AddTask(kunai_manager_);

	//ゲームレベルのポインタをセット
	kunai_manager_->SetGameLevel(this);

	//マップ、背景のロード
	tile_set_.Load(kTileSetPath, kTileSize, kTileSize, 0, 0);

	//タイルデータベースの初期化
	tile_database_.Initialize();
	
	//タイルデータベースのサイズ、CSVの読み込み、HPの付与
	tile_map_.SetTileSize(kTileSize);
	tile_map_.LoadFromCSV(kTutorialMapCSVPath);
	tile_map_.InitializeTileHp(tile_database_);

	// チュートリアルトリガーCSVの読み込み
	LoadTutorialTriggerCSV(kTutorialTriggerCSVPath);
	LoadStageChangeTriggerCSV(kTutorialStageChangeTriggerCSVPath);
	tutorial_manager_.Initialize();

	// チュートリアルトリガー生成
	BuildTutorialTriggersFromCSV();

	//背景画像の初期化
	back_ground_.Initialize();

	//カメラを初期化
	camera_.Initialize(GameInfo::GetInstance()->GetResolutionX(),
				       GameInfo::GetInstance()->GetResolutionY());

	//ワールドマップの範囲(タイルマップの幅×タイル幅)をセット
	const float world_width = static_cast<float>(tile_map_.GetWidth() * tile_set_.GetTileWidth());
	const float world_height = static_cast<float>(tile_map_.GetHeight() * tile_set_.GetTileHeight());
	camera_.SetWorldSize(world_width, world_height);


	//プレイヤーの初期化
	player_ = new Player();
	player_->SetGameLevel(this);
	player_->Initialize();

	//タスクマネージャーに追加
	TaskManager::GetInstance()->AddTask(player_);

	SpawnPlayerOnGround(kSpawnColumn);

	//クナイマネージャーにプレイヤーの情報を与える
	kunai_manager_->SetPlayer(player_);

	//クナイマネージャーにカメラの情報を渡す
	kunai_manager_->SetCamera(&camera_);

	//プレイヤーにクナイマネージャーを渡す
	player_->SetKunaiManager(kunai_manager_);

	//HPバーの初期化
	hp_bar_ = new HpBar();
	const int player_max_hp = player_->GetMaxHp();
	hp_bar_->Initialize(player_max_hp);
	TaskManager::GetInstance()->AddTask(hp_bar_);

	// プレビューSEをAudioManagerに登録
	// SE音量調整時にこのSEで音量を確認できる
	AudioManager::GetInstance()->SetPreviewSeHandle(
		player_->GetLongRangeShotSeHandle()
	);
	//エネミーマネージャーをnew
	enemy_manager_ = new EnemyManager();

	//ゲームレベルをエネミーマネージャーにセット
	enemy_manager_->SetGameLevel(this);

	//エネミーマネージャーにプレイヤーのポインタを教える
	enemy_manager_->SetPlayer(player_);

	//エネミーマネージャーにクナイマネージャーのポインタを教える
	enemy_manager_->SetKunaiManager(kunai_manager_);

	//初期化処理を呼び出し
	enemy_manager_->InitializeEnemy();

	//ゲームレベルのポインタをセット
	enemy_manager_->SetGameLevel(this);

	//クナイマネージャーにエネミーマネージャーを教える
	kunai_manager_->SetEnemyManager(enemy_manager_);

	//タスクマネージャーにエネミーマネージャーを追加
	TaskManager::GetInstance()->AddTask(enemy_manager_);


	//エフェクトマネージャーをnew
	effect_manager_ = new EffectManager();

	//エフェクトの初期化
	effect_manager_->InitializeEffect();

	//エフェクトマネージャーのポインタをエネミーマネージャーに渡す
	enemy_manager_->SetEffectManager(effect_manager_);

	//エフェクトマネージャーのポインタをプレイヤーインスタンスに渡す
	player_->SetEffectManager(effect_manager_);

	/// エフェクトマネージャーにカメラの情報を渡す
	effect_manager_->SetCamera(&camera_);

	//タスクマネージャーにエフェクトマネージャーを追加
	TaskManager::GetInstance()->AddTask(effect_manager_);

	//ポーズマネージャーをnew
	pause_manager_ = new PauseManager();

	pause_manager_->Initialize();

	//入力をポーズマネージャーへ渡す
	pause_manager_->SetInput(InputManager::GetInstance());

	//タスクマネージャーにポーズマネージャーをセット
	TaskManager::GetInstance()->SetPauseManager(pause_manager_);

	//敵スポーン情報CSVの読み込み
	enemy_spawn_map_.LoadFromCSV(kTutorialMapEnemySpawnCSVPath);

	// 敵生成
	if (enemy_manager_ != nullptr)
	{
		enemy_manager_->SpawnEnemiesFromMap(
			enemy_spawn_map_,
			tile_set_,
			this,
			effect_manager_,
			kunai_manager_);
	}

	//ダイアログマネージャーを初期化
	dialogue_manager_.Initialize();

	// SEロード
	boss_defeat_se_handle_ = AudioManager::GetInstance()->LoadSe(kBossDefeatSePath);
	game_clear_bgm_handle_ = AudioManager::GetInstance()->LoadBgm(kGameClearBgmPath);

	// チュートリアルBGM再生
	bgm_handle_ = AudioManager::GetInstance()->LoadBgm(kTutorialBgmPath);
	AudioManager::GetInstance()->PlayBgm(bgm_handle_);
}

/// <summary>
/// 管理するものをタスクマネージャーから解放する
/// </summary>
void GameLevel::ReleaseLevel(TaskId task_id)
{
}

/// <summary>
/// ゲームレベルの管理するものを破棄
/// </summary>
void GameLevel::DestroyLevel()
{
	AudioManager::GetInstance()->StopBgm(bgm_handle_);
	AudioManager::GetInstance()->DeleteBgm(bgm_handle_);
	AudioManager::GetInstance()->DeleteSe(boss_defeat_se_handle_);
	AudioManager::GetInstance()->StopBgm(game_clear_bgm_handle_);
	AudioManager::GetInstance()->DeleteBgm(game_clear_bgm_handle_);

	tutorial_manager_.Finalize();

	dialogue_manager_.Finalize();

	FadeManager::GetInstance()->DeleteGraphics();

	pause_manager_->Finalize();
	delete pause_manager_;
	pause_manager_ = nullptr;

	TaskManager::GetInstance()->SetPauseManager(nullptr);

	if (game_ui_ != nullptr)
	{
		game_ui_->DestroyGraphics();
		delete game_ui_;
		game_ui_ = nullptr;
	}


	//playerクラスについてインスタンスに情報が入っていれば
	if (player_ != nullptr)
	{
		//PlayerクラスのインスタンスDelete
		TaskManager::GetInstance()->ReleaseTask(player_->GetTaskId());

		//Playerクラスの後処理
		player_ = nullptr;

	}

	//HpBarクラスについて
	if (hp_bar_ != nullptr)
	{
		TaskManager::GetInstance()->ReleaseTask(hp_bar_->GetTaskId());

		hp_bar_ = nullptr;
	}

	//KunaiManagerクラスについて
	if (kunai_manager_ != nullptr)
	{
		//クナイの後処理を呼び出す
		TaskManager::GetInstance()->ReleaseTask(kunai_manager_->GetTaskId());

		//kunai_managerクラスの後処理
		kunai_manager_ = nullptr;
	}

	//EnemyManagerについて
	if (enemy_manager_ != nullptr)
	{
		TaskManager::GetInstance()->ReleaseTask(enemy_manager_->GetTaskId());

		//後処理
		enemy_manager_ = nullptr;
	}

	//effect_manager_について
	if (effect_manager_ != nullptr)
	{
		TaskManager::GetInstance()->ReleaseTask(effect_manager_->GetTaskId());

		//後処理
		effect_manager_ = nullptr;
	}

}

/// <summary>
/// 破棄前処理
/// </summary>
void GameLevel::OnDestroy()
{
	DestroyLevel();
}

/// <summary>
/// ワールドマップの幅を計算して返す
/// </summary>
float GameLevel::GetWorldWidth() const
{
	return static_cast<float>(tile_map_.GetWidth() * tile_set_.GetTileWidth());
}

/// <summary>
/// ワールドマップの高さを計算して返す
/// </summary>
float GameLevel::GetWorldHeight() const
{
	return static_cast<float>(tile_map_.GetHeight() * tile_set_.GetTileHeight());
}

/// <summary>
/// スクロール処理
/// </summary>
/// <param name="bg_pos">画像の位置</param>
/// <param name="scroll_speed">スクロールスピード</param>
/// <returns>スクロール後の画像の位置</returns>
int GameLevel::UpdateScroll(int bg_pos, int scroll_speed)
{
	int tmp_y = bg_pos + scroll_speed;

	//tmp_yに最低値: 0 最大値: GetResolutionYに補正
	tmp_y = std::clamp(tmp_y, 0, GameInfo::GetInstance()->GetResolutionY());

	bg_pos = tmp_y;

	return bg_pos;
}

/// <summary>
/// 背景スクロール描画処理
/// </summary>
/// <param name="start_pos_x">開始位置X</param>
/// <param name="start_pos_y">開始位置Y</param>
/// <param name="image_handle">画像のハンドル</param>
void GameLevel::RenderScroll(int start_pos_x, int start_pos_y, int image_handle)
{
	//画像1つめ。
	DrawGraph(start_pos_x, start_pos_y, image_handle, TRUE);

	//同じ画像2つめ。画面外から画面内に流れてくる
	DrawGraph(start_pos_x, start_pos_y - GameInfo::GetInstance()->GetResolutionY(), image_handle, TRUE);
}

/// <summary>
/// ゲームオーバーにする
/// </summary>
void GameLevel::OnGameOver()
{
	if (is_game_over_)
	{
		return;
	}

	is_game_over_ = true;

	if (game_ui_ != nullptr)
	{
		game_ui_->SetStateGameOver();
	}

	if (pause_manager_ != nullptr)
	{
		pause_manager_->SetHintEnabled(false);
	}
}

/// <summary>
/// ゲームクリアにする
/// </summary>
void GameLevel::OnGameClear()
{
	if (is_game_clear_)
	{
		return;
	}

	is_game_clear_ = true;

	//BGMを止めてSEを鳴らす
	AudioManager::GetInstance()->StopBgm(bgm_handle_);
	AudioManager::GetInstance()->PlaySe(boss_defeat_se_handle_);
	AudioManager::GetInstance()->PlayBgm(game_clear_bgm_handle_);

	if (game_ui_ != nullptr)
	{
		//どのボスをクリアしたかをUIに伝える
		if (current_stage_id_ == kEnhancedBossMapID)
		{
			game_ui_->SetClearMessageKind(GameUI::ClearMessageKind::kEnhancedBoss);
		}
		else
		{
			game_ui_->SetClearMessageKind(GameUI::ClearMessageKind::kNormalBoss);
		}

		game_ui_->SetStateGameClear();
	}

	if (pause_manager_ != nullptr)
	{
		pause_manager_->SetHintEnabled(false);
	}
}

/// <summary>
/// スポーンする座標を計算する
/// </summary>
bool GameLevel::FindSpawnPositionOnGround(int spawn_tile_x, float& out_spawn_x, float& out_spawn_y) const
{
	//player_ が無いと高さが取れないので失敗扱い
	if (player_ == nullptr)
	{
		out_spawn_x = 0.0f;
		out_spawn_y = 0.0f;
		return false;
	}

	//タイルの大きさ、マップの高さを取得
	const int tile_size = tile_map_.GetTileSize();
	const int map_h = tile_map_.GetHeight();

	//プレイヤーの高さを取得
	const int player_h = player_->GetPlayerImageHeight();

	//player_h が 0 の場合は十分に大きな値を使うためにタイルサイズ＊１０とする
	const int safe_player_h = (player_h > 0)
		? player_h
		: tile_map_.GetTileSize() * kTenTimes;

	//スポーン先のx,y座標を求める(y座標は宣言のみ)
	out_spawn_x = static_cast<float>(spawn_tile_x * tile_size);
	out_spawn_y = 0.0f;

	bool found = false;

	//下から上に向かって、最初に見つかった solid を地面にする
	for (int y = map_h - 1; y >= 0; --y)
	{
		const int id = tile_map_.GetTileId(spawn_tile_x, y);
		const TileDef& def = tile_database_.GetDef(id);

		if (def.is_solid)
		{
			//地面タイルの上端を基準に画面1枚分上にスポーン
			const float ground_top_y = static_cast<float>(y * tile_size);
			out_spawn_y = ground_top_y
				- static_cast<float>(safe_player_h)
				- static_cast<float>(GameInfo::GetInstance()->GetResolutionY());
			found = true;
			break;
		}
	}


	if (!found)
	{
		out_spawn_y = static_cast<float>(GameInfo::GetInstance()->GetResolutionY() - (safe_player_h * kTwoTimes));
	}

	return found;
}

/// <summary>
/// 敵を床の上にスポーンさせる座標を計算する
/// </summary>
bool GameLevel::FindEnemySpawnPositionOnGround(
	int spawn_tile_x,
	int spawn_tile_y,
	int enemy_width,
	int enemy_height,
	float enemy_scale,
	float& out_spawn_x,
	float& out_spawn_y) const
{
	const int tile_size = tile_map_.GetTileSize();
	const int map_width = tile_map_.GetWidth();
	const int map_height = tile_map_.GetHeight();

	if (spawn_tile_x < 0 || spawn_tile_x >= map_width)
	{
		out_spawn_x = 0.0f;
		out_spawn_y = 0.0f;
		return false;
	}

	if (spawn_tile_y < 0)
	{
		spawn_tile_y = 0;
	}
	if (spawn_tile_y >= map_height)
	{
		spawn_tile_y = map_height - 1;
	}

	out_spawn_x = static_cast<float>(spawn_tile_x * tile_size);

	const float scaled_enemy_height = static_cast<float>(enemy_height) * enemy_scale;

	for (int y = spawn_tile_y; y < map_height; ++y)
	{
		const int tile_id = tile_map_.GetTileId(spawn_tile_x, y);
		const TileDef& tile_def = tile_database_.GetDef(tile_id);

		if (!tile_def.is_solid)
		{
			continue;
		}

		const float ground_top_y = static_cast<float>(y * tile_size);

		//敵の足元が床に乗る位置にする
		out_spawn_y = ground_top_y - scaled_enemy_height;
		return true;
	}

	//床が見つからなかったとき
	out_spawn_y = static_cast<float>(spawn_tile_y * tile_size);
	return false;
}

/// <summary>
/// プレイヤーをスポーンさせる
/// </summary>
void GameLevel::SpawnPlayerOnGround(int spawn_tile_x)
{
	if (player_ == nullptr)
	{
		return;
	}

	float spawn_x = 0.0f;
	float spawn_y = 0.0f;

	FindSpawnPositionOnGround(spawn_tile_x, spawn_x, spawn_y);

	player_->SetPosition(spawn_x, spawn_y);

	//スポーン時は必ず落下状態
	player_->SetFallingState();

}

/// <summary>
/// チュートリアルの領域を踏んでいるか確認する
/// </summary>
void GameLevel::CheckTutorialTriggers()
{
	if (player_ == nullptr)
	{
		tutorial_manager_.HideTutorial();
		return;
	}

	const float player_x = player_->GetPlayerPositionX();
	const float player_y = player_->GetPlayerPositionY();
	const float player_width = static_cast<float>(player_->GetPlayerImageWidth());
	const float player_height = static_cast<float>(player_->GetPlayerImageHeight());

	// 足元だけで判定する
	const float foot_x = player_x + player_width * kQuarter;
	const float foot_y = player_y + player_height - kFootOffsetY;
	const float foot_width = player_width * kHalf;
	const float foot_height = kFootOffsetY;

	bool is_hit_any_trigger = false;

	for (std::size_t i = 0; i < tutorial_trigger_list_.size(); ++i)
	{
		const TutorialManager::TutorialTrigger& trigger = tutorial_trigger_list_[i];

		const bool is_hit = IsRectHit(
			foot_x, foot_y, foot_width, foot_height,
			trigger.x, trigger.y, trigger.width, trigger.height);

		if (!is_hit)
		{
			continue;
		}

		tutorial_manager_.ShowTutorial(trigger.type);
		is_hit_any_trigger = true;
		break;
	}

	if (!is_hit_any_trigger)
	{
		tutorial_manager_.HideTutorial();
	}
}

/// <summary>
/// ステージチェンジの領域を踏んでいるか確認する
/// </summary>
void GameLevel::CheckStageChangeTrigger()
{
	if (player_ == nullptr)
	{
		return;
	}

	const float foot_x = player_->GetPlayerPositionX() + player_->GetPlayerImageWidth() * kHalf;
	const float foot_y = player_->GetPlayerPositionY() + player_->GetPlayerImageHeight() - 1.0f;

	const int tile_x = static_cast<int>(foot_x / kTileSize);
	const int tile_y = static_cast<int>(foot_y / kTileSize);

	// 範囲外チェック
	if (tile_y < 0 || tile_y >= static_cast<int>(stage_change_trigger_id_map_.size()))
	{
		return;
	}

	if (tile_x < 0 || tile_x >= static_cast<int>(stage_change_trigger_id_map_[tile_y].size()))
	{
		return;
	}

	const int trigger_id = stage_change_trigger_id_map_[tile_y][tile_x];

	if (trigger_id == kStage01TileID)
	{
		StartStageChange(kStage01MapID, kSpawnColumn);
	}
	else if (trigger_id == kNormalBossTileID)
	{
		StartStageChange(kNormalBossMapID, kSpawnColumn);
	}
	else if (trigger_id == kEnhancedBossTileID)
	{
		StartStageChange(kEnhancedBossMapID, kSpawnColumn);
	}
}

/// <summary>
/// チュートリアルトリガーCSVを読み込む
/// </summary>
bool GameLevel::LoadTutorialTriggerCSV(const char* file_path)
{
	tutorial_trigger_id_map_.clear();

	std::ifstream ifs(file_path);
	if (!ifs.is_open())
	{
		return false;
	}

	std::string line;
	while (std::getline(ifs, line))
	{
		std::vector<int> row;
		std::stringstream ss(line);
		std::string cell;

		while (std::getline(ss, cell, ','))
		{
			row.push_back(std::stoi(cell));
		}

		tutorial_trigger_id_map_.push_back(row);
	}

	return true;
}

/// <summary>
/// ステージチェンジCSVを読み込む
/// </summary>
bool GameLevel::LoadStageChangeTriggerCSV(const char* file_path)
{
	stage_change_trigger_id_map_.clear();

	std::ifstream ifs(file_path);
	if (!ifs.is_open())
	{
		return false;
	}

	std::string line;
	while (std::getline(ifs, line))
	{
		std::vector<int> row;
		std::stringstream ss(line);
		std::string cell;

		while (std::getline(ss, cell, ','))
		{
			row.push_back(std::stoi(cell));
		}

		stage_change_trigger_id_map_.push_back(row);
	}

	return true;
}

/// <summary>
/// チュートリアルIDをタイプへ変換する
/// </summary>
TutorialManager::TutorialType GameLevel::ConvertTutorialTypeFromId(int id) const
{
	switch (id)
	{
	case 1:
		return TutorialManager::TutorialType::kMove;

	case 2:
		return TutorialManager::TutorialType::kJump;

	case 3:
		return TutorialManager::TutorialType::kDash;

	case 4:
		return TutorialManager::TutorialType::kCloseAttack;

	case 5:
		return TutorialManager::TutorialType::kLongRangeAttack;

	case 6:
		return TutorialManager::TutorialType::kHiddenHint;

	case 7:
		return TutorialManager::TutorialType::kBossHint;

	default:
		return TutorialManager::TutorialType::kNone;
	}
}

/// <summary>
/// 読み込んだCSVからチュートリアルトリガーを作る
/// </summary>
void GameLevel::BuildTutorialTriggersFromCSV()
{
	tutorial_trigger_list_.clear();

	const int map_height = static_cast<int>(tutorial_trigger_id_map_.size());

	for (int y = 0; y < map_height; ++y)
	{
		const int map_width = static_cast<int>(tutorial_trigger_id_map_[y].size());

		for (int x = 0; x < map_width; ++x)
		{
			const int id = tutorial_trigger_id_map_[y][x];
			const TutorialManager::TutorialType type = ConvertTutorialTypeFromId(id);

			if (type == TutorialManager::TutorialType::kNone)
			{
				continue;
			}

			TutorialManager::TutorialTrigger trigger;
			trigger.x = static_cast<float>(x * kTileSize);
			trigger.y = static_cast<float>(y * kTileSize);
			trigger.width = static_cast<float>(kTileSize);
			trigger.height = static_cast<float>(kTileSize);
			trigger.type = type;
			trigger.is_triggered_once = false;

			tutorial_trigger_list_.push_back(trigger);
		}
	}
}

/// <summary>
/// ステージチェンジを始める
/// </summary>
void GameLevel::StartStageChange(int next_stage_id, int spawn_column)
{
	if (is_stage_changing_)
	{
		return;
	}

	is_stage_changing_ = true;
	stage_change_state_ = StageChangeState::kFadeOut;

	next_stage_id_ = next_stage_id;
	next_spawn_column_ = spawn_column;

	FadeManager::GetInstance()->StartFadeOut(kFadeOutTime);
}

/// <summary>
/// 切り替え状態の更新
/// </summary>
void GameLevel::UpdateStageChange()
{
	if (!is_stage_changing_)
	{
		return;
	}

	switch (stage_change_state_)
	{
	case StageChangeState::kFadeOut:
		if (FadeManager::GetInstance()->IsFadeFinished())
		{
			stage_change_state_ = StageChangeState::kChanging;
		}
		break;

	case StageChangeState::kChanging:
		ChangeStage(next_stage_id_);
		stage_change_state_ = StageChangeState::kWait;
		break;

	case StageChangeState::kWait:
		FadeManager::GetInstance()->StartFadeIn(kStageChangeFadeInTime);
		stage_change_state_ = StageChangeState::kFadeIn;
		break;

	case StageChangeState::kFadeIn:
		if (FadeManager::GetInstance()->IsFadeFinished())
		{
			stage_change_state_ = StageChangeState::kNone;
			is_stage_changing_ = false;
		}
		break;

	case StageChangeState::kNone:
	default:
		break;
	}
}

/// <summary>
/// 実際のステージ変更処理
/// </summary>
void GameLevel::ChangeStage(int stage_id)
{
	current_stage_id_ = stage_id;

	const char* map_csv_path = nullptr;
	const char* enemy_spawn_csv_path = nullptr;
	const char* stage_change_trigger_csv_path = nullptr;
	const char* tutorial_trigger_csv_path = nullptr;

	switch (stage_id)
	{
	case 101:
		map_csv_path = kStage01MapCSVPath;
		enemy_spawn_csv_path = kStage01MapEnemySpawnCSVPath;
		stage_change_trigger_csv_path = kStage01StageChangeTriggerCSVPath;
		tutorial_trigger_csv_path = kStage01TutorialTriggerCSVPath;
		break;

	case 102: //ノーマルボス
		map_csv_path = kNormalBossMapCSVPath;
		enemy_spawn_csv_path = kNormalBossMapEnemySpawnCSVPath;
		stage_change_trigger_csv_path = nullptr;
		tutorial_trigger_csv_path = nullptr;

		if (enemy_manager_ != nullptr)
		{
			enemy_manager_->SetEnhancedBoss(false);
		}

		break;

	case 103: //強化ボス
		map_csv_path = kEnhancedBossMapCSVPath;
		enemy_spawn_csv_path = kEnhancedBossMapEnemySpawnCSVPath;
		stage_change_trigger_csv_path = nullptr;
		tutorial_trigger_csv_path = nullptr;

		if (enemy_manager_ != nullptr)
		{
			enemy_manager_->SetEnhancedBoss(true);
		}
		break;

	default:
		return;
	}

	if (enemy_manager_ != nullptr)
	{
		enemy_manager_->DeleteAllEnemy();
	}

	// 既存のBGMを止めて切り替え
	AudioManager::GetInstance()->StopBgm(bgm_handle_);
	AudioManager::GetInstance()->DeleteBgm(bgm_handle_);

	if (stage_id == kStage01MapID)
	{
		bgm_handle_ = AudioManager::GetInstance()->LoadBgm(kStage01BgmPath);
	}
	else if (stage_id == kNormalBossMapID)
	{
		bgm_handle_ = AudioManager::GetInstance()->LoadBgm(kNormalBossBgmPath);
	}
	else if (stage_id == kEnhancedBossMapID)
	{
		bgm_handle_ = AudioManager::GetInstance()->LoadBgm(kEnhancedBossBgmPath);
	}

	AudioManager::GetInstance()->PlayBgm(bgm_handle_);

	tutorial_trigger_list_.clear();
	tutorial_trigger_id_map_.clear();
	tutorial_manager_.HideTutorial();

	stage_change_trigger_id_map_.clear();

	if (stage_change_trigger_csv_path != nullptr)
	{
		LoadStageChangeTriggerCSV(stage_change_trigger_csv_path);
	}

	if (tutorial_trigger_csv_path != nullptr)
	{
		LoadTutorialTriggerCSV(tutorial_trigger_csv_path);
		BuildTutorialTriggersFromCSV();
	}

	//ボスステージ入場時に会話を開始する
	if (stage_id == kNormalBossMapID)
	{
		pending_dialogue_data_ = kNormalBossDialogue;
		is_waiting_dialogue_on_land_ = true;
	}
	else if (stage_id == kEnhancedBossMapID)
	{
		pending_dialogue_data_ = kEnhancedBossDialogue;
		is_waiting_dialogue_on_land_ = true;
	}

	tile_map_.SetTileSize(kTileSize);
	tile_map_.LoadFromCSV(map_csv_path);
	tile_map_.InitializeTileHp(tile_database_);

	const float world_width = static_cast<float>(tile_map_.GetWidth() * tile_set_.GetTileWidth());
	const float world_height = static_cast<float>(tile_map_.GetHeight() * tile_set_.GetTileHeight());
	camera_.SetWorldSize(world_width, world_height);

	// 新しいマップを読み込んだあとでスポーン
	SpawnPlayerOnGround(next_spawn_column_);

	if (player_ != nullptr)
	{
		const float player_center_x = player_->GetPlayerPositionX() + player_->GetPlayerImageWidth() * kHalf;
		const float player_center_y = player_->GetPlayerPositionY() + player_->GetPlayerImageHeight() * kHalf;

		camera_.SnapToTarget(player_center_x, player_center_y);

		camera_x_ = static_cast<int>(camera_.GetCameraLeftX());
		camera_y_ = static_cast<int>(camera_.GetCameraTopY());

		//Update()がスキップされているため描画座標を強制更新する
		player_->ForceUpdateScreenPosition(
			static_cast<float>(camera_x_),
			static_cast<float>(camera_y_));
	}

	if (enemy_spawn_csv_path != nullptr && enemy_manager_ != nullptr)
	{
		enemy_spawn_map_.LoadFromCSV(enemy_spawn_csv_path);

		enemy_manager_->SpawnEnemiesFromMap(
			enemy_spawn_map_,
			tile_set_,
			this,
			effect_manager_,
			kunai_manager_);
	}
}