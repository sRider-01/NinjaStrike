#include "KunaiBase.h"
#include "DxLib.h"
#include "GameInfo.h"
#include "Player/Player.h"
#include <algorithm>

/// <summary>
/// KunaiBaseで使う定数
/// </summary>
namespace
{
	/// <summary>
	/// 発射済みのクナイが画面内に残らないよう、画面内から消すための値
	/// </summary>
	const float kDeactivatePos = -100.0;
}


/// <summary>
/// コンストラクタ
/// </summary>
KunaiBase::KunaiBase()
{
}

/// <summary>
/// 自身を発射する処理
/// </summary>
/// <param name="pos_x">クナイのX座標</param>
/// <param name="pos_y">クナイのY座標</param>
void KunaiBase::Fire(float pos_x, float pos_y, bool is_facing_left)
{
	//空の仮想関数にしておく
}

/// <summary>
/// 画像の幅、高さをセットする
/// </summary>
void KunaiBase::SetImageResource(int handle, int width, int height)
{
	kunai_image_handle_ = handle;

	kunai_info_.kunai_width = width;

	kunai_info_.kunai_height = height;
}

/// <summary>
/// 発射したクナイをプールに戻す
/// </summary>
void KunaiBase::Deactivate()
{
	kunai_info_.pos_x = kDeactivatePos;
	kunai_info_.pos_y = kDeactivatePos;

	kunai_info_.is_left = false;

	kunai_info_.is_fired = false;
}