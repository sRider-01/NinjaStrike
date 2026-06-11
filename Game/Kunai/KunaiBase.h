#pragma once
#include "Enemy/EnemyBase.h"
#include <vector>
#include <array>
#include <iostream>
/// <summary>
/// クナイの情報についてのクラス
/// </summary>
class KunaiBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	KunaiBase();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~KunaiBase() = default;

protected:
	/// <summary>
	/// クナイ一発に含める情報
	/// </summary>
	struct KunaiInfo
	{
		/// <summary>
		/// 発射されたか
		/// </summary>
		bool is_fired = false;

		/// <summary>
		/// クナイのX座標
		/// </summary>
		float pos_x = 0.0f;

		/// <summary>
		/// クナイのY座標
		/// </summary>
		float pos_y = 0.0f;

		/// <summary>
		/// クナイの幅
		/// </summary>
		int kunai_width = 0;

		/// <summary>
		/// クナイの高さ
		/// </summary>
		int kunai_height = 0;

		/// <summary>
		/// 左向きか？
		/// </summary>
		bool is_left = false;
	};

public:

	/// <summary>
	/// 毎フレームの更新処理
	/// </summary>
	/// <param name="delta_time">前回実行フレームからの経過時間（秒）</param>
	virtual void Update(float delta_time) = 0;

	/// <summary>
	/// 毎フレームの描画処理
	/// </summary>
	virtual void Render() = 0;

	/// <summary>
	/// 自身を発射する処理
	/// </summary>
	/// <param name="kunai_type">クナイの種類</param>
	/// <param name="pos_x">クナイのX座標</param>
	/// <param name="pos_y">クナイのY座標</param>
	virtual void Fire(float pos_x, float pos_y, bool is_facing_left);

	/// <summary>
	/// 発射したクナイをプールに戻す
	/// </summary>
	void Deactivate();
	
	/// <summary>
	/// クナイインフォのゲッタ
	/// </summary>
	/// <returns>クナイインフォ</returns>
	KunaiInfo& GetInfo()
	{
		//kunai_info_を返す
		return kunai_info_;
	}

	/// <summary>
	/// 画像の幅、高さをセットする
	/// </summary>
	/// <param name="handle">画像ハンドル</param>
	/// <param name="width">画像幅</param>
	/// <param name="height">画像高さ</param>
	void SetImageResource(int handle, int width, int height);


protected:
	/// <summary>
	/// KunaiInfoのインスタンス
	/// </summary>
	KunaiInfo kunai_info_;

	/// <summary>
	/// 画像のハンドル
	/// </summary>
	int kunai_image_handle_ = -1;
};

