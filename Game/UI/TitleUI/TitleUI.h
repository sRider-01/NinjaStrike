#pragma once
#include "GameInfo.h"
#include <iostream>
#include <array>
/// <summary>
/// タイトルロゴ/Zを押してくださいのUIクラス
/// </summary>
class TitleUI
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	TitleUI();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleUI();

	/// <summary>
	/// 画像の読み込み処理
	/// </summary>
	void LoadGraphics();

	/// <summary>
	/// 経過時間の更新処理
	/// </summary>
	/// <param name="delta_time">前回実行フレームからの経過時間（秒）</param>
	void Update(float delta_time);

	/// <summary>
	/// 毎フレームの描画処理
	/// </summary>
	void Render();

	/// <summary>
	/// 画像を消す処理
	/// </summary>
	void DestroyGraphics();

	/// <summary>
	/// ゲームパッド接続状態をセットする
	/// </summary>
	/// <param name="is_connected">接続されているか</param>
	void SetGamepadConnected(bool is_connected)
	{
		is_gamepad_connected_ = is_connected;
	}
	/// <summary>
	/// 画像の種類
	/// </summary>
	static const int kImageCount = 3;

private:
	/// <summary>
	/// 画像の種類
	/// </summary>
	enum ImageKind
	{
		kTitle,		   //タイトル
		kPressZ,	   //「Zを押してください」
		kPressA,       // 「Aボタンを押してください」（パッド用）
	};

	/// <summary>
	/// 「Zを押してください」用フェードアウトの状態
	/// </summary>
	enum class AnimMode
	{
		kIdle, //通常
		kZoom, //拡大
	};


	/// <summary>
	/// ハンドルの配列
	/// </summary>
	std::array<int, kImageCount> handles_{};

	/// <summary>
	/// 画像の横幅の配列
	/// </summary>
	std::array<int, kImageCount> widths_{};

	/// <summary>
	/// 画像の高さの配列
	/// </summary>
	std::array<int, kImageCount> heights_{};

	/// <summary>
	/// タイトル画像のx座標
	/// </summary>
	int x_title_ui_pos_ = 0;

	/// <summary>
	/// タイトル画像のy座標
	/// </summary>
	int y_title_ui_pos_ = 0;

	/// <summary>
	/// 「Zを押してください」画像のx座標
	/// </summary>
	int x_press_z_ui_pos_ = 0;

	/// <summary>
	/// 「Zを押してください」画像のy座標
	/// </summary>
	int y_press_z_ui_pos_ = 0;

	/// <summary>
	/// タイトル画像の横幅微調整用
	/// </summary>
	int title_width_adjust = 0;

	/// <summary>
	/// タイトル画像の高さ微調整用
	/// </summary>
	int title_height_adjust = 0;

	/// <summary>
	/// 「Zを押してください」画像の高さ微調整用
	/// </summary>
	int press_z_height_adjust = 0;

	/// <summary>
	/// 点滅させるためのタイマー
	/// </summary>
	float blink_timer_ = 0.0f;

	/// <summary>
	/// 点滅させるための基準時間
	/// </summary>
	float interval_ = 0.0f;

	/// <summary>
	/// 画像を表示させるか？
	/// </summary>
	bool is_show_ = true;

	/// <summary>
	/// 画像拡大にかける時間
	/// </summary>
	float zoom_time_ = 0.0f;

	/// <summary>
	/// フェードアウトにかける時間
	/// </summary>
	float fade_time_ = 0.0f;

	/// <summary>
	/// 透明度
	/// </summary>
	float alpha_ = 0.0f;

	/// <summary>
	/// 現在の拡大率
	/// </summary>
	float now_scale_ = 1.0f;

	/// <summary>
	/// 拡大率
	/// </summary>
	float scale_rate = 1.0f;

	/// <summary>
	/// 透明率
	/// </summary>
	float alpha_rate = 1.0f;

	/// <summary>
	/// 拡大フェードアウトのアニメーション用タイマー
	/// </summary>
	float anim_timer_ = 0.0f;

	/// <summary>
	/// フェードアウトの状態。初期は「通常」
	/// </summary>
	AnimMode anim_mode_state_ = AnimMode::kIdle;

	/// <summary>
	/// ゲームパッドが接続されているか
	/// </summary>
	bool is_gamepad_connected_ = false;

};