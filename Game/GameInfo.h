#pragma once

/// <summary>
/// ゲーム情報管理クラス
/// どのレベルでも必要な共通のゲームの情報を管理する。
/// </summary>
class GameInfo
{
private:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameInfo();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameInfo();

public:

	/// <summary>
	/// GameInfoのインスタンスを取得する
	/// （シングルトンデザインパターン）
	/// </summary>
	/// <returns>GameInfoのインスタンス</returns>
	static GameInfo* GetInstance()
	{
		//生成されてないなら
		if (instance_ == nullptr)
		{
			//動的生成
			instance_ = new GameInfo();
		}

		//GameInfoのインスタンスを返す
		return instance_;
	}

	/// <summary>
	/// GameInfoのインスタンスを削除
	/// ゲーム終了時に必ず呼ぶ
	/// </summary>
	static void Destroy()
	{
		//生成済みなら
		if (instance_ != nullptr)
		{
			//instance_をDelete
			delete instance_;

			//instance_をnullptrにする
			instance_ = nullptr;
		}
	}


public:

	/// <summary>
	/// 画面の横幅のサイズを取得
	/// </summary>
	/// <returns>解像度の横幅のサイズ</returns>
	int GetResolutionX();

	/// <summary>
	/// 画面の縦幅のサイズを取得
	/// </summary>
	/// <returns>解像度の縦幅のサイズ</returns>
	int GetResolutionY();

	/// <summary>
	/// 画面横幅の中心座標を取得
	/// </summary>
	/// <returns>画面横幅の中心座標</returns>
	int GetCenterX();

	/// <summary>
	/// 画面縦幅の中心座標を取得
	/// </summary>
	/// <returns>画面縦幅の中心座標</returns>
	int GetCenterY();

	/// <summary>
	/// ゲーム画面の左端座標を取得
	/// </summary>
	/// <returns>ゲーム画面の左端座標</returns>
	int GetEdgeLeftX();

	/// <summary>
	/// ゲーム画面の右端座標を取得
	/// </summary>
	/// <returns>ゲーム画面の右端座標</returns>
	int GetEdgeRightX();

	/// <summary>
	/// カラービット数を取得
	/// </summary>
	/// <returns>カラービット数</returns>
	int GetColorBit();

private:

	/// <summary>
	/// ゲーム情報インスタンス
	/// （シングルトン用の静的インスタンス）
	/// </summary>
	static GameInfo* instance_;

	/// <summary>
	/// 解像度の横幅サイズ
	/// </summary>
	int x_resolution_ = 0;

	/// <summary>
	/// 解像度の縦幅サイズ
	/// </summary>
	int y_resolution_ = 0;

	/// <summary>
	/// 画面横幅の中心座標
	/// </summary>
	int x_center_ = 0;

	/// <summary>
	/// 画面縦幅の中心座標
	/// </summary>
	int y_center_ = 0;

	/// <summary>
	/// ゲーム画面の左端座標
	/// </summary>
	int x_left_edge_ = 0;

	/// <summary>
	/// ゲーム画面の右端座標
	/// </summary>
	int x_right_edge_ = 0;

	/// <summary>
	/// カラービット数
	/// </summary>
	int color_bit_ = 0;
};
