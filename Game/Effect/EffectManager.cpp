#include "EffectManager.h"
#include "DxLib.h"

/// <summary>
/// EffectManager.cppで使う定数
/// </summary>
namespace
{
	/// <summary>
	/// 斬撃エフェクトの画像をX方向に何等分するか
	/// </summary>
	const int kSlashImageDivX = 5;

	/// <summary>
	/// 斬撃エフェクトの画像をY方向に何等分するか
	/// </summary>
	const int kSlashImageDivY = 1;

	/// <summary>
	/// 斬撃エフェクトの一コマに使用する秒数
	/// </summary>
	const float kEffectSlashInterval = 0.03f;

	/// <summary>
	/// 斬撃エフェクトのパス
	/// </summary>
	const char* kSlashEffectPath = "Assets/Image/Effect/SlashEffect.png";


	/// <summary>
	/// 爆発エフェクトの画像をX方向に何等分するか
	/// </summary>
	const int kExplosionImageDivX = 5;

	/// <summary>
	/// 爆発エフェクトの画像をY方向に何等分するか
	/// </summary>
	const int kExplosionImageDivY = 1;

	/// <summary>
	/// 爆発エフェクトの一コマに使用する秒数
	/// </summary>
	const float kEffectExplosionInterval = 0.07f;

	/// <summary>
	/// 爆発エフェクトのパス
	/// </summary>
	const char* kExplosionEffectPath = "Assets/Image/Effect/ExplosionEffect.png";
}

/// <summary>
/// 毎フレームの更新処理
/// </summary>	
/// <param name="delta_time">前回実行フレームからの経過時間（秒）</param>
void EffectManager::Update(float delta_time)
{
	for (int i = 0; i < kMaxSlashCount; ++i)
	{
		if (slash_pool_[i].GetInfo().is_active_)
		{
			slash_pool_[i].Update(delta_time);
		}
	}

	for (int i = 0; i < kMaxExplosionCount; ++i)
	{
		if (explosion_pool_[i].GetInfo().is_active_)
		{
			explosion_pool_[i].Update(delta_time);
		}
	}
}

/// <summary>
/// 毎フレームの描画処理
/// </summary>
void EffectManager::Render()
{
	for (int i = 0; i < kMaxSlashCount; ++i)
	{
		if (slash_pool_[i].GetInfo().is_active_)
		{
			slash_pool_[i].Render();
		}
	}

	for (int i = 0; i < kMaxExplosionCount; ++i)
	{
		if (explosion_pool_[i].GetInfo().is_active_)
		{
			explosion_pool_[i].Render();
		}
	}
}

/// <summary>
/// 破棄前処理
/// </summary>
void EffectManager::OnDestroy()
{
	FinalizeEffect();
}

/// <summary>
/// 初期化
/// </summary>
void EffectManager::InitializeEffect()
{
	//エフェクトの一コマに使用する秒数をセット
	slash_interval_ = kEffectSlashInterval;
	explosion_interval_ = kEffectExplosionInterval;

	//画像サイズ取得（分割1枚のサイズ計算のため）
	int slash_image = LoadGraph(kSlashEffectPath);

	if (slash_image < 0)
	{
		return;
	}

	int explosion_image = LoadGraph(kExplosionEffectPath);

	if (explosion_image < 0)
	{
		//SlashImageをデリート
		DeleteGraph(slash_image);
		
		return;
	}

	int slash_width = 0;
	int slash_height = 0;
	GetGraphSize(slash_image, &slash_width, &slash_height);
	DeleteGraph(slash_image);

	if (slash_width <= 0 || slash_height <= 0)
	{
		//ExplosionImageをデリート
		DeleteGraph(explosion_image);
		
		return;
	}

	int explosion_width = 0;
	int explosion_height = 0;
	GetGraphSize(explosion_image, &explosion_width, &explosion_height);
	DeleteGraph(explosion_image);
	if (explosion_width <= 0 || explosion_height <= 0)
	{
		return;
	}

	//1枚分の幅、高さを計算
	const int slash_piece_width = slash_width / kSlashImageDivX;
	const int slash_piece_height = slash_height / kSlashImageDivY;

	//分割してロード
	int slash_handles[kSlashImageFrameCount] = { -1, -1, -1, -1, -1 };
	
	LoadDivGraph(kSlashEffectPath, kSlashImageFrameCount, kSlashImageDivX, kSlashImageDivY, slash_piece_width, slash_piece_height, slash_handles);

	for (int i = 0; i < kSlashImageFrameCount; ++i)
	{
		slash_frames_[i] = slash_handles[i];
	}

	for (int i = 0; i < kMaxSlashCount; ++i)
	{
		slash_pool_[i].SetFrames(slash_frames_.data(), kSlashImageFrameCount, slash_interval_);
	}

	//1枚分の幅、高さを計算
	const int explosion_piece_width = explosion_width / kExplosionImageDivX;
	const int explosion_piece_height = explosion_height / kExplosionImageDivY;

	//分割してロード
	int explosion_handles[kExplosionImageFrameCount] = { -1, -1, -1, -1, -1 };

	LoadDivGraph(kExplosionEffectPath, kExplosionImageFrameCount, kExplosionImageDivX, kExplosionImageDivY, explosion_piece_width, explosion_piece_height, explosion_handles);

	for (int i = 0; i < kExplosionImageFrameCount; ++i)
	{
		explosion_frames_[i] = explosion_handles[i];
	}

	for (int i = 0; i < kMaxExplosionCount; ++i)
	{
		explosion_pool_[i].SetFrames(explosion_frames_.data(), kExplosionImageFrameCount, explosion_interval_);
		explosion_pool_[i].Initialize();
	}
}

/// <summary>
/// 後処理
/// </summary>
void EffectManager::FinalizeEffect()
{
	for (int i = 0; i < kSlashImageFrameCount; ++i)
	{
		if (slash_frames_[i] >= 0)
		{
			DeleteGraph(slash_frames_[i]);
			slash_frames_[i] = -1;
		}
	}

	for (int i = 0; i < kExplosionImageFrameCount; ++i)
	{
		if (explosion_frames_[i] >= 0)
		{
			DeleteGraph(explosion_frames_[i]);
			explosion_frames_[i] = -1;
		}
	}
}

/// <summary>
/// エフェクトの発生
/// </summary>
void EffectManager::EffectAct(EffectOwner owner, EffectId id, float pos_x, float pos_y, bool turn_flag)
{
	if (id == EffectId::kSlash)
	{
		EffectBase* slash_effect = FindFreeEffect(EffectId::kSlash);
		if (slash_effect == nullptr)
		{
			return;
		}


		slash_effect->EffectAct(pos_x, pos_y,turn_flag);
	}
	else if (id == EffectId::kExplosion)
	{
		EffectBase* explosion_effect = FindFreeEffect(EffectId::kExplosion);
		if (explosion_effect == nullptr)
		{
			return;
		}


		explosion_effect->EffectAct(pos_x, pos_y, turn_flag);
	}
}

/// <summary>
/// 描画されていないエフェクトの検索
/// </summary>
EffectBase* EffectManager::FindFreeEffect(EffectId id)
{
	switch (id)
	{
	case EffectId::kSlash:
		for (int i = 0; i < kMaxSlashCount; ++i)
		{
			if (!slash_pool_[i].GetInfo().is_active_)
			{
				return &slash_pool_[i];
			}
		}
		break;

	case EffectId::kExplosion:
		for (int i = 0; i < kMaxExplosionCount; ++i)
		{
			if (!explosion_pool_[i].GetInfo().is_active_)
			{
				return &explosion_pool_[i];
			}
		}
		break;
	}

	return nullptr;
}

/// <summary>
/// カメラのセット
/// </summary>
/// <param name="camera">カメラのポインタ</param>
void EffectManager::SetCamera(Camera2D* camera)
{
	camera_ = camera;

	
	 for (std::size_t i = 0; i < slash_pool_.size(); ++i)
	 {
		 slash_pool_[i].SetCamera(camera_);
	 }

	 for (std::size_t i = 0; i < explosion_pool_.size(); ++i)
	 {
		 explosion_pool_[i].SetCamera(camera_);
	 }
}