#include "GameInfo.h"
#include "FadeManager.h"
#include "DebugManager/DebugManager.h"
#include "Task/TaskManager.h"
#include "Input/InputManager.h"
#include "Level/LevelChanger.h"
#include "Level/GameLevel.h"
#include "TileMap/TileSet.h"
#include "TileMap/TileDataBase.h"
#include "TileMap/TileMap.h"
#include "Stage/TileMapRenderer.h"
#include "Stage/BackGround.h"
#include "DxLib.h"
#include <crtdbg.h>
#include <chrono>

//メモリリーク検出のために必要
#if defined(_WIN64) || defined(_WIN32)
#include <windows.h>
#endif


/// <summary>
/// メイン処理用の無名名前空間(内部リンケージ)
/// </summary>
namespace
{
	/// <summary>
	/// 入力レイテンシの警告閾値（秒）
	/// 0.05s(50ms)以内なら「待ち」と感じない
	/// </summary>
	const float kLatencyWarnThreshold = 0.05f;

	/// <summary>
	/// 入力レイテンシの許容閾値（秒）
	/// 0.08sを超えると「遅い」と感じ始める
	/// </summary>
	const float kLatencyLimitThreshold = 0.08f;

	/// <summary>
	/// 入力レイテンシの計測結果（秒）
	/// </summary>
	float input_latency_seconds = 0.0f;

	/// <summary>
	/// 1000ミリ秒（1秒）
	/// </summary>
	const int kOneMillisecond = 1000;

	/// <summary>
	/// 目標FPS
	/// </summary>
	const int kTargetFps = 60;

	/// <summary>
	/// 1フレームの時間（秒）
	/// </summary>
	const float kOneFrameTimeSecond = (static_cast<float>(kOneMillisecond) / static_cast<float>(kTargetFps)) / kOneMillisecond;

	/// <summary>
	/// 現在の時間（ミリ秒）
	/// </summary>
	int current_time_millisecond = 0;

	/// <summary>
	/// 前回のフレーム実行時の時間（ミリ秒）
	/// </summary>
	int last_frame_time_millisecond = 0;

	/// <summary>
	/// フレーム数のカウント用
	/// </summary>
	int frame_count = 0;

	/// <summary>
	/// 最後にフレームレートを更新した時間（ミリ秒）
	/// </summary>
	int last_update_frame_rate_time_millisecond = 0;

	/// <summary>
	/// 算出したフレームレート格納用
	/// </summary>
	float frame_rate = 0.0f;

	/// <summary>
	/// 色の度合い
	/// </summary>
	const int color_level= 255;

	/// <summary>
	/// レイテンシ表示のY座標
	/// </summary>
	const int draw_latency_pos_y = 32;
}

/// <summary>
/// フレームレート計算用
/// </summary>
void CalcFrameRate();

/// <summary>
/// フレームレート描画用
/// </summary>
void DrawFrameRate();

/// <summary>
/// 入力レイテンシのオーバーレイ描画
/// 目標：0.05s以内　許容：0.08s以内
/// </summary>
void DrawInputLatencyOverlay();

/// <summary>
/// プログラムは WinMain から始まります
/// </summary>
/// <param name="hInstance">アプリケーションの現在のインスタンスハンドル</param>
/// <param name="hPrevInstance">Win16 の産物（常にNULL）</param>
/// <param name="lpCmdLine">コマンドライン引数</param>
/// <param name="nCmdShow">ウィンドウの表示状態</param>
/// <returns>アプリケーションの終了コード</returns>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	setlocale(LC_ALL, "Japanese");
	//メモリリーク検出のために記述
#if defined(_WIN64) || defined(_WIN32)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(11409);
#endif

	 //CRT内部の初期化が終わった後にスナップショットを取る
	_CrtMemState mem_state_start{};

	//ログファイルは出力しない設定にする
	SetOutApplicationLogValidFlag(FALSE);

	//ウィンドウモードに設定
	ChangeWindowMode(TRUE);

	//ウィンドウサイズとカラービットを設定
	//（値は後ほど定数化します）
	GameInfo* GameInstance = GameInfo::GetInstance();
	SetGraphMode(GameInstance->GetResolutionX(), GameInstance->GetResolutionY(), GameInstance->GetColorBit());

	//DXライブラリの初期化
	if (DxLib_Init() == -1)
	{

		//初期化失敗は強制終了
		return -1;
	}

	//描画先の画面を裏画面に設定
	SetDrawScreen(DX_SCREEN_BACK);

	//ウィンドウが非アクティブになってもゲームは止めないようにする
	SetAlwaysRunFlag(true);

	//タスクマネージャーにレベルチェンジャーを登録
	TaskManager::GetInstance()->AddTask(LevelChanger::GetInstance());

	//レベルチェンジャーの状態をkInitTitleLevelにする
	LevelChanger::GetInstance()->SetLevelChangerState(LevelChanger::LevelChangerState::kInitTitleLevel);

	//残りのシングルトンを先に初期化する
	InputManager::GetInstance();
	FadeManager::GetInstance();
	AudioManager::GetInstance();

	//全シングルトン初期化後にスナップショット
	_CrtMemCheckpoint(&mem_state_start);

	//ゲームメインループ
	while (ProcessMessage() != -1)
	{
		//システムに処理を渡す
		Sleep(1);

		//現在の時間更新
		current_time_millisecond = GetNowCount();

		//経過時間が目標フレームレート分の1以上経過しているなら
		//フレームを実行する
		if (current_time_millisecond - last_frame_time_millisecond >= kOneMillisecond / kTargetFps)
		{
			//前回フレーム実行からの経過時間を秒で算出
			float delta_time = static_cast<float>(current_time_millisecond - last_frame_time_millisecond) / kOneMillisecond;

			//最後にフレームを実行した時間を更新
			last_frame_time_millisecond = current_time_millisecond;

			//フレーム実行のたびにFPSをカウントする
			++frame_count;

			//画面の消去
			ClearDrawScreen();

			//ウィンドウがアクティブなら実行する処理
			if (GetWindowActiveFlag() == TRUE)
			{
				//入力処理開始時刻を記録
				std::chrono::time_point<std::chrono::high_resolution_clock> input_start_time
					= std::chrono::high_resolution_clock::now();

				//キーの情報をアップデートする
				InputManager::GetInstance()->UpdateKeyStateAll(delta_time);

				//Update関連を呼ぶ
				//タスクの更新
				TaskManager::GetInstance()->UpdateTask(delta_time);

				std::chrono::time_point<std::chrono::high_resolution_clock> input_end_time
					= std::chrono::high_resolution_clock::now();

				//このフレームでいずれかのキーが押されたか確認
				bool is_any_key_pushed_this_frame =
					InputManager::GetInstance()->IsPushThisFrame(InputManager::GameKeyKind::kLeft) ||
					InputManager::GetInstance()->IsPushThisFrame(InputManager::GameKeyKind::kRight) ||
					InputManager::GetInstance()->IsPushThisFrame(InputManager::GameKeyKind::kPlayerJump) ||
					InputManager::GetInstance()->IsPushThisFrame(InputManager::GameKeyKind::kPlayerRun) ||
					InputManager::GetInstance()->IsPushThisFrame(InputManager::GameKeyKind::kPlayerCloseAttack) ||
					InputManager::GetInstance()->IsPushThisFrame(InputManager::GameKeyKind::kPlayerLongRangeAttack);

				//キーが押されたフレームのみレイテンシを更新する
				if (is_any_key_pushed_this_frame)
				{
					input_latency_seconds = std::chrono::duration<float>(
						input_end_time - input_start_time).count();
				}
			}

			//キーの情報を記録する
			InputManager::GetInstance()->RecordKeyStateAll(delta_time);
			
			//描画関連を呼ぶ
			//タスクの描画
			TaskManager::GetInstance()->RenderTask();

			//破棄中・待機中でない場合のみ呼ぶ
			if (LevelChanger::GetInstance()->GetCurrentLevelKind()
				== LevelChanger::CurrentLevelKind::kBattle)
			{
				const LevelChanger::LevelChangerState state =
					LevelChanger::GetInstance()->GetLevelChangerState();

				//Wait状態のみ描画（破棄中・遷移中は呼ばない）
				if (state == LevelChanger::LevelChangerState::kWait)
				{
					LevelBase* current_level =
						LevelChanger::GetInstance()->GetCurrentLevel();
					if (current_level != nullptr)
					{
						GameLevel* game_level =
							static_cast<GameLevel*>(current_level);
						game_level->RenderUI();
					}
				}
			}

			//FadeManagerのUpdateを呼ぶ(透明度を更新)
			FadeManager::GetInstance()->Update(delta_time);

			//黒画を呼び出す(透明度を反映)
			FadeManager::GetInstance()->DrawGraphics();


			if (DebugManager::IsDebugEnabled())
			{
				//フレームレートの計算
				CalcFrameRate();

				//フレームレートの描画
				DrawFrameRate();

				//経過時間を描画
				DrawFormatString(0, 16, GetColor(255, 30, 30), "delta_time[%.4f]", delta_time);

				//レイテンシの計算、描画
				DrawInputLatencyOverlay();
			}

			//表画面への出力
			ScreenFlip();
		}
	}

	//レベルチェンジャーのForceDestroyCurrentLevelを呼ぶ
	LevelChanger::GetInstance()->ForceDestroyCurrentLevel();

	//黒画の破棄
	FadeManager::GetInstance()->DeleteGraphics();
	//インプットマネージャーの破棄
	InputManager::Destroy();
	//タスクの全破棄
	TaskManager::GetInstance()->Finalize();
	//タスクマネージャーの破棄
	TaskManager::Destroy();
	//フェードマネージャーの破棄
	FadeManager::Destroy();
	//オーディオマネージャーの破棄
	AudioManager::Destroy();
	//ゲームインフォの破棄
	GameInfo::Destroy();

	GameInstance = nullptr;

	//DXライブラリの終了処理
	DxLib_End();


	//終了後のスナップショットを取って差分だけ出力
#if defined(_WIN64) || defined(_WIN32)
	_CrtMemState mem_state_end{};
	_CrtMemState mem_state_diff{};
	_CrtMemCheckpoint(&mem_state_end);

	if (_CrtMemDifference(&mem_state_diff, &mem_state_start, &mem_state_end))
	{
		// 差分がある場合のみ出力（本物のリークのみ）
		_CrtMemDumpStatistics(&mem_state_diff);
	}
#endif

	return 0;
}

/// <summary>
/// フレームレート計算用
/// </summary>
void CalcFrameRate()
{
	//前回のフレームレート更新からの経過時間を求める
	int diff_time = current_time_millisecond - last_update_frame_rate_time_millisecond;

	//前回のフレームレート更新から
	//1秒以上経過していたらフレームレートを更新する
	if (diff_time > kOneMillisecond)
	{
		//フレーム回数をミリ秒に合わせる
		//少数まで出したいのでfloatにキャスト
		float count = (float)(frame_count * kOneMillisecond);

		//フレームレートを求める
		//理想どうりなら 60000 / 1000 で 60 となる
		frame_rate = count / diff_time;

		//フレームカウントをクリア
		frame_count = 0;

		//フレームレート更新時間を更新
		last_update_frame_rate_time_millisecond = current_time_millisecond;
	}

}

/// <summary>
/// フレームレート描画用
/// </summary>
void DrawFrameRate()
{
	//フレームレートの描画
	DrawFormatString(0, 0, GetColor(color_level, 0, 0), "FPS[%.2f]", frame_rate);
}

/// <summary>
/// 入力レイテンシのオーバーレイ描画
/// 目標：0.05s以内　許容：0.08s以内
/// </summary>
void DrawInputLatencyOverlay()
{
	unsigned int color = 0;
	if (input_latency_seconds <= kLatencyWarnThreshold)
	{
		color = GetColor(0, color_level, 0);
	}
	else if (input_latency_seconds <= kLatencyLimitThreshold)
	{
		color = GetColor(color_level, color_level, 0);
	}
	else
	{
		color = GetColor(color_level, 0, 0);
	}

	DrawFormatString(0, draw_latency_pos_y, color,
		"InputLatency[%.3fms] %s",
		input_latency_seconds * kOneMillisecond,
		input_latency_seconds <= kLatencyWarnThreshold ? "OK" :
		input_latency_seconds <= kLatencyLimitThreshold ? "WARN" : "NG");
}