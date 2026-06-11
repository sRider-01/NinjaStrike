#define NOMINMAX //std::maxを使うためにWindowsヘッダのマクロ定義をしないようにさせる
#include "KunaiManager.h"
#include "PlayerKunai.h"
#include "EnemyKunai.h"
#include "Enemy/EnemyManager.h"
#include "Camera/Camera2D.h"
#include "Player/Player.h"
#include "DxLib.h"
/// <summary>
/// クナイマネージャーのインスタンスを初期化
/// </summary>
KunaiManager* KunaiManager::instance_ = nullptr;

/// <summary>
/// KunaiManager.cppで使う定数
/// </summary>
namespace
{
	/// <summary>
	/// 敵のクナイによるダメージ
	/// </summary>
	const int kEnemyKunaiDamage = 1;

	/// <summary>
	/// 画像パス
	/// </summary>
	const char* kKunaiPath = "Assets/Image/Kunai/Kunai_128x128.png";

	/// <summary>
	/// 未ロードを表す値
	/// </summary>
	const int  kUnLoaded = -1;

	/// <summary>
	/// タイルへのダメージ
	/// </summary>
	const int kTileDamage = 1;
}

/// <summary>
/// コンストラクタ
/// </summary>
KunaiManager::KunaiManager()
{
}

/// <summary>
/// デストラクタ
/// </summary>
KunaiManager::~KunaiManager()
{
}

/// <summary>
/// 破棄前処理
/// </summary>
void KunaiManager::OnDestroy()
{
	FinalizeKunai();
}

/// <summary>
/// クナイの初期化
/// </summary>
void KunaiManager::InitializeKunai()
{
	//プレイヤークナイについて
	
	if (player_kunai_image_handle_ == kUnLoaded)
	{
		player_kunai_image_handle_ = LoadGraph(kKunaiPath);

		//ロード成功したら
		if (player_kunai_image_handle_ >= 0)
		{
			//画像のサイズを変数にセット
			GetGraphSize(player_kunai_image_handle_, &player_kunai_width_, &player_kunai_height_);
		}
	}

	//領域確保
	player_kunai_pool_.reserve(kMaxPlayerKunaiCount);

	//kMaxPlayerKunais分繰り返す
	for (int i = 0; i < kMaxPlayerKunaiCount; i++)
	{
		//クナイ１発分new
		PlayerKunai* kunai = new PlayerKunai();

		//クナイの画像サイズを各kunaiにセットする
		kunai->SetImageResource(player_kunai_image_handle_, player_kunai_width_, player_kunai_height_);

		//リストに入れる
		player_kunai_pool_.push_back(kunai);

	}


	//敵クナイについて
	if (enemy_kunai_image_handle_ == kUnLoaded)
	{
		enemy_kunai_image_handle_ = LoadGraph(kKunaiPath);

		//ロード成功したら
		if (enemy_kunai_image_handle_ >= 0)
		{
			//画像のサイズを変数にセット
			GetGraphSize(enemy_kunai_image_handle_, &enemy_kunai_width_, &enemy_kunai_height_);
		}
	}

	//領域確保
	enemy_kunai_pool_.reserve(kMaxEnemyKunaiCount);

	//kMaxEnemyKunais分繰り返す
	for (int i = 0; i < kMaxEnemyKunaiCount; i++)
	{
		//クナイ１発分new
		EnemyKunai* kunai = new EnemyKunai();

		//クナイの画像サイズを各kunaiにセットする
		kunai->SetImageResource(enemy_kunai_image_handle_, enemy_kunai_width_, enemy_kunai_height_);

		//リストに入れる
		enemy_kunai_pool_.push_back(kunai);

	}
}

/// <summary>
/// クナイの後処理
/// </summary>
void KunaiManager::FinalizeKunai()
{

	//リストの大きさ分だけの数値
	const std::size_t count = player_kunai_pool_.size();

	//生成したクナイ一発ずつについて
	for (std::size_t i = 0; i < count; ++i)
	{
		//クナイ一発分の情報をとる
		PlayerKunai* kunai = player_kunai_pool_[i];

		//nullptrでないなら
		if (kunai != nullptr)
		{
			//kunaiの情報をdelete
			delete kunai;

			//deleteした後のポインタをnullptrにする
			player_kunai_pool_[i] = nullptr;
		}
	}
	//リスト自体も空にする
	player_kunai_pool_.clear();

	//ハンドルの後処理
	if (player_kunai_image_handle_ >= 0)
	{
		DeleteGraph(player_kunai_image_handle_);

		player_kunai_image_handle_ = kUnLoaded;
	}

	//共通のクナイサイズも後処理
	player_kunai_width_ = 0;
	player_kunai_height_ = 0;

	//敵クナイについて

	//リストの大きさ分だけの数値
	const std::size_t enemy_count = enemy_kunai_pool_.size();

	//生成したクナイ一発ずつについて
	for (std::size_t i = 0; i < enemy_count; ++i)
	{
		//クナイ一発分の情報をとる
		EnemyKunai* kunai = enemy_kunai_pool_[i];

		//nullptrでないなら
		if (kunai != nullptr)
		{
			//kunaiの情報をdelete
			delete kunai;

			//deleteした後のポインタをnullptrにする
			enemy_kunai_pool_[i] = nullptr;
		}
	}
	//リスト自体も空にする
	enemy_kunai_pool_.clear();

	//ハンドルの後処理
	if (enemy_kunai_image_handle_ >= 0)
	{
		DeleteGraph(enemy_kunai_image_handle_);

		enemy_kunai_image_handle_ = kUnLoaded;
	}

	//共通のクナイサイズも後処理
	enemy_kunai_width_ = 0;
	enemy_kunai_height_ = 0;
}

/// <summary>
/// 毎フレームの更新処理
/// </summary>
/// <param name="delta_time">前回実行フレームからの経過時間（秒）</param>
void KunaiManager::Update(float delta_time)
{
	if (game_level_ != nullptr && game_level_->IsStageChanging())
	{
		return;
	}

	//リストの大きさ分だけの数値
	const std::size_t player_kunai_count = player_kunai_pool_.size();

	//生成したクナイ一発ずつについて
	for (std::size_t i = 0; i < player_kunai_count; ++i)
	{
		PlayerKunai* kunai = player_kunai_pool_[i];
		if (kunai == nullptr)
		{
			continue;
		}

		if (!kunai->GetInfo().is_fired)
		{
			continue;
		}

		kunai->Update(delta_time);

		if (player_ != nullptr)
		{
			const CollisionBase::CircleF kunai_circle = kunai->MakeHitCircle();

			const bool hit_tile = player_->HitHitableTilesByCircle(kunai_circle, kTileDamage);
			if (hit_tile)
			{
				kunai->Deactivate();
			}
		}
	}

	//敵クナイについて

	//リストの大きさ分だけの数値
	const std::size_t enemy_kunai_count = enemy_kunai_pool_.size();

	//生成したクナイ一発ずつについて
	for (std::size_t i = 0; i < enemy_kunai_count; ++i)
	{
		EnemyKunai* kunai = enemy_kunai_pool_[i];
		if (kunai == nullptr)
		{
			continue;
		}

		if (!kunai->GetInfo().is_fired)
		{
			continue;
		}

		kunai->Update(delta_time);

		if (player_ != nullptr)
		{
			const CollisionBase::CircleF player_circle = player_->GetHitCircle();
			const CollisionBase::CircleF kunai_circle = kunai->MakeHitCircle();

			if (CollisionBase::IsCircleOverlap(player_circle, kunai_circle))
			{
				player_->TakeDamage(kEnemyKunaiDamage);
				continue;
			}

			const bool hit_tile = player_->HitHitableTilesByCircle(kunai_circle, kTileDamage);
			if (hit_tile)
			{
				kunai->Deactivate();
			}
		}
	}
}

/// <summary>
/// 毎フレームの描画処理
/// </summary>
void KunaiManager::Render()
{
	//リストの大きさ分だけの数値
	const std::size_t player_kunai_count = player_kunai_pool_.size();

	const std::size_t enemy_kunai_count = enemy_kunai_pool_.size();

	//生成したクナイ一発ずつについて
	for (std::size_t i = 0; i < player_kunai_count; ++i)
	{
		//クナイ一発分の情報をとる
		KunaiBase* player_kunai = player_kunai_pool_[i];

		//発射されていないならば
		if (!player_kunai->GetInfo().is_fired)
		{
			//何もせず次のクナイの情報へ
			continue;
		}

		//クナイの情報についてRenderを実施する
		player_kunai->Render();
	}

	//敵クナイについて
		
	//生成したクナイ一発ずつについて
	for (std::size_t i = 0; i < enemy_kunai_count; ++i)
	{
		//クナイ一発分の情報をとる
		KunaiBase* enemy_kunai = enemy_kunai_pool_[i];

		//発射されていないならば
		if (!enemy_kunai->GetInfo().is_fired)
		{
			//何もせず次のクナイの情報へ
			continue;
		}

		//クナイの情報についてRenderを実施する
		enemy_kunai->Render();
	}

}

/// <summary>
/// クナイの発射
/// </summary>
void KunaiManager::Fire(KunaiKind kunai_kind, float pos_x, float pos_y, bool is_facing_left)
{
	//player_kunai_listの大きさ分だけの数値
	const std::size_t player_kunai_count = player_kunai_pool_.size();

	//enemy_kunai_listの大きさ分だけの数値
	const std::size_t enemy_kunai_count = enemy_kunai_pool_.size();

	switch (kunai_kind)
	{
	case KunaiManager::KunaiKind::kPlayerKunai:

		//生成したクナイ一発ずつについて
		for (std::size_t i = 0; i < player_kunai_count; ++i)
		{
			//クナイ一発分の情報をとる
			KunaiBase* kunai = player_kunai_pool_[i];

			//発射済みなら
			if (kunai->GetInfo().is_fired)
			{
				//何もせず次のクナイの情報へ
				continue;
			}

			//位置を渡して発射処理を呼び出す
			kunai->Fire(pos_x, pos_y, is_facing_left);


			//発射したので抜ける
			break;
		}

		break;
	case KunaiManager::KunaiKind::kEnemyKunai:

		//生成したクナイ一発ずつについて
		for (std::size_t i = 0; i < enemy_kunai_count; ++i)
		{
			//クナイ一発分の情報をとる
			KunaiBase* kunai = enemy_kunai_pool_[i];

			//発射済みなら
			if (kunai->GetInfo().is_fired)
			{
				//何もせず次のクナイの情報へ
				continue;
			}

			//位置を渡して発射処理を呼び出す
			kunai->Fire(pos_x, pos_y, is_facing_left);


			//発射したので抜ける
			break;
		}

		break;
	default:
		//何もしない
		break;
	}
}


/// <summary>
/// カメラのセット
/// </summary>
void KunaiManager::SetCamera(Camera2D* camera)
{
	camera_ = camera;

	 for (std::size_t i = 0; i < player_kunai_pool_.size(); ++i)
	 {
	     if (player_kunai_pool_[i] != nullptr)
	     {
			 player_kunai_pool_[i]->SetCamera(camera_);
	     }
	 }

	 //敵クナイについて
	 for (std::size_t i = 0; i < enemy_kunai_pool_.size(); ++i)
	 {
		 if (enemy_kunai_pool_[i] != nullptr)
		 {
			 enemy_kunai_pool_[i]->SetCamera(camera_);
		 }
	 }
}