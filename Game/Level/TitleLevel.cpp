#include "DxLib.h"
#include "TitleLevel.h"
#include "UI/TitleUI/TitleUI.h"
#include "LevelChanger.h"
#include "FadeManager.h"
#include "Input/InputManager.h"
#include "Audio/AudioManager.h"


/// <summary>
/// TitleLevel.cppのみで使用する定数
/// </summary>
namespace
{
	/// <summary>
	/// タイトルBGM
	/// </summary>
	const char* kTitleBgmPath = "Assets/Audio/BGM/TitleBgm.mp3";

	/// <summary>
	/// スタートSE
	/// </summary>
	const char* kTitleStartSePath = "Assets/Audio/SE/TitleStart.mp3";

	/// <summary>
	/// フェードアウトにかける時間
	/// </summary>
	const float kFadeInTime = 1.0f;

	/// <summary>
	/// フェードアウトにかける時間
	/// </summary>
	const float kFadeOutTime = 2.0f;
}

/// <summary>
/// コンストラクタ
/// </summary>
TitleLevel::TitleLevel()
	: title_level_state_(TitleLevelState::kNone)
	, title_bg_handle_(0)
	, elapsed_time_(0.0f)
{
}

/// <summary>
/// デストラクタ
/// </summary>
TitleLevel::~TitleLevel()
{
}

/// <summary>
/// 毎フレームの更新処理
/// </summary>
/// <param name="delta_time">前回実行フレームからの経過時間（秒）</param>
void TitleLevel::Update(float delta_time)
{
	//何もしない状態なら実行しない
	if (title_level_state_ == TitleLevelState::kNone)
	{
		return;
	}

	FadeManager::GetInstance()->Update(delta_time);

	//パッドが接続されているかをつたえる
	title_ui_->SetGamepadConnected(
		InputManager::GetInstance()->IsGamepadConnected()
	);

	//タイトルUIをUpdateさせる
	title_ui_->Update(delta_time);

	//Zキーもしくはエンターキーが押されたら
	if (InputManager::GetInstance()->IsPushThisFrame(InputManager::GameKeyKind::kPlayerLongRangeAttack) || InputManager::GetInstance()->IsPushThisFrame(InputManager::GameKeyKind::kDecide))
	{
		//BGMを止める
		AudioManager::GetInstance()->StopBgm(audio_handle_);

		if (is_first_se_)
		{
			//スタートSEを鳴らす
			AudioManager::GetInstance()->PlaySe(se_handle_);

			is_first_se_ = false;
		}

		//フェードアウトさせる
		FadeManager::GetInstance()->StartFadeOut(kFadeOutTime);

		//遷移準備完了
		is_change_ready_ = true;
	}

	//フェードが終わったら
	if (FadeManager::GetInstance()->IsFadeFinished() && is_change_ready_ == true)
	{
		AudioManager::GetInstance()->StopBgm(audio_handle_);
		//状態を何もしないへ
		title_level_state_ = TitleLevelState::kNone;

		//レベルチェンジャーの状態をタイトルレベル終了へ
		/* ココはレベルチェンジャー実装後に自分で記述する */
		LevelChanger::GetInstance()->SetLevelChangerState(LevelChanger::LevelChangerState::kFinTitleLevel);
	}
}

/// <summary>
/// 毎フレームの描画処理
/// </summary>
void TitleLevel::Render()
{
	//タイトル画像の描画
	title_ui_->Render();

	FadeManager::GetInstance()->DrawGraphics();
}

/// <summary>
/// レベルの開始処理
/// </summary>
void TitleLevel::BeginLevel()
{
	//TitleUIをnew
	title_ui_ = new TitleUI();

	//タイトル画像の読み込み
	title_ui_->LoadGraphics();

	//フェードに使う黒画読み込み
	FadeManager::GetInstance()->LoadGraphics();

	//状態を通常へ
	title_level_state_ = TitleLevelState::kPlay;

	//BGMをロード
	audio_handle_ = AudioManager::GetInstance()->LoadBgm(kTitleBgmPath);

	//SEをロード
	se_handle_ = AudioManager::GetInstance()->LoadSe(kTitleStartSePath);

	//BGMを再生
	AudioManager::GetInstance()->PlayBgm(audio_handle_);

	if (is_first_fade_)
	{
		//フェードインさせる
		FadeManager::GetInstance()->StartFadeIn(kFadeInTime);

		is_first_fade_ = false;
	}
}

/// <summary>
/// タイトルレベルの管理するものをタスクマネージャーから解放する
/// </summary>
void TitleLevel::ReleaseLevel(TaskId task_id)
{
	//今はタスクマネージャーに登録しているものはない
}

/// <summary>
/// タイトルレベルの管理するものを破棄
/// </summary>
void TitleLevel::DestroyLevel()
{
	//タイトル画像破棄
	title_ui_->DestroyGraphics();

	//TitleUIをdelete
	delete title_ui_;

	//ポインタの先をnullptrに
	title_ui_ = nullptr;

	//背景画像破棄
	if (title_bg_handle_ != 0)
	{
		DeleteGraph(title_bg_handle_);
		title_bg_handle_ = 0;
	}
}

/// <summary>
/// 破棄前処理
/// </summary>
void TitleLevel::OnDestroy()
{
	DestroyLevel();
}

/// <summary>
/// 画像の破棄
/// </summary>
void TitleLevel::DestroyGraphics()
{
	//背景画像破棄
	DeleteGraph(title_bg_handle_);
}