#pragma once
#include "LevelBase.h"
#include "Audio/AudioManager.h"
//前方宣言
class TitleUI;

/// <summary>
/// タイトル画面のレベル
/// </summary>
class TitleLevel : public LevelBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	explicit TitleLevel();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~TitleLevel();

public:
	/// <summary>
	/// タイトルレベルの状態
	/// </summary>
	enum class TitleLevelState
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
	/// タイトルレベルの管理するものをタスクマネージャーから解放する
	/// </summary>
	void ReleaseLevel(TaskId task_id) override;

	/// <summary>
	/// タイトルレベルの管理するものを破棄
	/// </summary>
	void DestroyLevel() override;

	/// <summary>
	/// 破棄前処理
	/// </summary>
	void OnDestroy() override;

	/// <summary>
	/// 画像の破棄
	/// </summary>
	void DestroyGraphics();

private:

	/// <summary>
	/// タイトルレベルの状態
	/// </summary>
	TitleLevelState title_level_state_;

	/// <summary>
	/// 背景画像
	/// </summary>
	int title_bg_handle_;

	/// <summary>
	/// 現在の経過時間
	/// </summary>
	float elapsed_time_;

	/// <summary>
	/// タイトルUIのポインタ
	/// </summary>
	TitleUI* title_ui_ = nullptr;

	/// <summary>
	/// BGM用のハンドル
	/// </summary>
	int audio_handle_ = 0;

	/// <summary>
	/// SE用のハンドル
	/// </summary>
	int se_handle_ = 0;

	/// <summary>
	/// SEを一回だけならす用
	/// </summary>
	bool is_first_se_ = true;

	/// <summary>
	/// 最初のフェードアウト用
	/// </summary>
	bool is_first_fade_ = true;

	/// <summary>
	/// 画面遷移準備完了か？
	/// </summary>
	bool is_change_ready_ = false;
};