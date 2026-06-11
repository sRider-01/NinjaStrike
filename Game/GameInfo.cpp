#include "GameInfo.h"

/// <summary>
/// GameInfo.cppで使う定数
/// </summary>
namespace
{
	/// <summary>
	/// 画面のサイズ X
	/// </summary>
	const int kResolutionX = 1280;

	/// <summary>
	/// 画面のサイズ Y
	/// </summary>
	const int kResolutionY = 720;

	/// <summary>
	/// カラービット数
	/// </summary>
	const int kColorBit = 32;

	/// <summary>
	/// ゲーム画面左端取得調整用
	/// </summary>
	const int kLeftEdge = 10;

	/// <summary>
	/// ゲーム画面右端取得調整用
	/// </summary>
	const int kRightEdge = 85;
}

/// <summary>
/// インスタンス初期化
/// </summary>
GameInfo* GameInfo::instance_ = nullptr;

/// <summary>
/// コンストラクタ
/// </summary>
GameInfo::GameInfo()
	:x_resolution_(kResolutionX)
	, y_resolution_(kResolutionY)
	, x_center_(kResolutionX / 2)
	, y_center_(kResolutionY / 2)
	, x_left_edge_((x_center_ / 4) + kLeftEdge)
	, x_right_edge_((kResolutionX * 3 / 4) + kRightEdge)
	, color_bit_(kColorBit)
{
}

/// <summary> 
/// デストラクタ
/// </summary>
GameInfo::~GameInfo()
{
}

/// <summary>
/// 画面の横幅のサイズを取得
/// </summary>
/// <returns>解像度の横幅のサイズ</returns>
int GameInfo::GetResolutionX()
{
	//横幅のサイズを返す
	return x_resolution_;
}

/// <summary>
/// 画面の縦幅のサイズを取得
/// </summary>
/// <returns>解像度の縦幅のサイズ</returns>
int GameInfo::GetResolutionY()
{
	//縦幅のサイズを返す
	return y_resolution_;
}

/// <summary>
/// 画面横幅の中心座標を取得
/// </summary>
/// <returns>画面横幅の中心座標</returns>
int GameInfo::GetCenterX()
{
	//横幅の中心座標を返す
	return x_center_;
}

/// <summary>
/// 画面縦幅の中心座標を取得
/// </summary>
/// <returns>画面縦幅の中心座標</returns>
int GameInfo::GetCenterY()
{
	//縦幅の中心座標を返す
	return y_center_;
}

/// <summary>
/// ゲーム画面の左端座標を取得
/// </summary>
/// <returns>ゲーム画面の左端座標</returns>
int GameInfo::GetEdgeLeftX()
{
	//ゲーム画面の左端座標を返す
	return x_left_edge_;
}

/// <summary>
/// ゲーム画面の右端座標を取得
/// </summary>
/// <returns>ゲーム画面の右端座標</returns>
int GameInfo::GetEdgeRightX()
{
	//ゲーム画面の右端座標を返す
	return x_right_edge_;
}
/// <summary>
/// カラービット数を取得
/// </summary>
/// <returns>カラービット数</returns>
int GameInfo::GetColorBit()
{
	//カラービット数を返す
	return color_bit_;
}