#pragma once
#include "Task/Task.h"

/// <summary>
/// HPバーに関するクラス
/// </summary>
class HpBar :public Task
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	HpBar() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~HpBar() = default;

public:
	/// <summary>
	/// 毎フレームの更新処理
	/// </summary>
	/// <param name="delta_time">前回実行フレームからの経過時間（秒）</param>
	void Update(float delta_time) override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Render() override;

	/// <summary>
	/// 破棄前処理
	/// </summary>
	void OnDestroy() override
	{
		Finalize();
	}

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="max_hp">最大HP</param>
	void Initialize(int max_hp);

	/// <summary>
	/// 後処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// 現在のHPをセット
	/// </summary>
	/// <param name="current_hp">現在のHP</param>
	void SetHp(int current_hp);



private:

	/// <summary>
	/// HPバーの画像ハンドル
	/// </summary>
	int hp_bar_image_handle_ = -1;

	/// <summary>
	/// 元の画像の横幅
	/// </summary>
	int source_image_width_ = 0;

	/// <summary>
	/// 元の画像の高さ
	/// </summary>
	int source_image_height_ = 0;

	/// <summary>
	/// 切り分けた後の画像一つ分の幅
	/// </summary>
	int frame_width_ = 0;

	/// <summary>
	/// 切り分けた後の画像一つ分の高さ
	/// </summary>
	int frame_height_ = 0;

	/// <summary>
	/// 描画する画像のインデックス(上から何番目か)
	/// </summary>
	int frame_index_ = 0;

	/// <summary>
	/// 描画する四角形の元画像でのX座標
	/// </summary>
	int src_x_ = 0;

	/// <summary>
	/// 描画する四角形の元画像でのY座標
	/// </summary>
	int src_y_ = 0;

	/// <summary>
	/// 描画位置X
	/// </summary>
	int draw_pos_x_ = 0;

	/// <summary>
	/// 描画位置Y
	/// </summary>
	int draw_pos_y_ = 0;

	/// <summary>
	/// 最大HP
	/// </summary>
	int max_hp_ = 0;

	/// <summary>
	/// 現在のHP
	/// </summary>
	int current_hp_ = 0;
};