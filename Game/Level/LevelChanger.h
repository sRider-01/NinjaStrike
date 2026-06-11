#pragma once
#include "Task\Task.h"

//前方宣言
class LevelBase;

/// <summary>
/// レベルの切り替えを管理するクラス
/// </summary>
class LevelChanger : public Task
{
private:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	explicit LevelChanger();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~LevelChanger();

public:
	/// <summary>
	/// ポーズ中でもレベル遷移は処理する必要があるため true を返す
	/// </summary>
	bool CanUpdateWhilePaused() override
	{
		return true;
	}

	/// <summary>
	/// レベルチェンジャーの状態
	/// </summary>
	enum class LevelChangerState
	{
		/// <summary>
		/// 待機
		/// </summary>
		kWait,

		/// <summary>
		/// タイトルレベルの初期化
		/// </summary>
		kInitTitleLevel,

		/// <summary>
		/// タイトルレベルの後処理
		/// </summary>
		kFinTitleLevel,

		/// <summary>
		/// ゲームレベルの初期化
		/// </summary>
		kInitGameLevel,

		/// <summary>
		/// ゲームレベルの後処理
		/// </summary>
		kFinGameLevel,

		/// <summary>
		/// 現在レベルに関連するオブジェクトの破棄
		/// </summary>
		kDestroyCurrentLevel,

		/// <summary>
		/// ゲームレベルの後処理（リトライ用）
		/// kFinGameLevel と異なり、次にゲームを再生成する
		/// </summary>
		kFinGameLevelToRestart,

		/// <summary>
		/// ゲームレベルをリトライする（タイトルを経由せず再生成）
		/// </summary>
		kRestartGameLevel,
	};

	/// <summary>
	/// 今のレベルの状態
	/// </summary>
	enum class CurrentLevelKind
	{
		/// <summary>
		/// なし
		/// </summary>
		kNone,

		/// <summary>
		/// タイトル画面
		/// </summary>
		kTitle,

		/// <summary>
		/// バトル(ゲーム)画面
		/// </summary>
		kBattle,
	};

	/// <summary>
	/// LevelChangerのインスタンスを取得する
	/// （シングルトンデザインパターン）
	/// </summary>
	/// <returns>LevelChangerのインスタンス</returns>
	static LevelChanger* GetInstance()
	{
		//生成されてないなら動的生成
		if (instance_ == nullptr)
		{
			instance_ = new LevelChanger();
		}

		//LevelChangerのインスタンスを返す
		return instance_;
	}

	/// <summary>
	/// LevelChangerのインスタンスを削除
	/// ゲーム終了時に必ず呼ぶ
	/// </summary>
	static void Destroy()
	{
		//生成済みなら削除
		if (instance_ != nullptr)
		{
			delete instance_;
			instance_ = nullptr;
		}
	}

	/// <summary>
	/// レベルチェンジャーの状態を設定する
	/// </summary>
	/// <param name="level_changer_state">設定する状態</param>
	void SetLevelChangerState(LevelChangerState level_changer_state);

	/// <summary>
	/// レベルチェンジャーの状態を返す
	/// </summary>
	/// <returns>レベルチェンジャーの状態</returns>
	LevelChangerState GetLevelChangerState() const;

	/// <summary>
	/// 毎フレームの更新処理
	/// </summary>
	/// <param name="delta_time">前回実行フレームからの経過時間（秒）</param>
	void Update(float delta_time) override;

	/// <summary>
	/// 現在のレベルを強制破棄
	/// ウィンドウの×ボタンでゲームを終了した場合などの対応
	/// </summary>
	void ForceDestroyCurrentLevel();

	/// <summary>
	/// 今のレベルの状態を返す
	/// </summary>
	/// <returns>今のレベルの状態</returns>
	CurrentLevelKind GetCurrentLevelKind()
	{
		return current_level_kind_;
	}

	/// <summary>
	/// 現在のレベルを返す
	/// </summary>
	LevelBase* GetCurrentLevel() const
	{
		return current_level_;
	}


private:

	/// <summary>
	/// 今のレベルの状態：初期値は「なし」
	/// </summary>
	CurrentLevelKind current_level_kind_ = CurrentLevelKind::kNone;

	/// <summary>
	/// LevelChangerのインスタンス
	/// </summary>
	static LevelChanger* instance_;

	/// <summary>
	/// レベルチェンジャーの状態
	/// </summary>
	LevelChangerState current_level_changer_state_;

	/// <summary>
	/// 現在のレベル
	/// </summary>
	LevelBase* current_level_;

	/// <summary>
	/// 次に消すレベル
	/// </summary>
	LevelBase* pending_destroy_level_ = nullptr;

	/// <summary>
	/// 現在のレベル破棄後のレベルチェンジャーの状態
	/// </summary>
	LevelChangerState next_level_changer_state_after_destroy_;

};