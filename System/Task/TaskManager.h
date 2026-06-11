#pragma once
#include <vector>


/// <summary>
/// int型をTaskIdという名前で再定義
/// </summary>
typedef unsigned int TaskId;

/// <summary>
/// Taskクラスの前方宣言
/// この.h内でTask*は扱えるようになる
/// </summary>
class Task;
class PauseManager;

/// <summary>
/// タスクマネージャー処理
/// </summary>
/// <remarks>
/// 各オブジェクトはタスクから派生してインスタンス化し、
/// タスクマネージャーではインスタンスを一元管理する
/// </remarks>
class TaskManager
{
private:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	TaskManager();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~TaskManager();

public:
	/// <summary>
	/// TaskManagerのインスタンスを取得する
	/// （シングルトンデザインパターン）
	/// </summary>
	/// <returns>TaskManagerのインスタンス</returns>
	static TaskManager* GetInstance()
	{
		//生成されてないなら
		if (instance_ == nullptr)
		{
			//動的生成
			instance_ = new TaskManager();
		}

		//インスタンスを返す
		return instance_;
	}

	/// <summary>
	/// TaskManagerのインスタンスを削除
	/// ゲーム終了時に必ず呼ぶ
	/// </summary>
	static void Destroy()
	{
		//instance_に情報が入っているなら
		if (instance_ != nullptr)
		{
			//instance_をDelete
			delete instance_;

			//Deleteした後のポインタをnullptrにする
			instance_ = nullptr;
		}
	}


public:

	/// <summary>
	/// タスクを追加する
	/// </summary>
	/// <param name="task">追加するタスク</param>
	/// <returns>発行したTaskId</returns>
	TaskId AddTask(Task* task);

	/// <summary>
	/// タスクを解放する
	/// </summary>
	/// <param name="task_id">タスクID</param>
	/// <returns>成功：解放したタスク、失敗：nullptr</returns>
	Task* ReleaseTask(TaskId task_id);

	/// <summary>
	/// タスクの毎フレーム更新処理を実行する
	/// </summary>
	/// <param name="delta_time">最後のフレームを完了するのに要した時間 (秒) </param>
	void UpdateTask(float delta_time);

	/// <summary>
	/// タスクの毎フレームの描画処理を実行する
	/// </summary>
	void RenderTask();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// PauseManagerをセットする
	/// </summary>
	/// <param name="pause_manager">PauseManagerのポインタ</param>
	void SetPauseManager(PauseManager* pause_manager)
	{
		pause_manager_ = pause_manager;
	}
private:

	/// <summary>
	/// TaskIDの開始番号
	/// </summary>
	static const TaskId kStartTaskId;

	/// <summary>
	/// TaskManagerのインスタンス
	/// </summary>
	static TaskManager* instance_;

	/// <summary>
	/// タスクリスト
	/// </summary>
	std::vector<Task*> task_list_;

	/// <summary>
	/// 追加タスクリスト
	/// </summary>
	std::vector<Task*> add_task_list_;

	/// <summary>
	/// 解放タスクリスト
	/// </summary>
	std::vector<TaskId> release_task_list_;

	/// <summary>
	/// 最後に割り振ったタスクID
	/// </summary>
	int last_assigned_task_id_;

	/// <summary>
	/// ポーズマネージャーのポインタ
	/// </summary>
	PauseManager* pause_manager_ = nullptr;
};