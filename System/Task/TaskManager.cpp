#include "Pause/PauseManager.h"
#include "TaskManager.h"
#include "Task.h"

//タスクマネージャーのインスタンス初期化
TaskManager* TaskManager::instance_ = nullptr;

/// <summary>
/// 割り当てるタスクIDの最初の数値
/// </summary>
const TaskId TaskManager::kStartTaskId = 100;

/// <summary>
/// コンストラクタ
/// </summary>
TaskManager::TaskManager()
	:task_list_()
	, add_task_list_()
	, release_task_list_()
	, last_assigned_task_id_(0)
{
}

/// <summary>
/// デストラクタ
/// </summary>
TaskManager::~TaskManager()
{
}

/// <summary>
/// タスクを追加する
/// </summary>
/// <param name="task">追加するタスク</param>
/// <returns>発行したTaskId</returns>
TaskId TaskManager::AddTask(Task* task)
{
	//追加するタスクがnullptrなら
	if (task == nullptr) {

		//失敗
		return -1;

	}
	//初めてのTaskId発行なら
	if (last_assigned_task_id_ == 0)
	{
		//新しいTaskIdの発行
		last_assigned_task_id_ = kStartTaskId;

	}

	//前回割り振ったIDに＋１して発行(した値をassign_task_idに入れる)
	TaskId assign_task_id = last_assigned_task_id_ + 1;

	//最新値更新
	last_assigned_task_id_ = assign_task_id;

	//生成したTaskIdを引数のタスクにSetTaskIdで設定
	task->SetTaskId(assign_task_id);

	//追加リストにタスクを追加する
	add_task_list_.emplace_back(task);

	//assign_task_idを返す
	return assign_task_id;
}

/// <summary>
/// タスクを解放する
/// </summary>
/// <param name="task_id">タスクID</param>
/// <returns>成功：解放したタスク、失敗：nullptr</returns>
Task* TaskManager::ReleaseTask(TaskId task_id)
{
	//目的のタスクを格納する
	Task* lelease_task = nullptr;

	//引数のTaskIdを見つける
	for (std::vector<Task*>::iterator ite = task_list_.begin(); ite != task_list_.end(); ite++)
	{
		//nulptrなら
		if ((*ite) == nullptr)
		{
			//何もせずスキップ
			continue;
		}

		//TaskIdを見つける iteが指すIdがTaskIdか？
		if (task_id == (*ite)->GetTaskId())
		{
			//解放するタスクとしてlelease_taskに追加
			lelease_task = (*ite);
			break;
		}
	}

	//見つからなかったら
	if (lelease_task == nullptr) {
		//終了
		return nullptr;
	}

	//すでにlelease済なら
	if (lelease_task->IsRelease() == true)
	{
		//lelease_taskを返す
		return lelease_task;
	}

	//見つけたタスクの解放フラグをtrueに
	lelease_task->SetRelease();

	//解放リストにタスクを追加する
	release_task_list_.emplace_back(lelease_task->GetTaskId());

	//lelease_taskを返す
	return lelease_task;

}

/// <summary>
/// タスクの毎フレーム更新処理を実行する
/// </summary>
/// <param name="delta_time">最後のフレームを完了するのに要した時間 (秒) </param>
void TaskManager::UpdateTask(float delta_time)
{
	//PauseManagerのUpdateはどのTaskよりも先に実施
	if (pause_manager_ != nullptr)
	{
		pause_manager_->Update(delta_time);
	}

	const bool is_paused = (pause_manager_ != nullptr) && pause_manager_->IsPaused();

	//task_list_のすべてのタスクにupdateを実施
	for (std::vector<Task*>::iterator ite = task_list_.begin(); ite != task_list_.end(); ite++)
	{
		//nulptrなら
		if ((*ite) == nullptr)
		{
			//何もせずスキップ
			continue;
		}


		//IsReleaseがtrueなら
		if ((*ite)->IsRelease() == true)
		{
			//何もせず終了
			continue;
		}

		//ポーズ中かつポーズ中更新がfalseなら
		if (is_paused && !(*ite)->CanUpdateWhilePaused())
		{
			//何もせず終了
			continue;
		}

		//Updateを呼び出す
		(*ite)->Update(delta_time);
	}

	//add_task_list_にあるタスクをtask_listに追加
	for (std::vector<Task*>::iterator ite = add_task_list_.begin(); ite != add_task_list_.end(); ite++)
	{
		//*iteをtask_list_の末尾に追加
		task_list_.emplace_back(*ite);
	}

	//add_task_list_のなかを全消し
	add_task_list_.clear();

	//release_task_list_にあるタスクをtask_listから解放
	while (!release_task_list_.empty())
	{
		//いま溜まっている解放IDを退避（これで release_task_list_ は空になる）
		std::vector<TaskId> release_ids;
		release_ids.swap(release_task_list_);

		for (std::size_t i = 0; i < release_ids.size(); ++i)
		{
			TaskId target_id = release_ids[i];

			for (std::vector<Task*>::iterator ite_target = task_list_.begin();
				ite_target != task_list_.end(); )
			{
				if (*ite_target == nullptr)
				{
					ite_target = task_list_.erase(ite_target);
					continue;
				}

				if ((*ite_target)->GetTaskId() == target_id)
				{
					(*ite_target)->OnDestroy();
					delete (*ite_target);

					ite_target = task_list_.erase(ite_target);
					break;
				}
				else
				{
					++ite_target;
				}
			}
		}
	}
}


/// <summary>
/// タスクの毎フレームの描画処理を実行する
/// </summary>
void TaskManager::RenderTask()
{
	//task_list_のすべてのタスクにRenderを実施
	for (std::vector<Task*>::iterator ite = task_list_.begin(); ite != task_list_.end(); ite++)
	{
		//nulptrなら
		if ((*ite) == nullptr)
		{
			//何もせずスキップ
			continue;
		}

		//IsReleaseがtrueなら更新しない
		if ((*ite)->IsRelease() == true)
		{
			//何もせず終了
			continue;
		}

		//Renderを呼び出す
		(*ite)->Render();
	}

	if (pause_manager_ != nullptr)
	{
		pause_manager_->Render();
	}
}

/// <summary>
/// 終了処理
/// </summary>
void TaskManager::Finalize()
{
	for (std::size_t i = 0; i < add_task_list_.size(); ++i)
	{
		task_list_.push_back(add_task_list_[i]);
	}
	add_task_list_.clear();

	for (std::size_t i = 0; i < task_list_.size(); ++i)
	{
		Task* task = task_list_[i];
		if (task == nullptr) 
		{
			continue;
		}

		task->OnDestroy();
		delete task;
		task_list_[i] = nullptr;
	}
	task_list_.clear();
	release_task_list_.clear();

	if (pause_manager_ != nullptr)
	{
		delete pause_manager_;
		pause_manager_ = nullptr;
	}
}