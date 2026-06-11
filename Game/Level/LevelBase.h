#pragma once
#include "Task\Task.h"

/// <summary>
/// レベルの基本クラス
/// 各レベルはLevelBaseを派生して定義する
/// </summary>
class LevelBase : public Task
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	explicit LevelBase();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~LevelBase();


	/// <summary>
	/// レベルの開始処理
	/// </summary>
	virtual void BeginLevel() = 0;

	/// <summary>
	/// レベル関連をタスクマネージャーから解放する
	/// </summary>
	virtual void ReleaseLevel(TaskId task_id) = 0;

	/// <summary>
	/// レベル関連を破棄
	/// </summary>
	virtual void DestroyLevel() = 0;

	/// <summary>
	/// 毎フレームの更新処理
	/// 派生先のレベルでオーバーライドする
	/// </summary>
	/// <param name="delta_time">前回実行フレームからの経過時間（秒）</param>
	void Update(float delta_time) override;

	/// <summary>
	/// 毎フレームの描画処理
	/// 派生先のレベルでオーバーライドする
	/// </summary>
	void Render() override;
};
