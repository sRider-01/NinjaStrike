#include "LevelChanger.h"
#include "TitleLevel.h"
#include "GameLevel.h"
#include "Task\TaskManager.h"

/// <summary>
/// レベルチェンジャーのインスタンス初期化
/// </summary>
LevelChanger* LevelChanger::instance_ = nullptr;
/// <summary>
/// コンストラクタ
/// </summary>
LevelChanger::LevelChanger()
	: current_level_changer_state_(LevelChangerState::kWait)
	, current_level_(nullptr)
	, next_level_changer_state_after_destroy_(LevelChangerState::kWait)
{
}

/// <summary>
/// デストラクタ
/// </summary>
LevelChanger::~LevelChanger()
{
}

/// <summary>
/// レベルチェンジャーの状態を設定する
/// </summary>
/// <param name="level_changer_state">設定する状態</param>
void LevelChanger::SetLevelChangerState(LevelChanger::LevelChangerState level_changer_state)
{
	current_level_changer_state_ = level_changer_state;
}

/// <summary>
/// レベルチェンジャーの状態を返す
/// </summary>
LevelChanger::LevelChangerState LevelChanger::GetLevelChangerState() const
{
	return current_level_changer_state_;
}

/// <summary>
/// 毎フレームの更新処理
/// </summary>
/// <param name="delta_time">前回実行フレームからの経過時間（秒）</param>
void LevelChanger::Update(float delta_time)
{
	//新しくレベルを生成したフラグ
	bool is_created_level = false;

	//レベル解放フラグ
	bool is_release_level = false;

	//タイトルレベルの初期化
	if (current_level_changer_state_ == LevelChangerState::kInitTitleLevel)
	{
		//タイトルレベルを生成
		current_level_ = new TitleLevel();
		//新しくレベルを生成したフラグをtrue
		is_created_level = true;

		current_level_kind_ = CurrentLevelKind::kTitle;
	}
	//タイトルレベルの後処理
	else if (current_level_changer_state_ == LevelChangerState::kFinTitleLevel)
	{
		//次はゲームレベルの初期化へ
		next_level_changer_state_after_destroy_ = LevelChangerState::kInitGameLevel;
		//レベル解放フラグをtrue
		is_release_level = true;

	}
	//ゲームレベルの初期化
	else if (current_level_changer_state_ == LevelChangerState::kInitGameLevel)
	{
		//ゲームレベルを生成
		current_level_ = new GameLevel();

		//新しくレベルを生成したフラグをtrue
		is_created_level = true;

		current_level_kind_ = CurrentLevelKind::kBattle;
	}
	//ゲームオーバー画面表示
	else if (current_level_changer_state_ == LevelChangerState::kFinGameLevelToRestart)
	{
		// 次はゲームレベルを再生成する
		next_level_changer_state_after_destroy_ = LevelChangerState::kRestartGameLevel;
		is_release_level = true;
	}
	//ゲームレベルの再生成
	else if (current_level_changer_state_ == LevelChangerState::kRestartGameLevel)
	{
		// タイトルを経由せずゲームレベルを再生成
		current_level_ = new GameLevel();
		is_created_level = true;
		current_level_kind_ = CurrentLevelKind::kBattle;
	}
	//ゲームレベルの後処理
	else if (current_level_changer_state_ == LevelChangerState::kFinGameLevel)
	{
		//次はタイトルの初期化へ
		next_level_changer_state_after_destroy_ = LevelChangerState::kInitTitleLevel;
		//レベル解放フラグをtrue
		is_release_level = true;
	}
	//現在レベルの破棄
	else if (current_level_changer_state_ == LevelChangerState::kDestroyCurrentLevel)
	{

		current_level_ = nullptr;
		//レベル破棄後に予約された状態へ変える
		current_level_changer_state_ = next_level_changer_state_after_destroy_;
	}

	//新しくレベルが生成されたなら
	if (is_created_level)
	{
		//生成したレベルをタスクに登録
		TaskManager::GetInstance()->AddTask(current_level_);

		//レベルの開始処理
		current_level_->BeginLevel();

		//レベルチェンジャーの状態は待機へ
		current_level_changer_state_ = LevelChangerState::kWait;
	}

	//現在のレベルは解放するなら
	if (is_release_level)
	{
		//現在レベルに関連するものをタスクマネージャーから解放する
		current_level_->ReleaseLevel(current_level_->GetTaskId());

		//現在レベルをタスクから解放
		TaskManager::GetInstance()->ReleaseTask(current_level_->GetTaskId());

		//レベルチェンジャーの状態は現在のレベル破棄へ
		current_level_changer_state_ = LevelChangerState::kDestroyCurrentLevel;
	}
}

/// <summary>
/// 現在のレベルを強制破棄
/// ウィンドウの×ボタンでゲームを終了した場合などの対応
/// </summary>
void LevelChanger::ForceDestroyCurrentLevel()
{
	//現在のレベルが破棄されてないなら
	if (current_level_ != nullptr) {

		//TaskManagerに解放（deleteはTaskManager側）
		TaskManager::GetInstance()->ReleaseTask(current_level_->GetTaskId());

		current_level_ = nullptr;
	}
}