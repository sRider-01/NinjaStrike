#define NOMINMAX //std::max,minを使うためにWindowsヘッダのマクロ定義をしないようにさせる
#include "DxLib.h"
#include "Player.h"
#include "DebugManager/DebugManager.h"
#include "Level/GameLevel.h"
#include "GameInfo.h"
#include "Effect/EffectManager.h"
#include "Collision/CollisionBase.h"
#include "Kunai/KunaiManager.h"
#include "Task/Task.h"
#include "Input/InputManager.h"
#include "Audio/AudioManager.h"
#include <algorithm>

/// <summary>
/// Player.cppで使う定数
/// </summary>
namespace
{
	/// <summary>
	/// 近距離攻撃SE
	/// </summary>
	const char* kCloseAttackSePath = "Assets/Audio/SE/CloseAttack.mp3";

	/// <summary>
	///遠距離攻撃SE
	/// </summary>
	const char* kLongRangeShotSePath = "Assets/Audio/SE/LongRangeShot.mp3";

	/// <summary>
	///被ダメージSE
	/// </summary>
	const char* kPlayerDamageSePath = "Assets/Audio/SE/PlayerDamage.mp3";

	/// <summary>
	/// ジャンプSE
	/// </summary>
	const char* kJumpSePath = "Assets/Audio/SE/Jump.mp3";

	/// <summary>
	/// 破壊可能タイルヒットSE
	/// </summary>
	const char* kHitableBreakSePath = "Assets/Audio/SE/HitableBreak.mp3";

	/// <summary>
	/// 破壊可能オブジェクト完全破壊SE
	/// </summary>
	const char* kHitableCompleteBreakSePath = "Assets/Audio/SE/HitableCompleteBreak.mp3";

	/// <summary>
	/// 最大HP
	/// </summary>
	const int kMaxHp = 10;

	/// <summary>
	/// タイルへのダメージ
	/// </summary>
	const int kPlayerAttackTileDamage = 1;

	/// <summary>
	/// 接触ダメージ
	/// </summary>
	const int kContactDamage = 1;

	/// <summary>
	/// 攻撃ダメージ
	/// </summary>
	const int kAttackDamage = 10;
	
	/// <summary>
	/// 被弾時の点滅状態の効果時間
	/// </summary>
	const float kBlinkDuration = 3.0f;
	
	/// <summary>
	/// 点滅の間隔
	/// </summary>
	const float kBlinkInterval = 0.05f;

	/// <summary>
	/// 当たり判定四角形のX座標を縮める値
	/// </summary>
	const float kShrinkX = 45.0f;

	/// <summary>
	/// 当たり判定四角形の上部を縮める値
	/// </summary>
	const float kShrinkTop = 25.0f;

	/// <summary>
	/// 当たり判定四角形の下部を縮める値
	/// </summary>
	const float kShrinkBottom = 2.0f;

	/// <summary>
	/// 接地判定に使う判定を下にずらす量
	/// </summary>
	const float kProbeCount = 2.0f;

	/// <summary>
	/// 自機の当たり判定円のX座標のオフセット
	/// </summary>
	const float kHitOffSetX = 85.0f;

	/// <summary>
	/// 自機の当たり判定円の中心X座標のオフセット
	/// </summary>
	const float kHitOffSetY = 60.0f;

	/// <summary>
	/// 自機の当たり判定円の半径
	/// </summary>
	const float kHitRadius = 30.0f;

	/// <summary>
	/// プレイヤーの手の位置X座標
	/// </summary>
	const float kHandOffsetX = 130.0f;

	/// <summary>
	/// プレイヤーの手の位置Y座標
	/// </summary>
	const float kHandOffsetY = 110.0f;

	/// <summary>
	/// 近距離攻撃当たり判定円の幅調整用
	/// </summary>
	const float kHitCircleWidth = 70.0f;

	/// <summary>
	/// 近距離攻撃当たり判定円のX座標オフセット
	/// </summary>
	const float kHitCircleOffsetX = 50.0f;

	/// <summary>
	/// 近距離攻撃の当たり判定円の高さの調整用
	/// </summary>		
	const float kCloseAttackHitCircleHeightRate = 0.7f;

	/// <summary>
	/// 近距離攻撃の当たり判定円の調整用
	/// </summary>
	const float kCloseAttackHitCircleRadiusRate = 0.6f;

	/// <summary>
	/// プレイヤーの歩行移動速度
	/// </summary>
	const int kSpeedPlayerWalk = 5;

	/// <summary>
	/// プレイヤーのダッシュ移動速度
	/// </summary>
	const int kSpeedPlayerRun = 10;

	/// <summary>
	/// 近距離攻撃のクールタイム（秒）
	/// </summary>
	const float kCloseAttackCooldownSeconds = 0.05f;

	/// <summary>
	/// 遠距離攻撃のクールタイム（秒）
	/// </summary>
	const float kLongRangeAttackCooldownSeconds = 0.10f;

	/// <summary>
	/// 近距離攻撃の移動ロック時間（秒）
	/// </summary>
	const float kCloseAttackLockSeconds = 0.05f;

	/// <summary>
	/// 画面外判定に使用する値
	/// </summary>
	const int kOutOfWindow = 30;

	/// <summary>
	/// アイドル状態の一コマに使用する秒数
	/// </summary>
	const float kAnimIdleInterval = 0.12f;

	/// <summary>
	/// 歩行状態の一コマに使用する秒数
	/// </summary>
	const float kAnimWalkInterval = 0.04f;

	/// <summary>
	/// ダッシュ状態の一コマに使用する秒数
	/// </summary>
	const float kAnimRunInterval = 0.03f;

	/// <summary>
	/// ジャンプの一コマに使用する秒数
	/// </summary>
	const float kAnimJumpInterval = 0.03f;

	/// <summary>
	/// 近距離攻撃の一コマに使用する秒数
	/// </summary>
	const float kAnimCloseAttackInterval = 0.005f;

	/// <summary>	
	/// 遠距離攻撃の一コマに使用する秒数
	/// </summary>
	const float kAnimLongRangeAttackInterval = 0.002f;

	/// <summary>
	/// やられアニメーションの一コマに使用する秒数
	/// </summary>
	const float kAnimDeadInterval = 0.05f;
	
	/// <summary>
	/// 重力の値
	/// </summary>
	const float kGravity = 1700.0f;

	/// <summary>
	/// ジャンプの初速度
	/// </summary>
	const float kJumpSpeed = 800.0f;

	/// <summary>
	/// ジャンプ入力バッファの有効時間（秒）
	/// この時間内に着地すれば即ジャンプする
	/// </summary>
	const float kJumpBufferSeconds = 0.1f;

	/// <summary>
	/// 着地キャンセルが可能になるまでのフレームの番目
	/// </summary>
	const int kLandingCancelFrames = 2;

	/// <summary>
	/// 空中でアニメーションを止めておくフレームの番目 
	/// </summary>
	const int kJumpHoldFrame = 7;

	/// <summary>
	/// 着地後に再生するフレームの番目
	/// </summary>
	const int kJumpLandStartFrame = 8;

	/// <summary>
	/// アイドル状態フレームのパスリスト
	/// </summary>
	const char* kPlayerIdleFramePaths[] =
	{
		"Assets/Image/Player/Idle/ninja_10KDStudios_idle_00000.png",
		"Assets/Image/Player/Idle/ninja_10KDStudios_idle_00001.png",
		"Assets/Image/Player/Idle/ninja_10KDStudios_idle_00002.png",
		"Assets/Image/Player/Idle/ninja_10KDStudios_idle_00003.png",
		"Assets/Image/Player/Idle/ninja_10KDStudios_idle_00004.png",
		"Assets/Image/Player/Idle/ninja_10KDStudios_idle_00005.png",
		"Assets/Image/Player/Idle/ninja_10KDStudios_idle_00006.png",
		"Assets/Image/Player/Idle/ninja_10KDStudios_idle_00007.png",
		"Assets/Image/Player/Idle/ninja_10KDStudios_idle_00008.png",
		"Assets/Image/Player/Idle/ninja_10KDStudios_idle_00009.png",
		"Assets/Image/Player/Idle/ninja_10KDStudios_idle_00010.png",
		"Assets/Image/Player/Idle/ninja_10KDStudios_idle_00011.png",
		"Assets/Image/Player/Idle/ninja_10KDStudios_idle_00012.png",
		"Assets/Image/Player/Idle/ninja_10KDStudios_idle_00013.png",
		"Assets/Image/Player/Idle/ninja_10KDStudios_idle_00014.png",
		"Assets/Image/Player/Idle/ninja_10KDStudios_idle_00015.png",
	};

	/// <summary>
	/// 歩行状態フレームのパスリスト
	/// </summary>
	const char* kPlayerWalkFramePaths[] =
	{
		"Assets/Image/Player/Walk/ninja_10KDStudios_walk_00000.png",
		"Assets/Image/Player/Walk/ninja_10KDStudios_walk_00001.png",
		"Assets/Image/Player/Walk/ninja_10KDStudios_walk_00002.png",
		"Assets/Image/Player/Walk/ninja_10KDStudios_walk_00003.png",
		"Assets/Image/Player/Walk/ninja_10KDStudios_walk_00004.png",
		"Assets/Image/Player/Walk/ninja_10KDStudios_walk_00005.png",
		"Assets/Image/Player/Walk/ninja_10KDStudios_walk_00006.png",
		"Assets/Image/Player/Walk/ninja_10KDStudios_walk_00007.png",
		"Assets/Image/Player/Walk/ninja_10KDStudios_walk_00008.png",
		"Assets/Image/Player/Walk/ninja_10KDStudios_walk_00009.png",
		"Assets/Image/Player/Walk/ninja_10KDStudios_walk_00010.png",
		"Assets/Image/Player/Walk/ninja_10KDStudios_walk_00011.png",
		"Assets/Image/Player/Walk/ninja_10KDStudios_walk_00012.png",
		"Assets/Image/Player/Walk/ninja_10KDStudios_walk_00013.png",
		"Assets/Image/Player/Walk/ninja_10KDStudios_walk_00014.png",
		"Assets/Image/Player/Walk/ninja_10KDStudios_walk_00015.png",
		"Assets/Image/Player/Walk/ninja_10KDStudios_walk_00016.png",
		"Assets/Image/Player/Walk/ninja_10KDStudios_walk_00017.png",
		"Assets/Image/Player/Walk/ninja_10KDStudios_walk_00018.png",
		"Assets/Image/Player/Walk/ninja_10KDStudios_walk_00019.png",
		"Assets/Image/Player/Walk/ninja_10KDStudios_walk_00020.png",
		"Assets/Image/Player/Walk/ninja_10KDStudios_walk_00021.png",
		"Assets/Image/Player/Walk/ninja_10KDStudios_walk_00022.png",
		"Assets/Image/Player/Walk/ninja_10KDStudios_walk_00023.png",
		"Assets/Image/Player/Walk/ninja_10KDStudios_walk_00024.png",
		"Assets/Image/Player/Walk/ninja_10KDStudios_walk_00025.png",
	};

	/// <summary>
	/// ダッシュ状態フレームのパスリスト
	/// </summary>
	const char* kPlayerRunFramePaths[] =
	{
		"Assets/Image/Player/Run/ninja_10KDStudios_run_00000.png",
		"Assets/Image/Player/Run/ninja_10KDStudios_run_00001.png",
		"Assets/Image/Player/Run/ninja_10KDStudios_run_00002.png",
		"Assets/Image/Player/Run/ninja_10KDStudios_run_00003.png",
		"Assets/Image/Player/Run/ninja_10KDStudios_run_00004.png",
		"Assets/Image/Player/Run/ninja_10KDStudios_run_00005.png",
		"Assets/Image/Player/Run/ninja_10KDStudios_run_00006.png",
		"Assets/Image/Player/Run/ninja_10KDStudios_run_00007.png",
		"Assets/Image/Player/Run/ninja_10KDStudios_run_00008.png",
		"Assets/Image/Player/Run/ninja_10KDStudios_run_00009.png",
		"Assets/Image/Player/Run/ninja_10KDStudios_run_00010.png",
		"Assets/Image/Player/Run/ninja_10KDStudios_run_00011.png",
		"Assets/Image/Player/Run/ninja_10KDStudios_run_00012.png",
		"Assets/Image/Player/Run/ninja_10KDStudios_run_00013.png",
		"Assets/Image/Player/Run/ninja_10KDStudios_run_00014.png",
		"Assets/Image/Player/Run/ninja_10KDStudios_run_00015.png",
		"Assets/Image/Player/Run/ninja_10KDStudios_run_00016.png",
	};

	/// <summary>
	/// ジャンプ状態フレームのパスリスト
	/// </summary>
	const char* kPlayerJumpFramePaths[] =
	{
		"Assets/Image/Player/Jump/ninja_10KDStudios_jump_00000.png",
		"Assets/Image/Player/Jump/ninja_10KDStudios_jump_00001.png",
		"Assets/Image/Player/Jump/ninja_10KDStudios_jump_00002.png",
		"Assets/Image/Player/Jump/ninja_10KDStudios_jump_00003.png",
		"Assets/Image/Player/Jump/ninja_10KDStudios_jump_00004.png",
		"Assets/Image/Player/Jump/ninja_10KDStudios_jump_00005.png",
		"Assets/Image/Player/Jump/ninja_10KDStudios_jump_00006.png",
		"Assets/Image/Player/Jump/ninja_10KDStudios_jump_00007.png",
		"Assets/Image/Player/Jump/ninja_10KDStudios_jump_00008.png",
		"Assets/Image/Player/Jump/ninja_10KDStudios_jump_00009.png",
		"Assets/Image/Player/Jump/ninja_10KDStudios_jump_00010.png",
		"Assets/Image/Player/Jump/ninja_10KDStudios_jump_00011.png",
		"Assets/Image/Player/Jump/ninja_10KDStudios_jump_00012.png",
		"Assets/Image/Player/Jump/ninja_10KDStudios_jump_00013.png",
		"Assets/Image/Player/Jump/ninja_10KDStudios_jump_00014.png",
		"Assets/Image/Player/Jump/ninja_10KDStudios_jump_00015.png",
	};

	/// <summary>
	/// 攻撃フレームのパスリスト
	/// </summary>
	const char* kPlayerAttackFramePaths[] =
	{
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00000.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00001.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00002.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00003.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00004.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00005.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00006.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00007.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00008.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00009.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00010.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00011.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00012.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00013.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00014.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00015.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00016.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00017.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00018.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00019.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00020.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00021.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00022.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00023.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00024.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00025.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00026.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00027.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00028.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00029.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00030.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00031.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00032.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00033.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00034.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00035.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00036.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00037.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00038.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00039.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00040.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00041.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00042.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00043.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00044.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00045.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00046.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00047.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00048.png",
		"Assets/Image/Player/Attack/ninja_10KDStudios_attack_00049.png",
	};


	/// <summary>
	/// やられフレームのパスリスト
	/// </summary>
	const char* kPlayerDeadFramePaths[] =
	{
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00000.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00001.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00002.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00003.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00004.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00005.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00006.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00007.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00008.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00009.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00010.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00011.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00012.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00013.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00014.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00015.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00016.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00017.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00018.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00019.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00020.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00021.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00022.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00023.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00024.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00025.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00026.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00027.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00028.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00029.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00030.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00031.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00032.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00033.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00034.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00035.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00036.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00037.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00038.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00039.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00040.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00041.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00042.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00043.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00044.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00045.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00046.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00047.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00048.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00049.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00050.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00051.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00052.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00053.png",
		"Assets/Image/Player/Dead/ninja_10KDStudios_death_00054.png",

	};


	/// <summary>
	/// アニメーションの配列
	/// </summary>
	const Player::PlayerAnimDef player_anim_defs[] =
	{
		{ Player::AnimKind::kIdle, kPlayerIdleFramePaths, static_cast<int>(sizeof(kPlayerIdleFramePaths) / sizeof(kPlayerIdleFramePaths[0])), kAnimIdleInterval },
		{ Player::AnimKind::kWalk, kPlayerWalkFramePaths, static_cast<int>(sizeof(kPlayerWalkFramePaths) / sizeof(kPlayerWalkFramePaths[0])), kAnimWalkInterval },
		{ Player::AnimKind::kRun,  kPlayerRunFramePaths,  static_cast<int>(sizeof(kPlayerRunFramePaths) / sizeof(kPlayerRunFramePaths[0])),  kAnimRunInterval },
		{ Player::AnimKind::kJump, kPlayerJumpFramePaths, static_cast<int>(sizeof(kPlayerJumpFramePaths) / sizeof(kPlayerJumpFramePaths[0])), kAnimJumpInterval },
		{ Player::AnimKind::kCloseAttack, kPlayerAttackFramePaths, static_cast<int>(sizeof(kPlayerAttackFramePaths) / sizeof(kPlayerAttackFramePaths[0])), kAnimCloseAttackInterval },
		{ Player::AnimKind::kLongRangeAttack, kPlayerAttackFramePaths, static_cast<int>(sizeof(kPlayerAttackFramePaths) / sizeof(kPlayerAttackFramePaths[0])), kAnimLongRangeAttackInterval },
		{ Player::AnimKind::kDead, kPlayerDeadFramePaths, static_cast<int>(sizeof(kPlayerDeadFramePaths) / sizeof(kPlayerDeadFramePaths[0])), kAnimDeadInterval },
	};

	/// <summary>
	/// プレイヤーの足場判定に使う「左右2点」
	/// </summary>
	const int kTwoPoint = 2;

	/// <summary>
	/// 足場判定に使う画像の少し内側に判定を持ってくるマージン
	/// </summary>
	const float kMargin = 4.0f;

	/// <summary>
	/// 落下判定に使う画面下の余白
	/// </summary>
	const float kFallOutMargin = 50.0f;

	/// <summary>
	/// 落下復帰時に少し左へ戻す量
	/// </summary>
	const float kRespawnLeftOffset = 350.0f;

	/// <summary>
	/// 値を四分の一にする
	/// </summary>
	const float kQuarter = 0.25f;

	/// <summary>
	/// 空のタイルID
	/// </summary>
	const int kEmptyTileId = 0;

	/// <summary>
	/// 数値を半分にする
	/// </summary>
	const float kHalf = 0.5f;

	/// <summary>
	/// 未ロードであることを表す
	/// </summary>
	const int kUnLoaded = -1;
}

/// <summary>
/// コンストラクタ
/// </summary>
Player::Player()
	: player_state_(PlayerState::kNone)
	, max_hp_(kMaxHp)
	, blink_duration_(0)
	, is_flip_x_(false)
	, current_anim_(AnimKind::kIdle)
	, current_frame_(0)
	, anim_time_(0.0f)
	, scale_(1.0f)
	, x_player_(static_cast<float>(GameInfo::GetInstance()->GetCenterX()))
	, y_player_(static_cast<float>(GameInfo::GetInstance()->GetResolutionY()))
	, attack_cooldown_seconds_(0.0f)
	, attack_lock_seconds_(0.0f)
	, respawn_x_(static_cast<float>(GameInfo::GetInstance()->GetCenterX()))
	, respawn_y_(0.0f)
{
}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{
}

/// <summary>
/// 初期化処理
/// </summary>
void Player::Initialize()
{
	//配列の要素すべてを「-1」未ロードにする
	for (int i = 0; i < kAnimCount; ++i)
	{
		for (int f = 0; f < kMaxAnimFrameCount; ++f)
		{
			anim_frames_[i][f] = kUnLoaded;
		}
	}

	//モーションがいくつあるか
	const int player_anim_def_count = static_cast<int>(sizeof(player_anim_defs) / sizeof(player_anim_defs[0]));


	for (int i = 0; i < player_anim_def_count; ++i)
	{
		const int anim_index = GetAnimIndex(player_anim_defs[i].kind);

		anim_frame_counts_[anim_index] = player_anim_defs[i].count;
		anim_interval_[anim_index] = player_anim_defs[i].interval;

		//上限チェック（配列越えの防止）
		const int load_count = std::min(player_anim_defs[i].count, Player::kMaxAnimFrameCount);

		for (int f = 0; f < load_count; ++f)
		{
			//画像のロード
			const int handle = LoadGraph(player_anim_defs[i].paths[f]);
			anim_frames_[anim_index][f] = handle;
		}

		//サイズ取得（1枚目がロードできている場合）
		if (anim_frames_[anim_index][0] >= 0)
		{
			GetGraphSize(anim_frames_[anim_index][0], &anim_width_[anim_index], &anim_height_[anim_index]);
		}
	}

	current_anim_ = AnimKind::kIdle;
	current_hp_ = kMaxHp;
	current_frame_ = 0;
	anim_time_ = 0.0f;
	is_flip_x_ = false;
	is_alive_ = true;
	is_jumping_ = false;
	is_close_attack_ = false;
	is_long_range_attack_ = false;
	velocity_y_ = 0.0f;

	close_attack_se_handle_ = AudioManager::GetInstance()->LoadSe(kCloseAttackSePath);
	jump_se_handle_ = AudioManager::GetInstance()->LoadSe(kJumpSePath);
	long_range_shot_se_handle_ = AudioManager::GetInstance()->LoadSe(kLongRangeShotSePath);
	damage_se_handle_ = AudioManager::GetInstance()->LoadSe(kPlayerDamageSePath);

	hitable_break_se_handle_ = AudioManager::GetInstance()->LoadSe(kHitableBreakSePath);
	hitable_complete_break_se_handle_ =
		AudioManager::GetInstance()->LoadSe(kHitableCompleteBreakSePath);
}

/// <summary>
/// 後処理
/// </summary>
void Player::Finalize()
{
	for (int i = 0; i < kAnimCount; ++i)
	{
		for (int f = 0; f < kMaxAnimFrameCount; ++f)
		{
			if (anim_frames_[i][f] != kUnLoaded)
			{
				DeleteGraph(anim_frames_[i][f]);
				anim_frames_[i][f] = kUnLoaded;
			}
		}
	}

	AudioManager::GetInstance()->DeleteSe(close_attack_se_handle_);
	AudioManager::GetInstance()->DeleteSe(jump_se_handle_);
	AudioManager::GetInstance()->DeleteSe(long_range_shot_se_handle_);
	AudioManager::GetInstance()->DeleteSe(damage_se_handle_);

	AudioManager::GetInstance()->DeleteSe(hitable_break_se_handle_);
	AudioManager::GetInstance()->DeleteSe(hitable_complete_break_se_handle_);
}

/// <summary>
/// 被弾処理
/// </summary>
void Player::TakeDamage(int damage)
{
	//blink_durationが0でない、点滅無敵状態の時
	if (blink_duration_ > 0.0f)
	{
		//ダメージ処理をしない
		return;
	}

	current_hp_ -= damage;

	AudioManager::GetInstance()->PlaySe(damage_se_handle_);

	if (current_hp_ <= 0)
	{
		current_hp_ = 0;
		//やられた処理を入れる
		OnDead();
	}

	//点滅無敵を開始する
	blink_duration_ = kBlinkDuration;
	blink_interval_timer_ = 0.0f;
	is_visible_ = true;
}

/// <summary>
/// 点滅無敵の更新処理
/// </summary>
void Player::UpdateInvisible(float delta_time)
{
	//点滅無敵の効果時間をデクリメント
	blink_duration_ -= delta_time;

	//点滅無敵の効果時間がない またはHP0 なら
	if (blink_duration_ <= 0.0f || current_hp_<= 0)
	{
		//初期状態にして返す
		blink_duration_ = 0.0f;
		blink_interval_timer_ = 0.0f;
		is_visible_ = true;
		return;
	}

	//点滅描画間隔を進める
	blink_interval_timer_ += delta_time;

	//点滅の間隔毎に
	if (blink_interval_timer_ >= kBlinkInterval)
	{
		//点滅間隔を進める
		blink_interval_timer_ -= kBlinkInterval;

		//描画するかの情報を反転
		is_visible_ = !is_visible_;
	}
}

/// <summary>
/// やられ処理
/// </summary>
void Player::OnDead()
{

	SetAnim(AnimKind::kDead);

}


/// <summary>
/// ジャンプ開始処理
/// </summary>
void Player::StartJump()
{
	//ジャンプ,着地中なら
	if (is_jumping_ || is_landing_)
	{
		//何もしない
		return;
	}

	is_jumping_ = true;
	is_landing_ = false;

	if (is_run_)
	{
		air_move_speed_ = kSpeedPlayerRun;
	}
	else
	{
		air_move_speed_ = kSpeedPlayerWalk;
	}

	//上に向かって飛ぶ
	velocity_y_ = -kJumpSpeed;

	AudioManager::GetInstance()->PlaySe(jump_se_handle_);
}

/// <summary>
/// 近距離攻撃開始処理
/// </summary>
void Player::StartCloseAttack()
{
	if (is_close_attack_)
	{
		return;
	}

	is_close_attack_ = true;
	attack_facing_left_ = facing_left_;

	is_tile_damage_in_close_attack_ = false;

	//クールタイムをセット
	attack_cooldown_seconds_ = kCloseAttackCooldownSeconds;

	//移動ロックをセット
	attack_lock_seconds_ = std::max(attack_lock_seconds_, kCloseAttackLockSeconds);


	const float hand_x = x_player_ + kHandOffsetX;
	const float hand_y = y_player_ + kHandOffsetY;

	effect_manager_->EffectAct(EffectManager::EffectOwner::kPlayer,
		EffectManager::EffectId::kSlash,
		hand_x, hand_y, attack_facing_left_);

	AudioManager::GetInstance()->PlaySe(close_attack_se_handle_);

}

/// <summary>
/// 遠距離攻撃開始処理
/// </summary>
void Player::StartLongRangeAttack()
{
	if (is_long_range_attack_)
	{
		return;
	}

	is_long_range_attack_ = true;
	attack_facing_left_ = facing_left_;

	//クールタイムをセット
	attack_cooldown_seconds_ = kLongRangeAttackCooldownSeconds;
	
	const float hand_x = x_player_ + kHandOffsetX;
	const float hand_y = y_player_ + kHandOffsetY;

	kunai_manager_->Fire(KunaiManager::KunaiKind::kPlayerKunai, hand_x, hand_y, attack_facing_left_);

	AudioManager::GetInstance()->PlaySe(long_range_shot_se_handle_);
}

/// <summary>
/// 当たり判定四角形を作る
/// </summary>
CollisionBase::RectF Player::MakeBodyRect()
{
	//今のフレームの幅、高さを取得
	int img_width = 0;
	int img_height = 0;
	GetCurrentFrameSize(img_width, img_height);

	const float body_width = static_cast<float>(img_width) * scale_;
	const float body_height = static_cast<float>(img_height) * scale_;

	//引っかかり防止のため四角形を少し縮める
	const float shrink_x = kShrinkX;
	const float shrink_top = kShrinkTop;
	const float shrink_bottom = kShrinkBottom;

	CollisionBase::RectF rect{};
	rect.left = x_player_ + shrink_x;
	rect.top = y_player_ + shrink_top;
	rect.right = x_player_ + body_width - shrink_x;
	rect.bottom = y_player_ + body_height - shrink_bottom;
	return rect;
}

/// <summary>
/// 壁との当たり判定処理
/// </summary>
void Player::WallCollision(float move_x)
{
	if (move_x == 0.0f)
	{
		return;
	}


	if (game_level_ == nullptr)
	{
		return;
	}

	TileMap* tile_map = game_level_->GetTileMap();
	TileDatabase* tile_db = game_level_->GetTileDatabase();
	TileSet* tile_set = game_level_->GetTileSet();

	if (tile_map == nullptr || tile_db == nullptr || tile_set == nullptr)
	{
		return;
	}

	//タイルのサイズ取得
	const int tile_width = tile_set->GetTileWidth();
	const int tile_height = tile_set->GetTileHeight();
	if (tile_width <= 0 || tile_height <= 0)
	{
		return;
	}

	//マップの大きさ取得
	const int map_width = tile_map->GetWidth();
	const int map_height = tile_map->GetHeight();
	if (map_width <= 0 || map_height <= 0)
	{
		return;
	}

	CollisionBase::RectF body = MakeBodyRect();

	int start_x = static_cast<int>(body.left) / tile_width;
	int end_x = static_cast<int>(body.right) / tile_width;
	int start_y = static_cast<int>(body.top) / tile_height;
	int end_y = static_cast<int>(body.bottom) / tile_height;

	//ワールド座標内に丸める
	start_x = std::clamp(start_x, 0, map_width - 1);
	end_x = std::clamp(end_x, 0, map_width - 1);
	start_y = std::clamp(start_y, 0, map_height - 1);
	end_y = std::clamp(end_y, 0, map_height - 1);

	for (int tile_y = start_y; tile_y <= end_y; tile_y++)
	{
		for (int tile_x = start_x; tile_x <= end_x; tile_x++)
		{
			//タイルIDを取得
			const int tile_id = tile_map->GetTileId(tile_x, tile_y);

			//地面判定でないなら次へ
			if (!tile_db->IsSolid(tile_id))
			{
				continue; 
			}

			//タイルの四角形を作成
			const float tile_left = static_cast<float>(tile_x * tile_width);
			const float tile_right = tile_left + static_cast<float>(tile_width);
			const float tile_top = static_cast<float>(tile_y * tile_height);
			const float tile_bottom = tile_top + static_cast<float>(tile_height);

			//AABB重なり判定
			const bool overlap =
				(body.left < tile_right) && (body.right > tile_left) &&
				(body.top < tile_bottom) && (body.bottom > tile_top);

			//重なっていないなら次へ
			if (!overlap)
			{
				continue;
			}

			//壁にぶつかった後の押し戻し処理
			if (move_x > 0.0f)
			{
				//右移動：タイルの左に押し戻す
				const float body_width = body.right - body.left;
				x_player_ = tile_left - (body_width + kShrinkX);
			}
			else
			{
				//左移動：タイルの右に押し戻す
				x_player_ = tile_right - kShrinkX;
			}

			//押し戻した後の四角形に更新しておく
			body = MakeBodyRect();

		}
	}
}

/// <summary>
/// 地面との当たり判定処理
/// </summary>
bool Player::GroundCollision(float move_y)
{
	if (move_y == 0.0f)
	{
		return false;
	}

	if (game_level_ == nullptr)
	{
		return false;
	}

	is_on_ground_ = false;

	TileMap* tile_map = game_level_->GetTileMap();
	TileDatabase* tile_db = game_level_->GetTileDatabase();
	TileSet* tile_set = game_level_->GetTileSet();

	if (tile_map == nullptr || tile_db == nullptr || tile_set == nullptr)
	{
		return false;
	}

	//タイルのサイズ取得
	const int tile_width = tile_set->GetTileWidth();
	const int tile_height = tile_set->GetTileHeight();
	if (tile_width <= 0 || tile_height <= 0)
	{
		return false;
	}

	//マップの大きさ取得
	const int map_width = tile_map->GetWidth();
	const int map_height = tile_map->GetHeight();
	if (map_width <= 0 || map_height <= 0)
	{
		return false;
	}

	CollisionBase::RectF body = MakeBodyRect();

	int start_x = static_cast<int>(body.left) / tile_width;
	int end_x = static_cast<int>(body.right) / tile_width;
	int start_y = static_cast<int>(body.top) / tile_height;
	int end_y = static_cast<int>(body.bottom) / tile_height;

	//ワールド座標内に丸める
	start_x = std::clamp(start_x, 0, map_width - 1);
	end_x = std::clamp(end_x, 0, map_width - 1);
	start_y = std::clamp(start_y, 0, map_height - 1);
	end_y = std::clamp(end_y, 0, map_height - 1);

	for (int tile_y = start_y; tile_y <= end_y; tile_y++)
	{
		for (int tile_x = start_x; tile_x <= end_x; tile_x++)
		{
			//タイルIDを取得
			const int tile_id = tile_map->GetTileId(tile_x, tile_y);

			//地面判定でないなら次へ
			if (!tile_db->IsSolid(tile_id))
			{
				continue;
			}

			//タイルの四角形を作成
			const float tile_left = static_cast<float>(tile_x * tile_width);
			const float tile_right = tile_left + static_cast<float>(tile_width);
			const float tile_top = static_cast<float>(tile_y * tile_height);
			const float tile_bottom = tile_top + static_cast<float>(tile_height);

			//AABB重なり判定
			const bool overlap =
				(body.left < tile_right) && (body.right > tile_left) &&
				(body.top < tile_bottom) && (body.bottom > tile_top);

			//重なっていないなら次へ
			if (!overlap)
			{
				continue;
			}

			//壁にぶつかった後の押し戻し処理
			if (move_y > 0.0f)
			{
				//落下処理
				const float body_height = body.bottom - body.top;

				y_player_ = tile_top - (body_height + kShrinkTop);

				velocity_y_ = 0.0f;
				is_on_ground_ = true;

				return true;
			}
			else
			{
				//上昇処理
				y_player_ = tile_bottom - kShrinkTop;

				velocity_y_ = 0.0f;

				return false;
			}
		}
	}

	return false;
}


/// <summary>
/// 今地面に接しているか？(probe_yだけ下にずらして判定をとる)
/// </summary>
bool Player::IsOnGroundByProbe(float probe_y)
{
	if (probe_y <= 0.0f)
	{
		return false;
	}

	if (game_level_ == nullptr)
	{
		return false;
	}

	TileMap* tile_map = game_level_->GetTileMap();
	TileDatabase* tile_db = game_level_->GetTileDatabase();
	TileSet* tile_set = game_level_->GetTileSet();

	if (tile_map == nullptr || tile_db == nullptr || tile_set == nullptr)
	{
		return false;
	}

	const int tile_width = tile_set->GetTileWidth();
	const int tile_height = tile_set->GetTileHeight();
	if (tile_width <= 0 || tile_height <= 0)
	{
		return false;
	}

	const int map_width = tile_map->GetWidth();
	const int map_height = tile_map->GetHeight();
	if (map_width <= 0 || map_height <= 0)
	{
		return false;
	}

	const CollisionBase::RectF body = MakeBodyRect();

	//足元だけを probe_y 下にずらして判定する
	CollisionBase::RectF foot_probe{};
	foot_probe.left = body.left;
	foot_probe.right = body.right;
	foot_probe.top = body.bottom;
	foot_probe.bottom = body.bottom + probe_y;

	int start_x = static_cast<int>(foot_probe.left) / tile_width;
	int end_x = static_cast<int>(foot_probe.right) / tile_width;
	int start_y = static_cast<int>(foot_probe.top) / tile_height;
	int end_y = static_cast<int>(foot_probe.bottom + probe_y) / tile_height;

	start_x = std::clamp(start_x, 0, map_width - 1);
	end_x = std::clamp(end_x, 0, map_width - 1);
	start_y = std::clamp(start_y, 0, map_height - 1);
	end_y = std::clamp(end_y, 0, map_height - 1);

	for (int tile_y = start_y; tile_y <= end_y; ++tile_y)
	{
		for (int tile_x = start_x; tile_x <= end_x; ++tile_x)
		{
			const int tile_id = tile_map->GetTileId(tile_x, tile_y);
			if (!tile_db->IsSolid(tile_id))
			{
				continue;
			}

			const float tile_left = static_cast<float>(tile_x * tile_width);
			const float tile_right = tile_left + static_cast<float>(tile_width);
			const float tile_top = static_cast<float>(tile_y * tile_height);
			const float tile_bottom = tile_top + static_cast<float>(tile_height);

			const bool overlap =
				(foot_probe.left < tile_right) && (foot_probe.right > tile_left) &&
				(foot_probe.top < tile_bottom) && (foot_probe.bottom > tile_top);

			if (overlap)
			{
				return true;
			}
		}
	}

	return false;
}
/// <summary>
/// 当たり判定円を作成する
/// </summary>
CollisionBase::CircleF Player::MakeCloseAttackHitCircle(bool is_facing_left) const
{
	//今のフレームサイズ（=見た目の幅,高さ）
	int img_width = 0;
	int img_height = 0;
	GetCurrentFrameSize(img_width, img_height);

	const float player_width = static_cast<float>(img_width) * scale_;
	const float player_height = static_cast<float>(img_height) * scale_;

	const float radius = (player_width * kCloseAttackHitCircleRadiusRate);

	CollisionBase::CircleF circle{};
	circle.radius = radius;

	if (!is_facing_left)
	{
		//右向き：プレイヤー右端から前方へ
		const float circle_left = x_player_ + player_width;
		circle.center_x = circle_left + (kHitCircleWidth * kQuarter);
	}
	else
	{
		//左向き：プレイヤー左端から前方へ
		const float circle_right = x_player_ + kHitCircleOffsetX;
		circle.center_x = circle_right - (kHitCircleWidth * kHalf);
	}

	circle.center_y = y_player_ + player_height * kCloseAttackHitCircleHeightRate;

	return circle;
}

/// <summary>
/// 自機の当たり判定円を作成する
/// </summary>
CollisionBase::CircleF Player::GetHitCircle() 
{
	//今のフレームサイズ（=見た目の幅,高さ）
	int img_width = 0;
	int img_height = 0;
	GetCurrentFrameSize(img_width, img_height);

	CollisionBase::CircleF circle{};
	circle.center_x = x_player_+ img_width * scale_ - kHitOffSetX;
	circle.center_y = y_player_ + img_height* scale_ - kHitOffSetY;
	circle.radius = kHitRadius * scale_;
	return circle;
}

/// <summary>
/// 攻撃の当たり判定円を取得する（外部クラス用）
/// </summary>
bool Player::TryGetCloseAttackHitCircle(CollisionBase::CircleF& out_circle)
{
	if (!is_close_attack_)
	{
		return false;
	}

	//当たり判定円を作って引数に入れる
	out_circle = MakeCloseAttackHitCircle(attack_facing_left_);

	return true;
}

/// <summary>
/// 攻撃を受け取る属性のタイルにヒットしたかを見る
/// </summary>
bool Player::HitHitableTilesByCircle(const CollisionBase::CircleF& hit_circle,int damage)
{
	if (game_level_ == nullptr)
	{
		return false;
	}

	TileMap* tile_map = game_level_->GetTileMap();
	TileDatabase* tile_db = game_level_->GetTileDatabase();
	TileSet* tile_set = game_level_->GetTileSet();

	if (tile_map == nullptr || tile_db == nullptr || tile_set == nullptr)
	{
		return false;
	}

	//タイルのサイズ取得
	const int tile_width = tile_set->GetTileWidth();
	const int tile_height = tile_set->GetTileHeight();
	if (tile_width <= 0 || tile_height <= 0)
	{
		return false;
	}

	//マップの大きさ取得
	const int map_width = tile_map->GetWidth();
	const int map_height = tile_map->GetHeight();
	if (map_width <= 0 || map_height <= 0)
	{
		return false;
	}

	const float left = hit_circle.center_x - hit_circle.radius;
	const float right = hit_circle.center_x + hit_circle.radius;
	const float top = hit_circle.center_y - hit_circle.radius;
	const float bottom = hit_circle.center_y + hit_circle.radius;

	int start_tile_x = WorldToTile(left, tile_width);
	int end_tile_x = WorldToTile(right - 1.0f, tile_width);
	int start_tile_y = WorldToTile(top, tile_height);
	int end_tile_y = WorldToTile(bottom - 1.0f, tile_height);

	start_tile_x = std::clamp(start_tile_x, 0, map_width - 1);
	end_tile_x = std::clamp(end_tile_x, 0, map_width - 1);
	start_tile_y = std::clamp(start_tile_y, 0, map_height - 1);
	end_tile_y = std::clamp(end_tile_y, 0, map_height - 1);


	bool hit_any_tile = false;

	for (int tile_y = start_tile_y; tile_y <= end_tile_y; ++tile_y)
	{
		for (int tile_x = start_tile_x; tile_x <= end_tile_x; ++tile_x)
		{
			//対象のタイルのIDを取得
			const int current_tile_id = tile_map->GetTileId(tile_x, tile_y);

			//Hitable属性がなければ次へ
			if (!tile_db->IsHitable(current_tile_id) && !tile_db->IsSolid(current_tile_id))
			{
				continue;
			}

			//対象のタイルの四角形を作成
			CollisionBase::RectF tile_rect;
			tile_rect.left = static_cast<float>(tile_x * tile_width);
			tile_rect.top = static_cast<float>(tile_y * tile_height);
			tile_rect.right = tile_rect.left + static_cast<float>(tile_width);
			tile_rect.bottom = tile_rect.top + static_cast<float>(tile_height);

			//当たってなければ次へ
			if (!CollisionBase::IsCircleRectOverlap(hit_circle, tile_rect))
			{
				continue;
			}

			//攻撃当たり判定四角形と対象のタイルの四角形で重なっている箇所のタイルIDを0(空タイル)にする
			if (tile_db->IsHitable(current_tile_id))
			{
				const bool is_destroyed = tile_map->ApplyDamageToTile(
					tile_x, tile_y, damage, *tile_db);

				if (is_destroyed)
				{
					//HP0で破壊された → 完全破壊SE
					AudioManager::GetInstance()->PlaySe(hitable_complete_break_se_handle_);
				}
				else
				{
					//ヒットしたがまだ残っている → 通常ヒットSE
					AudioManager::GetInstance()->PlaySe(hitable_break_se_handle_);
				}
			}

			//破壊不可タイルでも「当たった」ことにする
			hit_any_tile = true;
		}
	}

	//タイルに当たったかどうかを返す
	return hit_any_tile;
}



/// <summary>
/// 座標をセットする
/// </summary>
void Player::SetPosition(float x, float y)
{
	x_player_ = x;
	y_player_ = y;
}

/// <summary>
/// 毎フレームの更新処理
/// </summary>
/// <param name="delta_time">前回実行フレームからの経過時間（秒）</param>
void Player::Update(float delta_time)
{
	if (game_level_ != nullptr && game_level_->IsStageChanging())
	{
		return;
	}

	if (game_level_ != nullptr && game_level_->IsGameEnded())
	{
		//終了状態ではアニメだけ更新して入力を受け付けない
		UpdateAnimation(delta_time);
		return;
	}

	//ダイアログ中の処理
	if (game_level_ != nullptr && game_level_->IsInDialogue())
	{
		//重力を加算
		if (!is_on_ground_)
		{
			velocity_y_ += kGravity * delta_time;
		}
		else
		{
			if (velocity_y_ > 0.0f)
			{
				velocity_y_ = 0.0f;
			}
		}

		//縦移動・着地判定
		const float prev_y = y_player_;
		y_player_ += velocity_y_ * delta_time;
		const float moved_y = y_player_ - prev_y;
		const bool landed_this_frame = GroundCollision(moved_y);

		//着地処理
		if (landed_this_frame)
		{
			is_flip_x_ = facing_left_;
			is_jumping_ = false;
			is_landing_ = true;
			landing_frame_count_ = 0;

			SetAnim(AnimKind::kJump);
			const int anim_count = GetCurrentAnimFrameCount();
			const int start = std::clamp(kJumpLandStartFrame, 0, std::max(anim_count - 1, 0));
			current_frame_ = start;
			anim_time_ = 0.0f;
		}

		//描画座標の更新
		GetCurrentFrameSize(render_current_width_, render_current_height_);
		render_draw_width_ = static_cast<float>(render_current_width_) * scale_;
		render_draw_height_ = static_cast<float>(render_current_height_) * scale_;
		base_center_x_ = x_player_ + render_draw_width_ * kHalf;
		base_center_y_ = y_player_ + render_draw_height_ * kHalf;
		base_image_center_x_ = static_cast<float>(render_current_width_) * kHalf;
		base_image_center_y_ = static_cast<float>(render_current_height_) * kHalf;
		camera_x_ = static_cast<float>(game_level_->GetCameraLeftX());
		camera_y_ = static_cast<float>(game_level_->GetCameraTopY());
		screen_center_x_ = base_center_x_ - camera_x_;
		screen_center_y_ = base_center_y_ - camera_y_;

		//アニメ更新
		UpdateAnimation(delta_time);

		if (!is_jumping_ && !is_landing_)
		{
			SetAnim(AnimKind::kIdle);
		}
		return;
	}

	//死んでいたら
	if (!is_alive_)
	{
		//何もしない
		return;
	}

	//HP0なら（やられアニメーションは見せる、操作は受け付けない）
	if (current_hp_ <= 0)
	{
		SetAnim(AnimKind::kDead);
		UpdateAnimation(delta_time);
		return;
	}

	//クールタイム減算
	attack_lock_seconds_ = std::max(0.0f, attack_lock_seconds_ - delta_time);
	attack_cooldown_seconds_ = std::max(0.0f, attack_cooldown_seconds_ - delta_time);

	//攻撃硬直時間があるかつ地上ならば移動ロックがTrue
	const bool is_action_locked = (attack_lock_seconds_ > 0.0f) && is_on_ground_;
	
	//点滅無敵の更新
	UpdateInvisible(delta_time);

	InputManager* input = InputManager::GetInstance();

	bool is_left = false;
	bool is_right = false;
	bool push_jump = false;
	bool push_close_attack = false;
	bool push_long_attack = false;
	bool down_run = false;

	if (!is_action_locked)
	{
		is_left = input->IsDown(InputManager::GameKeyKind::kLeft);
		is_right = input->IsDown(InputManager::GameKeyKind::kRight);

		push_jump = input->IsPushThisFrame(InputManager::GameKeyKind::kPlayerJump);
		push_close_attack = input->IsPushThisFrame(InputManager::GameKeyKind::kPlayerCloseAttack);
		push_long_attack = input->IsPushThisFrame(InputManager::GameKeyKind::kPlayerLongRangeAttack);

		down_run = input->IsDown(InputManager::GameKeyKind::kPlayerRun);
	}

	//歩行しているか
	const bool is_move = (is_left || is_right);

	//ロック中は向き更新しない
	if (!is_action_locked)
	{
		is_input_left_ = (is_left && !is_right);
		is_input_right_ = (is_right && !is_left);

		if (is_input_left_)
		{
			facing_left_ = true;
		}
		if (is_input_right_)
		{
			facing_left_ = false;
		}
	}

	//ダッシュ状態か？
	is_run_ = is_move && down_run;


	//ジャンプ処理
	//ジャンプ入力バッファを更新
	if (push_jump)
	{
		//ジャンプを押した瞬間にバッファをセット
		jump_buffer_seconds_ = kJumpBufferSeconds;
	}
	else
	{
		//押していないフレームはバッファを減らす
		jump_buffer_seconds_ = std::max(0.0f, jump_buffer_seconds_ - delta_time);
	}

	//ジャンプ開始（バッファが残っていて地上にいれば）
	if (jump_buffer_seconds_ > 0.0f && !is_jumping_ && !is_landing_)
	{
		StartJump();
		//ジャンプしたらバッファを消費
		jump_buffer_seconds_ = 0.0f;
	}

	//横移動について
	//速度設定
	int speed = 0;
	if (!is_on_ground_)
	{
		speed = air_move_speed_;
	}
	else if (is_run_)
	{
		speed = kSpeedPlayerRun;
	}
	else
	{
		speed = kSpeedPlayerWalk;
	}

	const float prev_x = x_player_;

	//移動
	if (is_left)
	{
		MoveLeft(speed);
	}
	else if (is_right)
	{
		MoveRight(speed);
	}
	const float moved_x = x_player_ - prev_x;

	//壁判定を更新
	WallCollision(moved_x);

	const bool was_on_ground = is_on_ground_;
	if (was_on_ground)
	{
		is_on_ground_ = IsOnGroundByProbe(kProbeCount);
	}

	if (was_on_ground && !is_on_ground_)
	{
		if (is_run_)
		{
			air_move_speed_ = kSpeedPlayerRun;
		}
		else
		{
			air_move_speed_ = kSpeedPlayerWalk;
		}

		is_jumping_ = true;

		is_landing_ = false;

		if (velocity_y_ < 0.0f)
		{
			velocity_y_ = 0.0f;
		}
	}

	//縦移動について
	//地面にいないなら常に重力が働く
	if (!is_on_ground_)
	{
		velocity_y_ += kGravity * delta_time;
	}
	else
	{
		//接地中に下向き速度が残っていたら0にする
		if (velocity_y_ > 0.0f)
		{
			velocity_y_ = 0.0f;
		}
	}

	const float prev_y = y_player_;

	y_player_ += velocity_y_ * delta_time;

	const float moved_y = y_player_ - prev_y;

	const bool landed_this_frame = GroundCollision(moved_y);

	//着地処理
	if (landed_this_frame)
	{
		is_flip_x_ = facing_left_;
		is_jumping_ = false;

		is_landing_ = true;
		landing_frame_count_ = 0;

		SetAnim(AnimKind::kJump);
		const int count = GetCurrentAnimFrameCount();
		const int start = std::clamp(kJumpLandStartFrame, 0, std::max(count - 1, 0));
		current_frame_ = start;
		anim_time_ = 0.0f;
	}

	//着地キャンセル判定
	//is_landingがtrueの間、フレームをカウントして
	//一定フレーム後にバッファが残っていればジャンプキャンセル
	if (is_landing_)
	{
		landing_frame_count_++;

		if (landing_frame_count_ >= kLandingCancelFrames &&
			jump_buffer_seconds_ > 0.0f)
		{
			//着地モーションをキャンセルしてジャンプ
			is_landing_ = false;
			landing_frame_count_ = 0;
			StartJump();
			jump_buffer_seconds_ = 0.0f;
		}
	}

	//リスポーン地点更新
	UpdateRespawnPoint();

	//近距離攻撃開始
	if (push_close_attack)
	{
		//近距離、遠距離攻撃をしていないかつクールタイムが終わっている
		const bool can_start_close_attack =(!is_close_attack_) &&
										   (!is_long_range_attack_) &&
										   (attack_cooldown_seconds_ <= 0.0f) &&
										   (!is_action_locked);

		if (can_start_close_attack)
		{
			StartCloseAttack();
		}
	}
	//遠距離攻撃開始
	else if (push_long_attack)
	{
		//近距離、遠距離攻撃をしていないかつクールタイムが終わっている
		const bool can_start_long_attack =(!is_close_attack_) &&
										  (!is_long_range_attack_) &&
										  (attack_cooldown_seconds_ <= 0.0f) &&
										  (!is_action_locked);

		if (can_start_long_attack)
		{
			StartLongRangeAttack();
		}
	}

	//アニメ切り替え
	
	if (is_close_attack_)
	{
		SetAnim(AnimKind::kCloseAttack);
	}
	else if (is_long_range_attack_)
	{
		SetAnim(AnimKind::kLongRangeAttack);
	}
	else if (is_jumping_ || is_landing_)
	{
		SetAnim(AnimKind::kJump);
	}
	else if (is_run_)
	{
		is_flip_x_ = facing_left_;
		SetAnim(AnimKind::kRun);
	}
	else if (is_move)
	{
		is_flip_x_ = facing_left_;
		SetAnim(AnimKind::kWalk);
	}
	else
	{
		SetAnim(AnimKind::kIdle);
	}


	UpdateAnimation(delta_time);

	//落下からの復帰
	if (IsOutOfBottom())
	{
		RespawnFromFall();
	}

	//今のフレームのサイズをとる
	GetCurrentFrameSize(render_current_width_, render_current_height_);

	//描画する幅、高さを計算する
	render_draw_width_ = static_cast<float>(render_current_width_) * scale_;
	render_draw_height_ = static_cast<float>(render_current_height_) * scale_;

	//基準となる中心座標を計算する
	base_center_x_ = x_player_ + render_draw_width_ * kHalf;
	base_center_y_ = y_player_ + render_draw_height_ * kHalf;

	//画像の中心座標を計算する
	base_image_center_x_ = static_cast<float>(render_current_width_) * kHalf;
	base_image_center_y_ = static_cast<float>(render_current_height_) * kHalf;

	if (is_close_attack_)
	{
		//この近距離攻撃でまだタイルにダメージを与えていないなら
		if (!is_tile_damage_in_close_attack_)
		{
			const CollisionBase::CircleF hit_circle = MakeCloseAttackHitCircle(attack_facing_left_);

			const bool hit_tile = HitHitableTilesByCircle(hit_circle, kPlayerAttackTileDamage);
			if (hit_tile)
			{
				is_tile_damage_in_close_attack_ = true;
			}
		}

	}

	//カメラの処理

	//カメラの左上座標を取得
	camera_x_ = static_cast<float>(game_level_->GetCameraLeftX());
	camera_y_ = static_cast<float>(game_level_->GetCameraTopY());

	//ゲーム画面の中心座標を計算(描画する基準となる中心座標 - カメラのX,Y座標)
	screen_center_x_ = base_center_x_ - camera_x_;
	screen_center_y_ = base_center_y_ - camera_y_;

}

/// <summary>
/// アニメーションの更新処理
/// </summary>
void Player::UpdateAnimation(float delta_time)
{
	anim_time_ += delta_time;

	const int idx = GetAnimIndex(current_anim_);
	const float interval = anim_interval_[idx];
	if (interval <= 0.0f)
	{
		return;
	}

	const int count = GetCurrentAnimFrameCount();
	if (count <= 0)
	{
		return;
	}

	while (anim_time_ >= interval)
	{
		anim_time_ -= interval;

		//やられアニメーションについて
		if (current_anim_ == AnimKind::kDead)
		{
			current_frame_ += 1;

			if (current_frame_ >= count)
			{
				current_frame_ = 0;
				is_alive_ = false;
			}

			continue;
		}

		//ジャンプについて
		if (current_anim_ == AnimKind::kJump)
		{
			const int hold = std::clamp(kJumpHoldFrame, 0, count - 1);
			const int land_start = std::clamp(kJumpLandStartFrame, 0, count - 1);

			//空中：0〜7まで進めて 7 で止める
			if (!is_landing_)
			{
				if (current_frame_ < hold)
				{
					current_frame_ += 1;
				}
				else
				{
					current_frame_ = hold;
				}
			}
			//着地中：8〜最後まで進める（最後まで行ったら着地終了）
			else
			{
				if (current_frame_ < land_start)
				{
					current_frame_ = land_start;
				}
				else if (current_frame_ < count - 1)
				{
					current_frame_ += 1;
				}
				else
				{
					current_frame_ = count - 1;
					is_landing_ = false; //着地アニメ終了
				}
			}

			continue;
		}

		//近距離攻撃について
		if (current_anim_ == AnimKind::kCloseAttack)
		{
			if (current_frame_ < count - 1)
			{
				current_frame_ += 1;
			}
			else
			{
				current_frame_ = count - 1;
				is_close_attack_ = false; //近距離攻撃アニメ終了
			}
			continue;
		}

		//遠距離攻撃について
		if (current_anim_ == AnimKind::kLongRangeAttack)
		{
			if (current_frame_ < count - 1)
			{
				current_frame_ += 1;
			}
			else
			{
				current_frame_ = count - 1;
				is_long_range_attack_ = false; //遠距離攻撃アニメ終了
			}
			continue;
		}

		//それ以外
		current_frame_ += 1;
		if (current_frame_ >= count)
		{
			current_frame_ = 0;
		}
		else if (current_frame_ < 0)
		{
			current_frame_ = count - 1;
		}
	}
}

/// <summary>
/// アニメーション状態のセット
/// </summary>
void Player::SetAnim(AnimKind anim)
{
	if (current_anim_ == anim)
	{
		return;
	}

	current_anim_ = anim;

	is_flip_x_ = facing_left_;

	current_frame_ = 0;
	anim_time_ = 0.0f;
}

/// <summary>
/// 今のアニメーションフレーム値を取得する
/// </summary>
int Player::GetCurrentAnimFrameCount() const
{
	const int idx = GetAnimIndex(current_anim_);
	int count = anim_frame_counts_[idx];


	if (count < 0)
	{
		count = 0;
	}

	if (count > kMaxAnimFrameCount)
	{
		count = kMaxAnimFrameCount;
	}

	return count;
}

/// <summary>
/// 今のフレームハンドルを取得する
/// </summary>
int Player::GetCurrentFrameHandle() const
{
	const int idx = GetAnimIndex(current_anim_);
	const int count = GetCurrentAnimFrameCount();

	if (count <= 0)
	{
		return -1;
	}

	return anim_frames_[idx][current_frame_];
}

/// <summary>
/// 今のフレームのサイズを取得する
/// </summary>
void Player::GetCurrentFrameSize(int& out_width, int& out_height) const
{
	const int idx = GetAnimIndex(current_anim_);
	out_width = anim_width_[idx];
	out_height = anim_height_[idx];
}

/// <summary>
/// 毎フレームの描画処理
/// </summary>
void Player::Render()
{
	//死んでいたら
	if (is_alive_ == false)
	{
		//何もしない
		return;
	}

	//点滅無敵状態なら
	if (!is_visible_)
	{
		//何もしない
		return;
	}

	const int handle = GetCurrentFrameHandle();
	if (handle < 0)
	{
		return;
	}

		
	DrawRotaGraph2F(
		screen_center_x_, screen_center_y_,
		base_image_center_x_, base_image_center_y_,
		scale_,
		0.0f,
		handle,
		TRUE,
		is_flip_x_
	);

	//デバッグ用当たり判定円表示
	if (DebugManager::IsDebugEnabled())
	{
		if (is_close_attack_)
		{
			const CollisionBase::CircleF hit_circle = MakeCloseAttackHitCircle(attack_facing_left_);
			const int draw_x = static_cast<int>(hit_circle.center_x - camera_x_);
			const int draw_y = static_cast<int>(hit_circle.center_y - camera_y_);
			const int r = static_cast<int>(hit_circle.radius);
			DrawCircle(draw_x, draw_y, r, GetColor(255, 0, 0), FALSE);
		}

		const CollisionBase::CircleF body_circle = GetHitCircle();
		const int draw_x = static_cast<int>(body_circle.center_x - camera_x_);
		const int draw_y = static_cast<int>(body_circle.center_y - camera_y_);
		const int r = static_cast<int>(body_circle.radius);
		DrawCircle(draw_x, draw_y, r, GetColor(0, 255, 255), FALSE);
	}
}

/// <summary>
/// プレイヤーのX座標の取得
/// </summary>
/// <returns>プレイヤーのX座標</returns>
float Player::GetPlayerPositionX() const
{
	return x_player_;
}

/// <summary>
/// プレイヤーのY座標の取得
/// </summary>
/// <returns>プレイヤーのY座標</returns>
float Player::GetPlayerPositionY() const
{
	return y_player_;
}

/// <summary>
/// プレイヤー画像の幅の取得
/// </summary>
/// <returns>プレイヤー画像の幅</returns>
int Player::GetPlayerImageWidth() const
{
	int width = 0;
	int height = 0;

	GetCurrentFrameSize(width, height);
	
	return static_cast<int>(static_cast<float>(width) * scale_);
}

/// <summary>
/// プレイヤー画像の高さの取得
/// </summary>
/// <returns>プレイヤー画像の高さ</returns>
int Player::GetPlayerImageHeight() const
{
	int width = 0;
	int height = 0;

	GetCurrentFrameSize(width, height);

	return static_cast<int>(static_cast<float>(height) * scale_);
}

/// <summary>
/// プレイヤー画像の拡大率の取得
/// </summary>
/// <returns>プレイヤー画像の拡大率</returns>
float Player::GetPlayerImageScale() const
{
	return scale_;
}

/// <summary>
/// 自機を右に動かす
/// </summary>
void Player::MoveRight(int speed)
{
	//プレイヤーをx座標正方向へ動かす
	float tmp_x = x_player_ + static_cast<float>(speed);

	//tmp_xを最低値: 0  最大値: GetEdgeRightXに補正
	tmp_x = std::clamp(tmp_x, 0.0f, game_level_->GetWorldWidth() - (static_cast<float>(GetPlayerImageWidth())) );

	//tmp_xをx_player_とする
	x_player_ = static_cast<float>(tmp_x);


}

/// <summary>
/// 自機を左に動かす
/// </summary>
void Player::MoveLeft(int speed)
{
	//プレイヤーをx座標負方向へ動かす 
	float tmp_x = x_player_ - static_cast<float>(speed);
	
	//tmp_xを補正
	tmp_x = std::clamp(tmp_x, 0.0f, game_level_->GetWorldWidth() - (static_cast<float>(GetPlayerImageWidth())));

	//tmp_xをx_player_とする
	x_player_ = static_cast<float>(tmp_x);

	
}

/// <summary>
/// プレイヤーが画面下に落ちたか
/// </summary>
bool Player::IsOutOfBottom()
{
	if (game_level_ == nullptr)
	{
		return false;
	}

	const float camera_top = static_cast<float>(game_level_->GetCameraTopY());
	const float camera_bottom = camera_top + static_cast<float>(GameInfo::GetInstance()->GetResolutionY());

	int image_width = 0;
	int image_height = 0;
	GetCurrentFrameSize(image_width, image_height);

	const float player_top = y_player_;

	return (player_top > camera_bottom + kFallOutMargin);
}

/// <summary>
/// 地面の上にいるときの安全な復帰地点を更新する
/// </summary>
void Player::UpdateRespawnPoint()
{
	if (!is_on_ground_)
	{
		return;
	}

	respawn_x_ = x_player_;
	respawn_y_ = y_player_;
}

/// <summary>
/// 落下したときに少し左へ戻して復帰させる
/// </summary>
void Player::RespawnFromFall()
{
	if (game_level_ == nullptr)
	{
		return;
	}

	int image_width = 0;
	int image_height = 0;
	GetCurrentFrameSize(image_width, image_height);

	const float player_width = static_cast<float>(image_width) * scale_;
	const float player_height = static_cast<float>(image_height) * scale_;

	const float camera_top = static_cast<float>(game_level_->GetCameraTopY());

	float respawn_x = respawn_x_ - kRespawnLeftOffset;
	if (respawn_x < 0.0f)
	{
		respawn_x = 0.0f;
	}

	const float max_x = static_cast<float>(game_level_->GetWorldWidth()) - player_width;
	respawn_x = std::clamp(respawn_x, 0.0f, max_x);

	// 画面の上端にプレイヤー下端を合わせる
	const float respawn_y = camera_top - player_height;

	SetPosition(respawn_x, respawn_y);

	// 空中復帰なので落下状態にする
	velocity_y_ = 0.0f;
	is_on_ground_ = false;
	is_jumping_ = true;
	is_landing_ = false;

	// 攻撃状態は解除
	is_close_attack_ = false;
	is_long_range_attack_ = false;
	attack_lock_seconds_ = 0.0f;

	SetAnim(AnimKind::kJump);

}

/// <summary>
/// 描画用スクリーン座標を強制更新する
/// </summary>
void Player::ForceUpdateScreenPosition(float camera_left_x, float camera_top_y)
{
	int img_width = 0;
	int img_height = 0;
	GetCurrentFrameSize(img_width, img_height);

	render_draw_width_ = static_cast<float>(img_width) * scale_;
	render_draw_height_ = static_cast<float>(img_height) * scale_;

	base_center_x_ = x_player_ + render_draw_width_ * kHalf;
	base_center_y_ = y_player_ + render_draw_height_ * kHalf;

	base_image_center_x_ = static_cast<float>(img_width) * kHalf;
	base_image_center_y_ = static_cast<float>(img_height) * kHalf;

	camera_x_ = camera_left_x;
	camera_y_ = camera_top_y;

	screen_center_x_ = base_center_x_ - camera_x_;
	screen_center_y_ = base_center_y_ - camera_y_;
}