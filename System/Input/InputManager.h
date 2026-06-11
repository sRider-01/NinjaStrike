#pragma once

/// <summary>
/// インプットマネージャー処理
/// </summary>
/// <remarks>
/// キーボードやパッドからの入力を管理する
/// （サンプルなのでパッドは未実装）
/// </remarks>
class InputManager
{

private:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	InputManager();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~InputManager();

public:

	/// <summary>
	/// 使用するゲームでのキーの種類
	/// </summary>
	enum class GameKeyKind
	{
		/// <summary>
		/// 決定
		/// </summary>
		kDecide,

		/// <summary>
		/// キャンセル
		/// </summary>
		kCancel,

		/// <summary>
		/// 左ボタン
		/// </summary>
		kLeft,

		/// <summary>
		/// 右ボタン
		/// </summary>
		kRight,

		/// <summary>
		/// ポーズ
		/// </summary>
		kPause,

		/// <summary>
		/// ダッシュ
		/// </summary>
		kPlayerRun,

		/// <summary>
		/// ジャンプ
		/// </summary>
		kPlayerJump,

		/// <summary>
		/// 近距離攻撃
		/// </summary>
		kPlayerCloseAttack,
		
		/// <summary>
		/// プレイヤーバレット発射
		/// </summary>
		kPlayerLongRangeAttack,

		/// <summary>
		/// 上ボタン（メニュー選択用）
		/// </summary>
		kUp,

		/// <summary>
		/// 下ボタン（メニュー選択用）
		/// </summary>
		kDown,

		/// <summary>
		/// やめるボタン（タイトルへ戻る）
		/// </summary>
		kQuit,

		/// <summary>
		/// デバッグ用接触ダメージ
		/// </summary>
		kDebugContactDamage,

		/// <summary>
		/// デバッグ用攻撃ダメージ
		/// </summary>
		kDebugAttackDamage,

		/// <summary>
		/// 総数
		/// </summary>
		kNum,
	};

public:

	/// <summary>
	/// InputManagerのインスタンスを取得する
	/// （シングルトンデザインパターン）
	/// </summary>
	/// <returns>InputManagerのインスタンス</returns>
	static InputManager* GetInstance()
	{
		//生成されてないなら
		if (instance_ == nullptr) {
			//動的生成
			instance_ = new InputManager();
		}

		//InputManagerのインスタンスを返す
		return instance_;
	}

	/// <summary>
	/// InputManagerのインスタンスを削除
	/// ゲーム終了時に必ず呼ぶ
	/// </summary>
	static void Destroy()
	{
		//生成済みなら
		if (instance_ != nullptr) {

			//Deleteする
			delete instance_;

			//instance_の先をnullptrにする
			instance_ = nullptr;
		}
	}

	/// <summary>
	/// ゲームパッドが接続されているかを返す
	/// </summary>
	/// <returns>接続されているか</returns>
	bool IsGamepadConnected() const;
public:

	/// <summary>
	/// タスクの更新の前に呼ばれる関数
	/// </summary>
	/// <param name="delta_time">前回実行フレームからの経過時間（秒）</param>
	void UpdateKeyStateAll(float delta_time);

	/// <summary>
	/// タスクの更新の後に呼ばれる関数
	/// </summary>
	/// <param name="delta_time">前回実行フレームからの経過時間（秒）</param>
	void RecordKeyStateAll(float delta_time);

	/// <summary>
	/// 引数のキーはこのフレームで押されたか
	/// </summary>
	/// <param name="key_kind">調べるキー</param>
	/// <returns>押されているならtrue</returns>
	bool IsPushThisFrame(GameKeyKind game_key_kind);

	/// <summary>
	/// 引数のキーは押されているか
	/// </summary>
	/// <param name="game_key_kind">調べるキー</param>
	/// <returns>押されているならtrue</returns>
	bool IsDown(GameKeyKind game_key_kind);


private:

	/// <summary>
	/// キー状態を格納するバッファのサイズ
	/// </summary>
	static const int kKeyBufferSize = 256;

	/// <summary>
	/// InputManagerのインスタンス
	/// </summary>
	static InputManager* instance_;

	/// <summary>
	/// 現在のフレームのキー情報
	/// </summary>
	char current_key_buffer_[kKeyBufferSize];

	/// <summary>
	/// 前回フレームのキー情報
	/// </summary>
	char pre_key_buffer_[kKeyBufferSize];

	/// <summary>
	/// 現在のフレームのパッド情報
	/// </summary>
	int current_pad_state_ = 0;
	
	/// <summary>
	/// 前回フレームのパッド情報
	/// </summary>
	int pre_pad_state_ = 0;
};