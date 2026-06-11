#include "LevelBase.h"

/// <summary>
/// コンストラクタ
/// </summary>
LevelBase::LevelBase()
{
}

/// <summary>
/// デストラクタ
/// </summary>
LevelBase::~LevelBase()
{
}


/// <summary>
/// 毎フレームの更新処理
/// 派生先のレベルでオーバーライドする
/// </summary>
/// <param name="delta_time">前回実行フレームからの経過時間（秒）</param>
void LevelBase::Update(float delta_time)
{
}

/// <summary>
/// 毎フレームの描画処理
/// 派生先のレベルでオーバーライドする
/// </summary>
void LevelBase::Render()
{
}