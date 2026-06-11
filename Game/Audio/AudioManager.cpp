#include "AudioManager.h"
#include "DxLib.h"

/// <summary>
/// インスタンス初期化
/// </summary>
AudioManager* AudioManager::audio_manager_instance_ = nullptr;

/// <summary>
/// AudioManager.cppで使う定数
/// </summary>
namespace
{
	/// <summary>
	/// ゲーム内音量の最大値（0〜100）
	/// </summary>
	const int kGameVolumeMax = 100;

	/// <summary>
	/// DxLibの音量の最大値（0〜255）
	/// </summary>
	const int kDxlibVolumeMax = 255;

	/// <summary>
	/// BGMボリュームの初期値
	/// </summary>
	const int kInitialBGMVolume = 50;

	/// <summary>
	/// SEボリュームの初期値
	/// </summary>
	const int kInitialSEVolume = 70;
	
}

/// <summary>
/// コンストラクタ
/// </summary>
AudioManager::AudioManager()
			: bgm_volume_(kInitialBGMVolume)
			, se_volume_(kInitialSEVolume)
{
}

/// <summary>
/// SE音源のロード
/// </summary>
int AudioManager::LoadSe(const char* kind)
{
	return LoadSoundMem(kind);
}

/// <summary>
/// SEの再生
/// </summary>
void AudioManager::PlaySe(const int handle)
{
	const int dxlib_volume = se_volume_ * (kDxlibVolumeMax / kGameVolumeMax);
	ChangeVolumeSoundMem(dxlib_volume, handle);
	PlaySoundMem(handle, DX_PLAYTYPE_BACK);
}

/// <summary>
/// SEの破棄
/// </summary>
void AudioManager::DeleteSe(const int handle)
{
	DeleteSoundMem(handle);
}

/// <summary>
/// BGM音源のロード
/// </summary>
int AudioManager::LoadBgm(const char* kind)
{
	return LoadSoundMem(kind);
}

/// <summary>
/// BGMの再生
/// </summary>
void AudioManager::PlayBgm(const int handle)
{
	current_bgm_handle_ = handle;

	const int dxlib_volume = bgm_volume_ * (kDxlibVolumeMax / kGameVolumeMax);
	ChangeVolumeSoundMem(dxlib_volume, handle);
	PlaySoundMem(handle, DX_PLAYTYPE_LOOP);
}

/// <summary>
/// BGMの破棄
/// </summary>
void AudioManager::DeleteBgm(const int handle)
{
	DeleteSoundMem(handle);
}

/// <summary>
/// BGMの一時停止
/// </summary>
void AudioManager::StopBgm(const int handle)
{
	StopSoundMem(handle);
}

/// <summary>
/// BGMの音量を設定する
/// </summary>
void AudioManager::SetBgmVolume(int volume)
{
	bgm_volume_ = volume;

	const int dxlib_volume = bgm_volume_ * kDxlibVolumeMax / kGameVolumeMax;
	ChangeVolumeSoundMem(dxlib_volume, current_bgm_handle_);
}

/// <summary>
/// SEの音量を設定する
/// </summary>
void AudioManager::SetSeVolume(int volume)
{
	se_volume_ = volume;
}

/// <summary>
/// 現在のSE音量でプレビューSEを鳴らす
/// </summary>
void AudioManager::PlaySePreview()
{
	if (preview_se_handle_ == -1)
	{
		return;
	}

	PlaySe(preview_se_handle_);
}