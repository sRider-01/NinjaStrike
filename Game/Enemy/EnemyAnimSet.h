#pragma once
#include <array>

/// <summary>
/// 敵のアニメーション管理用クラス
/// </summary>
class EnemyAnimSet
{
public:
	/// <summary>
	/// アニメーションの種類
	/// </summary>
	enum class AnimKind
	{
		/// <summary>
		/// アイドル状態
		/// </summary>
		kIdle,

		/// <summary>
		/// 歩行
		/// </summary>
		kRun,

		/// <summary>
		/// ジャンプ
		/// </summary>
		kJump,

		/// <summary>
		/// 防御状態
		/// </summary>
		kGuard,

		/// <summary>
		/// 攻撃状態
		/// </summary>
		kAttack,

		/// <summary>
		/// 遠距離攻撃
		/// </summary>
		kLongRangeAttack,

		/// <summary>
		/// ダメージ
		/// </summary>
		kDamage,

		/// <summary>
		/// やられ
		/// </summary>
		kDead,

		/// <summary>
		/// 種類数取得用
		/// </summary>
		kNum,
	};

	/// <summary>
	/// アニメーションの配列内の情報内訳
	/// </summary>
	struct AnimDef
	{
		/// <summary>
		/// 種別
		/// </summary>
		EnemyAnimSet::AnimKind kind;

		/// <summary>
		/// パスの配列のポインタ
		/// </summary>
		const char*  path;

		/// <summary>
		/// 実際に使うフレーム数
		/// </summary>
		int frame_count;

		/// <summary>
		/// 横分割数
		/// </summary>
		int div_x;  

		/// <summary>
		/// 縦分割数
		/// </summary>
		int div_y;  

		/// <summary>
		/// 1コマの幅
		/// </summary>
		int frame_width;

		/// <summary>
		/// 1コマの高さ
		/// </summary>
		int frame_height;

		/// <summary>
		/// 1フレームの秒数
		/// </summary>
		float interval;
	};

	/// <summary>
	/// アニメーションの種類
	/// </summary>
	static const int kAnimCount = static_cast<int>(AnimKind::kNum);

	/// <summary>
	/// アニメーションフレームの最大値、決め打ちで入れている
	/// </summary>
	static const int kMaxAnimFrameCount = 64;

public:
	/// <summary>
	/// [フレーム][アニメ種類]
	/// </summary>
	std::array<std::array<int, kMaxAnimFrameCount>, kAnimCount> anim_frames_{};

	/// <summary>
	/// アニメごとの実フレーム数
	/// </summary>
	std::array<int, kAnimCount> anim_frame_counts_{};

	/// <summary>
	/// アニメごとの画像横幅サイズ（フレーム1枚目から取得）
	/// </summary>
	std::array<int, kAnimCount> anim_width_{};

	/// <summary>
	/// アニメごとの画像高さサイズ（フレーム1枚目から取得）
	/// </summary>
	std::array<int, kAnimCount> anim_height_{};

	/// <summary>
	/// アニメごとの1コマ秒数
	/// </summary>
	std::array<float, kAnimCount> anim_interval_{};
};