#pragma once
#include "Task/Task.h"
#include "Audio/AudioManager.h"
#include <string>
#include <vector>

//前方宣言
class Player;
class PlayerKunai;
class EnemyKunai;
class EnemyManager;
class GameLevel;
class Camera2D;

/// <summary>
/// クナイの情報を管理するクラス
/// </summary>
class KunaiManager : public Task
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	KunaiManager();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~KunaiManager();

public:
	/// <summary>
	/// クナイの情報の列挙
	/// </summary>
	enum  class KunaiKind
	{
		/// <summary>
		/// プレイヤーのクナイ
		/// </summary>
		kPlayerKunai,

		/// <summary>
		/// 敵のクナイ
		/// </summary>
		kEnemyKunai,
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
	void OnDestroy() override;

	/// <summary>
	/// クナイの初期化
	/// </summary>
	void InitializeKunai();

	/// <summary>
	/// クナイの後処理
	/// </summary>
	void FinalizeKunai();

	/// <summary>
	/// クナイの発射
	/// </summary>
	void Fire(KunaiKind kunai_owner_kind, float pos_x, float pos_y,bool is_facing_left);

	/// <summary>
	/// プレイヤークナイのプールを取得
	/// </summary>
	/// <returns></returns>
	std::vector<PlayerKunai*>& GetPlayerKunaiPool()
	{
		return player_kunai_pool_;
	}

	/// <summary>
	/// カメラのセット
	/// </summary>
	/// <param name="camera">カメラのポインタ</param>
	void SetCamera(Camera2D* camera);

	/// <summary>
	/// EnemyManagerのポインタを引数の内容にセット
	/// </summary>
	void SetEnemyManager(EnemyManager* enemy_manager)
	{
		enemy_manager_ = enemy_manager;
	}

	/// <summary>
	/// Player_を引数の内容にセット
	/// </summary>
	void SetPlayer(Player* player)
	{
		player_ = player;
	}

	/// <summary>
	/// GameLevelのポインタをセット
	/// </summary>
	void SetGameLevel(GameLevel* game_level)
	{
		game_level_ = game_level;
	}
private:

	/// <summary>
	/// KunaiManagerのインスタンス
	/// </summary>
	static KunaiManager* instance_;

	/// <summary>
	/// プレイヤークナイの数(30発)
	/// </summary>
	static constexpr int kMaxPlayerKunaiCount = 30;

	/// <summary>
	/// 敵クナイの数(30発)
	/// </summary>
	static constexpr int kMaxEnemyKunaiCount = 30;

	/// <summary>
	/// プレイヤークナイの画像ハンドル
	/// </summary>
	int player_kunai_image_handle_ = -1;

	/// <summary>
	/// 敵クナイの画像ハンドル
	/// </summary>
	int enemy_kunai_image_handle_ = -1;

	/// <summary>
	/// プレイヤークナイの幅
	/// </summary>
	int player_kunai_width_ = 0;

	/// <summary>
	/// プレイヤークナイの高さ
	/// </summary>
	int player_kunai_height_ = 0;

	/// <summary>
	/// 敵クナイの幅
	/// </summary>
	int enemy_kunai_width_ = 0;

	/// <summary>
	/// 敵クナイの高さ
	/// </summary>
	int enemy_kunai_height_ = 0;

	/// <summary>
	/// プレイヤークナイの情報を入れるプール
	/// </summary>
	std::vector<PlayerKunai*> player_kunai_pool_;

	/// <summary>
	/// 敵クナイの情報を入れるプール
	/// </summary>
	std::vector<EnemyKunai*> enemy_kunai_pool_;

	/// <summary>
	/// EnemyManagerのポインタ
	/// </summary>
	EnemyManager* enemy_manager_ = nullptr;

	/// <summary>
	/// Playerのポインタ
	/// </summary>
	Player* player_ = nullptr;

	/// <summary>
	/// GameLevelのポインタ
	/// </summary>
	GameLevel* game_level_ = nullptr;

	/// <summary>
	/// Camera2Dのポインタ
	/// </summary>
	Camera2D* camera_ = nullptr;

};