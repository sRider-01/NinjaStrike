#pragma once
#include "Task/Task.h"
#include "SlashEffect.h"
#include "ExplosionEffect.h"
#include <array>

/// <summary>
/// エフェクトの管理クラス
/// </summary>
class EffectManager : public Task
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	EffectManager() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EffectManager() = default;

public:
	/// <summary>
	/// エフェクトの種別
	/// </summary>
	enum class EffectId
	{
		/// <summary>
		/// 斬撃エフェクト
		/// </summary>
		kSlash,

		/// <summary>
		/// 爆発エフェクト
		/// </summary>
		kExplosion,

		/// <summary>
		/// サイズ取得用
		/// </summary>
		kCount, 
	};

	/// <summary>
	/// エフェクトの発生元情報の列挙
	/// </summary>
	enum  class EffectOwner
	{
		/// <summary>
		/// プレイヤー
		/// </summary>
		kPlayer,

		/// <summary>
		/// 敵
		/// </summary>
		kEnemy,

		/// <summary>
		/// サイズ取得用
		/// </summary>
		kCount,
	};

	/// <summary>
	/// 毎フレームの更新処理
	/// </summary>	/// <param name="delta_time">前回実行フレームからの経過時間（秒）</param>
	void Update(float delta_time) override;

	/// <summary>
	/// 毎フレームの描画処理
	/// </summary>
	void Render() override;

	/// <summary>
	/// 破棄前処理
	/// </summary>
	void OnDestroy() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void InitializeEffect();

	/// <summary>
	/// 後処理
	/// </summary>
	void FinalizeEffect();

	/// <summary>
	/// エフェクトの発生
	/// </summary>
	/// <param name="owner">誰が発生させたか</param>
	/// <param name="id">エフェクトの種類</param>
	/// <param name="pos_x">X座標</param>
	/// <param name="pos_y">Y座標</param>
	void EffectAct(EffectOwner owner, EffectId id, float pos_x, float pos_y, bool turn_flag);

	/// <summary>
	/// 描画されていないエフェクトの検索
	/// </summary>
	/// <returns>描画されていないエフェクトのポインタ</returns>
	EffectBase* FindFreeEffect(EffectId id);

	/// <summary>
	/// カメラのセット
	/// </summary>
	/// <param name="camera">カメラのポインタ</param>
	void SetCamera(Camera2D* camera);

private:
	/// <summary>
	/// 斬撃エフェクトの最大発生数
	/// </summary>
	static const int kMaxSlashCount = 10;

	/// <summary>
	/// 斬撃エフェクトのフレーム数
	/// </summary>
	static const int kSlashImageFrameCount = 5;

	/// <summary>
	/// 斬撃エフェクトのフレーム配列
	/// </summary>
	std::array<int, kSlashImageFrameCount> slash_frames_{};
	
	/// <summary>
	/// 斬撃エフェクトのプール
	/// </summary>
	std::array<SlashEffect, kMaxSlashCount> slash_pool_{};

	/// <summary>
	/// 斬撃エフェクトの一コマに使用する秒数
	/// </summary>
	float slash_interval_ = 0.0f;


	/// <summary>
	/// 爆発エフェクトの最大発生数
	/// </summary>
	static const int kMaxExplosionCount = 10;

	/// <summary>
	/// 爆発エフェクトのフレーム数
	/// </summary>
	static const int kExplosionImageFrameCount = 5;

	/// <summary>
	/// 爆発エフェクトのフレーム配列
	/// </summary>
	std::array<int, kExplosionImageFrameCount> explosion_frames_{};

	/// <summary>
	/// 爆発エフェクトのプール
	/// </summary>
	std::array<ExplosionEffect, kMaxExplosionCount> explosion_pool_{};

	/// <summary>
	/// 爆発エフェクトの一コマに使用する秒数
	/// </summary>
	float explosion_interval_ = 0.0f;

	/// <summary>
	/// カメラのポインタ
	/// </summary>
	Camera2D* camera_ = nullptr;
};
