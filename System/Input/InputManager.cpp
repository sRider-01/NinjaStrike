#include "InputManager.h"
#include "DxLib.h"

/// <summary>
/// タスクマネージャーのインスタンス初期化
/// </summary>
InputManager* InputManager::instance_ = nullptr;

/// <summary>
/// InputManager.cppのみで使う定数
/// </summary>
namespace
{
	/// <summary>
	/// 同じ動作をするボタンの最大数
	/// </summary>
	const int kMaxAssignKey = 2;

	/// <summary>
	/// ゲームボタンに対応したDXライブラリ側のキー定数
	/// </summary>
	const int kDxlibKeyKind[static_cast<int>(InputManager::GameKeyKind::kNum)][kMaxAssignKey] =
	{
		{ KEY_INPUT_RETURN, -1 },               //Decide
		{ KEY_INPUT_BACK,   -1 },               //Cancel
		{ KEY_INPUT_LEFT,   KEY_INPUT_A },      //Left  ← + A
		{ KEY_INPUT_RIGHT,  KEY_INPUT_D },      //Right → + D
		{ KEY_INPUT_ESCAPE, -1 },               //Pause
		{ KEY_INPUT_LSHIFT, KEY_INPUT_RSHIFT }, //Dash
		{ KEY_INPUT_SPACE , -1 },               //jump
		{ KEY_INPUT_X , KEY_INPUT_L },          //CloseAttack
		{ KEY_INPUT_Z,  KEY_INPUT_J },          //LongRangeAttack
		{ KEY_INPUT_UP,     KEY_INPUT_W },      //Up
		{ KEY_INPUT_DOWN,   KEY_INPUT_S },      //Down
		{ KEY_INPUT_Q, -1 },                    //Quit 
		//Debug
		{ KEY_INPUT_C, -1},//ContactDamage
		{ KEY_INPUT_T, -1},//AttackDamage
	};

	/// <summary>
	/// 同じ動作をするボタンの最大数(ゲームパッド)
	/// </summary>
	const int kMaxAssignPad = 2;

	/// <summary>
	/// アナログスティックが「倒れた」と判定するためのしきい値
	/// </summary>
	const int kAnalogStickDeadzone = 400;

	/// <summary>
	/// ゲームボタンに対応したDXライブラリ側のパッド定数(ゲームパッド)
	/// </summary>
	const int kDxlibPadKind[static_cast<int>(InputManager::GameKeyKind::kNum)][kMaxAssignPad] =
	{
		{ PAD_INPUT_1, -1 },            //Decide（A/×）← Jumpと兼用。ポーズ中の操作のため競合は起きない
		{ PAD_INPUT_2, -1 },            //Cancel（B/○）← 同上
		{ PAD_INPUT_LEFT,  -1 },        //Left
		{ PAD_INPUT_RIGHT, -1 },        //Right
		{ PAD_INPUT_10, -1 },           //Pause（Start）
		{ PAD_INPUT_6, -1 },            //Dash（RB）
		{ PAD_INPUT_1, PAD_INPUT_2 },   //Jump（A/×、B/○）
		{ PAD_INPUT_4, -1 },            //CloseAttack（Y/△）
		{ PAD_INPUT_3, -1 },            //LongRangeAttack（X/□）
		{ PAD_INPUT_UP,   -1 },			// Up
		{ PAD_INPUT_DOWN, -1 },         // Down
		{ PAD_INPUT_9,  -1 },           // Quit（Back/Select）
	};

	/// <summary>
	/// InputManager.cppでのみ使う関数
	/// パッドの調べたいボタンは押されているか？
	/// </summary>
	/// <param name="state">パッドの状態</param>
	/// <param name="pad_mask">調べたいボタン</param>
	/// <returns>押されている/押されていない</returns>
	bool IsPadDown(int state, int pad_mask)
	{
		if ((state & pad_mask) != 0)
		{
			return true;
		}
		return false;
	}
}

/// <summary>
/// コンストラクタ
/// </summary>
InputManager::InputManager()
	: current_key_buffer_{ 0 }
	, pre_key_buffer_{ 0 }
{
}

/// <summary>
/// デストラクタ
/// </summary>
InputManager::~InputManager()
{
}

/// <summary>
/// 現在のフレームのキー情報をすべて更新
/// タスクマネージャーの更新の前に呼ぶ
/// </summary>
/// <param name="delta_time">前回実行フレームからの経過時間（秒）</param>
void InputManager::UpdateKeyStateAll(float delta_time)
{
	//現在のキー情報を更新する
	GetHitKeyStateAll(current_key_buffer_);

	current_pad_state_ = GetJoypadInputState(DX_INPUT_PAD1);
}

/// <summary>
/// 現在のフレームのキー情報を記録
/// フレームの最後に呼ぶ
/// </summary>
/// <param name="delta_time">前回実行フレームからの経過時間（秒）</param>
void InputManager::RecordKeyStateAll(float delta_time)
{
	//現在のキー情報を記録しておく
	for (int i = 0; i < kKeyBufferSize; ++i) {
		pre_key_buffer_[i] = current_key_buffer_[i];
	}

	pre_pad_state_ = current_pad_state_;
}

/// <summary>
/// 引数のキーはこのフレームで押されたか
/// </summary>
/// <param name="key_kind">調べるキー</param>
/// <returns>押されているならtrue</returns>
bool InputManager::IsPushThisFrame(GameKeyKind game_key_kind)
{
	const int action = static_cast<int>(game_key_kind);

	//キーについて
	for (int i = 0; i < kMaxAssignKey; ++i)
	{
		const int key = kDxlibKeyKind[action][i];
		if (key < 0)
		{
			continue;
		}

		//current_key_buffer_ と pre_key_buffer_ の値をチェック
		//current_key_buffer_ と pre_key_buffer_ の値が違う(押されている)なら
		//今フレームで 0→1 になったら「押した瞬間」
		if (current_key_buffer_[key] == 1 && pre_key_buffer_[key] == 0)
		{
			return true;
		}
	}

	//パッドについて
	for (int i = 0; i < kMaxAssignPad; ++i)
	{
		const int pad = kDxlibPadKind[action][i];
		if (pad < 0)
		{
			continue;
		}

		const bool is_down_now = IsPadDown(current_pad_state_, pad);

		const bool was_down = IsPadDown(pre_pad_state_, pad);

		if (is_down_now && !was_down)
		{
			return true;
		}
	}

	return false;
}

/// <summary>
/// 引数のキーは押されているか
/// </summary>
/// <param name="game_key_kind">調べるキー</param>
/// <returns>押されているならtrue</returns>
bool InputManager::IsDown(GameKeyKind game_key_kind)
{
	const int action = static_cast<int>(game_key_kind);

	//キーについて
	//current_key_buffer_の値をチェック(押されているかチェック)
	//0:押されていない 1:押されている
	for (int i = 0; i < kMaxAssignKey; ++i)
	{
		const int key = kDxlibKeyKind[action][i];
		if (key < 0)
		{
			continue;
		}

		if (current_key_buffer_[key] == 1)
		{
			return true;
		}
	}

	//パッドについて
	for (int i = 0; i < kMaxAssignPad; ++i)
	{
		const int pad = kDxlibPadKind[action][i];
		if (pad < 0)
		{
			continue; 
		}

		if ((current_pad_state_ & pad) != 0)
		{
			return true;
		}
	}

	//パッド左スティックについて
	if (game_key_kind == GameKeyKind::kLeft || game_key_kind == GameKeyKind::kRight)
	{
		int x = 0;
		int y = 0;
		GetJoypadAnalogInput(&x, &y, DX_INPUT_PAD1);

		//左かつしきい値より小さい値（左に倒してる判定）なら
		if (game_key_kind == GameKeyKind::kLeft && x < -kAnalogStickDeadzone)
		{
			return true; 
		}
		
		//右かつしきい値より大きい値（右に倒してる判定）なら
		if (game_key_kind == GameKeyKind::kRight && x > kAnalogStickDeadzone)
		{
			return true;
		}
	}
	return false;
}

/// <summary>
/// ゲームパッドが接続されているかを返す
/// </summary>
bool InputManager::IsGamepadConnected() const
{
	return GetJoypadNum() > 0;
}