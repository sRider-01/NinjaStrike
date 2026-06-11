#pragma once

/// <summary>
/// エフェクトの基本クラス
/// </summary>
class EffectBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	EffectBase() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EffectBase() = default;

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
	/// エフェクトの発生
	/// </summary>
	/// <param name="pos_x">X座標</param>
	/// <param name="pos_y">Y座標</param>
	/// <param name="turn_flag">反転フラグ</param>
	virtual void EffectAct(float pos_x, float pos_y , bool turn_flag) = 0;

protected:
	/// <summary>
	/// エフェクト一つに持たせる情報
	/// </summary>
	struct EffectInfo
	{
		/// <summary>
		/// 発生しているか
		/// </summary>
		bool is_active_ = false;

		/// <summary>
		/// X座標
		/// </summary>
		float pos_x = 0.0f;

		/// <summary>
		/// Y座標
		/// </summary>
		float pos_y = 0.0f;
	};
public:
	/// <summary>
	/// EffectInfoのゲッタ
	/// </summary>
	EffectInfo& GetInfo()
	{
		return effect_info_;
	}

protected:
	/// <summary>
	/// EffectInfoのインスタンス
	/// </summary>
	EffectInfo effect_info_;
};