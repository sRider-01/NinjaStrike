#pragma once
#include "Level/GameLevel.h"
#include "GameInfo.h"
#include "Kunai/KunaiManager.h"
#include "Collision/CollisionBase.h"
#include "Task/Task.h"
#include "Input/InputManager.h"
#include <array>
#include <cmath>

//前方宣言
class GameLevel;
class KunaiManager;
class EffectManager;

/// <summary>
/// プレイヤー（自機）についてのクラス
/// </summary>
class Player :public Task
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Player();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

public:
	/// <summary>
	/// プレイヤーの状態
	/// </summary>
	enum class PlayerState
	{
		/// <summary>
		/// 処理なし
		/// </summary>
		kNone,

		/// <summary>
		/// 通常処理
		/// </summary>
		kPlay,
	};

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
		/// 歩行状態
		/// </summary>
		kWalk,

		/// <summary>
		/// ダッシュ状態
		/// </summary>
		kRun,

		/// <summary>
		/// ジャンプ状態
		/// </summary>
		kJump,

		/// <summary>
		/// 近距離攻撃状態
		/// </summary>
		kCloseAttack,

		/// <summary>
		/// 遠距離攻撃状態
		/// </summary>
		kLongRangeAttack,

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
	/// 毎フレームの更新処理
	/// </summary>
	/// <param name="delta_time">前回実行フレームからの経過時間（秒）</param>
	void Update(float delta_time) override;

	/// <summary>
	/// 毎フレームの描画処理
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
	void Initialize();

	/// <summary>
	/// 後処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// 被ダメージ処理
	/// </summary>
	/// <param name="damage">ダメージ量</param>
	void TakeDamage(int damage);

	/// <summary>
	/// やられ処理
	/// </summary>
	void OnDead();

	/// <summary>
	/// 点滅無敵の更新処理
	/// </summary>
	/// <param name="delta_time">前回実行フレームからの経過時間（秒）</param>
	void UpdateInvisible(float delta_time);

	/// <summary>
	/// ジャンプ開始処理
	/// </summary>
	void StartJump();

	/// <summary>
	/// 近距離攻撃開始処理
	/// </summary>
	void StartCloseAttack();

	/// <summary>
	/// 遠距離攻撃開始処理
	/// </summary>
	void StartLongRangeAttack();

	/// <summary>
	/// 当たり判定四角形を作る
	/// </summary>
	CollisionBase::RectF MakeBodyRect();

	/// <summary>
	/// 壁との当たり判定処理
	/// </summary>
	/// <param name="move_x">動く先のX座標</param>
	void WallCollision(float move_x);

	/// <summary>
	/// 地面との当たり判定処理
	/// </summary>
	/// <param name="move_y">動く先のY座標</param>
	/// <returns>地面と接しているか</returns>
	bool GroundCollision(float move_y);

	/// <summary>
	/// 今地面に接しているか？(probe_yだけ下にずらして判定をとる)
	/// </summary>
	/// <param name="probe_y">判定を下にずらす量</param>
	/// <returns>今地面に接しているか</returns>
	bool IsOnGroundByProbe(float probe_y);

	/// <summary>
	/// 当たり判定円を作成する
	/// </summary>
	CollisionBase::CircleF MakeCloseAttackHitCircle(bool is_facing_left) const;

	/// <summary>
	/// 自機の当たり判定円を作成する
	/// </summary>
	/// <returns>自機の当たり判定円</returns>
	CollisionBase::CircleF GetHitCircle();

	/// <summary>
	/// 攻撃の当たり判定円を取得する（外部クラス用）
	/// </summary>
	/// <param name="out_circle">攻撃の当たり判定円</param>
	bool TryGetCloseAttackHitCircle(CollisionBase::CircleF& out_circle);
	
	/// <summary>
	/// 攻撃を受け取る属性のタイルにヒットしたかを見る
	/// </summary>
	/// <param name="damage">与えるダメージ</param>
	/// <returns>タイルに当たった（破壊可能/破壊不可どちらでも）ならtrue</returns>
	bool HitHitableTilesByCircle(const CollisionBase::CircleF& hit_circle, int damage);

	/// <summary>
	/// 座標をセットする
	/// </summary>
	/// <param name="x">X座標</param>
	/// <param name="y">Y座標</param>
	void SetPosition(float x, float y);

	/// <summary>
	/// アニメーションの更新処理
	/// </summary>
	/// <param name="delta_time">前回実行フレームからの経過時間（秒）</param>
	void UpdateAnimation(float delta_time);

	/// <summary>
	/// アニメーション状態のセット
	/// </summary>
	/// <param name="anim">セットするアニメーション</param>
	void SetAnim(AnimKind anim);

	/// <summary>
	/// 今のアニメーションフレーム値を取得する
	/// </summary>
	/// <returns>アニメーションフレーム値</returns>
	int GetCurrentAnimFrameCount() const;

	/// <summary>
	/// 今のフレームハンドルを取得する
	/// </summary>
	/// <returns>フレームハンドル</returns>
	int GetCurrentFrameHandle() const;

	/// <summary>
	/// 今のフレームのサイズを取得する
	/// </summary>
	/// <param name="out_w">横幅データの受取先</param>
	/// <param name="out_h">高さデータの受取先</param>
	void GetCurrentFrameSize(int& out_w, int& out_h) const;

	/// <summary>
	/// プレイヤーのX座標の取得
	/// </summary>
	/// <returns>プレイヤーのX座標</returns>
	float GetPlayerPositionX() const;

	/// <summary>
	/// プレイヤーのY座標の取得
	/// </summary>
	/// <returns>プレイヤーのY座標</returns>
	float GetPlayerPositionY() const;

	/// <summary>
	/// プレイヤー画像の幅の取得
	/// </summary>
	/// <returns>プレイヤー画像の幅</returns>
	int GetPlayerImageWidth() const;

	/// <summary>
	/// プレイヤー画像の高さの取得
	/// </summary>
	/// <returns>プレイヤー画像の高さ</returns>
	int GetPlayerImageHeight() const;

	/// <summary>
	/// プレイヤー画像の拡大率の取得
	/// </summary>
	/// <returns>プレイヤー画像の拡大率</returns>
	float GetPlayerImageScale() const;

	/// <summary>
	/// 自機を右に動かす
	/// </summary>
	void MoveRight(int speed);

	/// <summary>
	/// 自機を左に動かす
	/// </summary>
	void MoveLeft(int speed);

	/// <summary>
	/// 座標をタイル座標に変換する
	/// </summary>
	/// <param name="world">マップの座標</param>
	/// <param name="tile_size">タイルのサイズ</param>
	/// <returns>変換後のタイル座標</returns>
	int WorldToTile(float world, int tile_size)
	{
		return static_cast<int>(std::floor(world / static_cast<float>(tile_size)));
	}

	/// <summary>
	/// GameLevelのポインタをPlayerクラス初期化時にセットするための関数
	/// </summary>
	/// <param name="battlelevel">GameLevelのポインタ</param>
	void SetGameLevel(GameLevel* battlelevel)
	{
		//game_level_に引数のポインタをセット
		game_level_ = battlelevel;
	}

	/// <summary>
	/// エフェクトマネージャーのポインタをセット
	/// </summary>
	/// <param name="mgr">ポインタ</param>
	void SetEffectManager(EffectManager* mgr)
	{
		effect_manager_ = mgr;
	}

	/// <summary>
	/// クナイマネージャーをセット
	/// </summary>
	/// <param name="kunai_manager">ポインタ</param>
	void SetKunaiManager(KunaiManager* kunai_manager)
	{
		kunai_manager_ = kunai_manager;
	}

	/// <summary>
	/// アニメーションのIDを受け取る
	/// </summary>
	/// <param name="kind">アニメーションの種類</param>
	/// <returns>アニメーションのID</returns>
	int GetAnimIndex(AnimKind kind) const
	{
		return static_cast<int>(kind);
	}

	/// <summary>
	/// 最大HPを取得する
	/// </summary>
	/// <returns>最大HP</returns>
	int GetMaxHp() const
	{
		return max_hp_;
	}

	/// <summary>
	/// 現在のHPを取得する
	/// </summary>
	/// <returns>現在のHP</returns>
	int GetCurrentHp() const
	{
		return current_hp_;
	}

	/// <summary>
	/// 生きているかを返す
	/// </summary>
	/// <returns>生きているか</returns>
	bool IsAlive() const
	{
		return is_alive_;
	}

	/// <summary>
	/// 地面の上にいるかを返す
	/// </summary>
	/// <returns>地面の上にいるか</returns>
	bool IsOnGround() const
	{
		return is_on_ground_;
	}

	/// <summary>
	/// 画面下外か？
	/// </summary>
	/// <returns>Yes/No</returns>
	bool IsOutOfBottom();

	/// <summary>
	/// リスポーン地点の更新
	/// </summary>
	void UpdateRespawnPoint();

	/// <summary>
	/// 落下からの復帰
	/// </summary>
	void RespawnFromFall();

	/// <summary>
	/// 描画用スクリーン座標を強制更新する
	/// ステージ切り替え時にUpdate()をスキップしても
	/// 正しい位置に描画できるようにする
	/// </summary>
	/// <param name="camera_left_x">カメラの左端X座標</param>
	/// <param name="camera_top_y">カメラの上端Y座標</param>
	void ForceUpdateScreenPosition(float camera_left_x, float camera_top_y);

	/// <summary>
	/// 遠距離攻撃SEハンドルを返す
	/// AudioManagerのプレビューSE登録用
	/// </summary>
	/// <returns>遠距離攻撃SEハンドル</returns>
	int GetLongRangeShotSeHandle() const
	{
		return long_range_shot_se_handle_;
	}

	/// <summary>
	/// 落下状態に強制的にセットする
	/// ステージ切り替え後にスポーンした時点で呼ぶ
	/// </summary>
	void SetFallingState()
	{
		is_on_ground_ = false;
		facing_left_ = false;
		is_jumping_ = true;
		velocity_y_ = 1.0f;
		is_landing_ = false;
	}

	/// <summary>
	/// アニメーションの配列内の情報内訳
	/// </summary>
	struct PlayerAnimDef
	{
		/// <summary>
		/// 種別
		/// </summary>
		Player::AnimKind kind;

		/// <summary>
		/// パスの配列のポインタ(読み取り専用としてchar*をみて、その先のpathsも読み取り専用で見る)
		/// </summary>
		const char* const* paths;

		/// <summary>
		/// フレーム数
		/// </summary>
		int count;

		/// <summary>
		/// 1フレームの秒数
		/// </summary>
		float interval;
	};

	/// <summary>
	/// アニメーションの種類数
	/// </summary>
	static const int kAnimCount = static_cast<int>(AnimKind::kNum);

	/// <summary>
	/// アニメーションフレームの最大値、決め打ちで入れている
	/// </summary>
	static const int kMaxAnimFrameCount = 70;

private:
	/// <summary>
	/// 生きているか？（描画するか？）
	/// </summary>
	bool is_alive_ = true;

	/// <summary>
	/// 最大HP
	/// </summary>
	int max_hp_ = 0;

	/// <summary>
	/// 今のHP
	/// </summary>
	int current_hp_ = 0;

	/// <summary>
	/// 点滅状態の効果時間
	/// </summary>
	float blink_duration_ = 0.0f;

	/// <summary>
	/// 点滅状態切り替え用経過時間
	/// </summary>
	float blink_interval_timer_ = 0.0f;

	/// <summary>
	/// 見えるか？＝描画するか？
	/// </summary>
	bool is_visible_ = true;

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

	/// <summary>
	/// 今のフレームの幅
	/// </summary>
	int render_current_width_ = 0;

	/// <summary>
	/// 今のフレームの高さ
	/// </summary>
	int render_current_height_ = 0;

	/// <summary>
	/// 実際に描画する幅
	/// </summary>
	float render_draw_width_ = 0.0f;
	
	/// <summary>
	/// 実際に描画する高さ
	/// </summary>
	float render_draw_height_ = 0.0f;

	/// <summary>
	/// 描画の基準になる中心X座標
	/// </summary>
	float base_center_x_ = 0.0f;

	/// <summary>
	/// 描画の基準になる中心Y座標
	/// </summary>
	float base_center_y_ = 0.0f;

	/// <summary>
	/// 画像の中心X座標
	/// </summary>
	float base_image_center_x_ = 0.0f;

	/// <summary>
	/// 画像の中心Y座標
	/// </summary>
	float base_image_center_y_ = 0.0f;

	//カメラ関連 //
	
	/// <summary>
	/// カメラX座標
	/// </summary>
	float camera_x_ = 0.0f;

	/// <summary>
	/// カメラY座標
	/// </summary>
	float camera_y_ = 0.0f;

	/// <summary>
	/// ゲーム画面の中心X座標
	/// </summary>
	float screen_center_x_ = 0.0f;

	/// <summary>
	/// ゲーム画面の中心Y座標
	/// </summary>
	float screen_center_y_ = 0.0f;

	//アニメーション関連 //

	/// <summary>
	/// プレイヤーは今左を向いているか？
	/// </summary>
	bool facing_left_ = false;

	/// <summary>
	/// 攻撃開始時の向き
	/// </summary>
	bool attack_facing_left_ = false;

	/// <summary>
	/// 左右反転して描画するか？
	/// </summary>
	bool is_flip_x_ = false;

	/// <summary>
	/// 今のアニメーションの状態（アイドル状態）
	/// </summary>
	AnimKind current_anim_ = AnimKind::kIdle;

	/// <summary>
	/// 今のフレーム
	/// </summary>
	int current_frame_ = 0;

	/// <summary>
	/// 経過時間
	/// </summary>
	float anim_time_ = 0.0f;

	/// <summary>
	/// 拡大率
	/// </summary>
	float scale_ = 1.0f;

	/// <summary>
	/// プレイヤーX座標
	/// </summary>
	float x_player_ = 0.0f;

	/// <summary>
	/// プレイヤーY座標
	/// </summary>
	float y_player_ = 0.0f;

	/// <summary>
	/// 左に向いているか？
	/// </summary>
	bool is_input_left_ = false;
	
	/// <summary>
	/// 右に向いているか？
	/// </summary>
	bool is_input_right_ = false;

	/// <summary>
	/// ダッシュ中か？
	/// </summary>
	bool is_run_ = false;

	//ジャンプ関連

	/// <summary>
	/// ジャンプしているか？
	/// </summary>
	bool is_jumping_ = false;

	/// <summary>
	/// ジャンプ中の速度
	/// </summary>
	int air_move_speed_ = 0;

	/// <summary>
	/// 着地アニメ中か？
	/// </summary>
	bool is_landing_ = false;

	/// <summary>
	/// 地面上にいるか？
	/// </summary>
	bool is_on_ground_ = true;

	/// <summary>
	/// 上方向へ働くジャンプ力
	/// </summary>
	float velocity_y_ = 0.0f;

	/// <summary>
	/// ジャンプ入力バッファの残り時間（秒）
	/// 0より大きければジャンプ入力が有効
	/// </summary>
	float jump_buffer_seconds_ = 0.0f;

	/// <summary>
	/// 着地してからのフレーム数カウント
	/// </summary>
	int landing_frame_count_ = 0;

	//落下関連
	
	/// <summary>
	/// リスポーン地点X
	/// </summary>
	float respawn_x_ = 0.0f;

	/// <summary>
	/// リスポーン地点Y
	/// </summary>
	float respawn_y_ = 0.0f;

	//攻撃関連

	/// <summary>
	/// 近距離攻撃しているか？
	/// </summary>
	bool is_close_attack_ = false;

	/// <summary>
	/// 遠距離攻撃しているか？
	/// </summary>
	bool is_long_range_attack_ = false;

	/// <summary>
	/// 攻撃のクールタイム
	/// </summary>
	float attack_cooldown_seconds_ = 0.0f;

	/// <summary>
	/// 攻撃による行動ロックの残り秒数
	/// </summary>
	float attack_lock_seconds_ = 0.0f;

	/// <summary>
	/// この近距離攻撃でタイルにダメージを与えたか？
	/// </summary>
	bool is_tile_damage_in_close_attack_ = false;


	/// <summary>
	/// ゲームレベルの状態
	/// </summary>
	PlayerState player_state_ = PlayerState::kNone;


	/// <summary>
	/// ゲームレベルのポインタ
	/// </summary>
	GameLevel* game_level_ = nullptr;

	/// <summary>
	/// エフェクトマネージャーのポインタ
	/// </summary>
	EffectManager* effect_manager_ = nullptr;

	/// <summary>
	/// クナイマネージャーのポインタ
	/// </summary>
	KunaiManager* kunai_manager_ = nullptr;

	/// <summary>
	/// 近距離攻撃SEのハンドル
	/// </summary>
	int close_attack_se_handle_ = -1;

	/// <summary>
	/// 攻撃SEハンドル
	/// </summary>
	int long_range_shot_se_handle_ = -1;

	/// <summary>
	/// 被ダメージSEハンドル
	///</summary>
	int damage_se_handle_ = -1;

	/// <summary>
	/// ジャンプSEのハンドル
	/// </summary>
	int jump_se_handle_ = -1;

	/// <summary>
	/// 破壊可能タイルヒットSEのハンドル
	/// </summary>
	int hitable_break_se_handle_ = -1;

	/// <summary>
	/// 破壊可能オブジェクト完全破壊SEのハンドル
	/// </summary>
	int hitable_complete_break_se_handle_ = -1;
};