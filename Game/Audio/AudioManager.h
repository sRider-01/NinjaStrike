#pragma once

/// <summary>
/// BGMなどの音に関するクラス
/// </summary>
class AudioManager
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	AudioManager();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~AudioManager() = default;

	/// <summary>
	/// SE音源のロード
	/// </summary>
	/// <param name="kind">SEの種類</param>
	int LoadSe(const char* kind);

	/// <summary>
	/// SEの再生
	/// </summary>
	/// <param name="handle">SEのハンドル</param>
	void PlaySe(const int handle);

	/// <summary>
	/// SEの破棄
	/// </summary>
	/// <param name="handle">SEのハンドル</param>
	void DeleteSe(const int handle);

	/// <summary>
	/// BGM音源のロード
	/// </summary>
	/// <param name="kind">BGMの種類</param>
	int LoadBgm(const char* kind);

	/// <summary>
	/// BGMの再生
	/// </summary>
	/// <param name="handle">BGMのハンドル</param>
	void PlayBgm(const int handle);

	/// <summary>
	/// BGMの破棄
	/// </summary>
	/// <param name="handle">BGMのハンドル</param>
	void DeleteBgm(const int handle);

	/// <summary>
	/// BGMの一時停止
	/// </summary>
	void StopBgm(const int handle);

	/// <summary>
	/// BGMの音量を設定する（0〜100）
	/// </summary>
	/// <param name="volume">音量（0=無音 100=最大）</param>
	void SetBgmVolume(int volume);

	/// <summary>
	/// SEの音量を設定する（0〜100）
	/// </summary>
	/// <param name="volume">音量（0=無音 100=最大）</param>
	void SetSeVolume(int volume);

	/// <summary>
	/// BGMの音量を返す
	/// </summary>
	int GetBgmVolume() const
	{
		return bgm_volume_;
	}

	/// <summary>
	/// SEの音量を返す
	/// </summary>
	int GetSeVolume() const
	{
		return se_volume_;
	}

	/// <summary>
	/// プレビュー用SEハンドルをセットする
	/// SE音量調整時の確認音として使う
	/// </summary>
	/// <param name="handle">SEハンドル</param>
	void SetPreviewSeHandle(int handle)
	{
		preview_se_handle_ = handle;
	}

	/// <summary>
	/// 現在のSE音量でプレビューSEを鳴らす
	/// </summary>
	void PlaySePreview();

	/// <summary>
	/// インスタンスを取得する
	/// （シングルトンデザインパターン）
	/// </summary>
	/// <returns>インスタンス</returns>
	static AudioManager* GetInstance()
	{
		//生成されてないなら
		if (audio_manager_instance_ == nullptr) {
			//動的生成
			audio_manager_instance_ = new AudioManager();
		}

		//audio_manager_instance_のインスタンスを返す
		return audio_manager_instance_;
	}

	/// <summary>
	/// インスタンスを削除
	/// ゲーム終了時に必ず呼ぶ
	/// </summary>
	static void Destroy()
	{
		//生成済みなら
		if (audio_manager_instance_ != nullptr) {

			//Deleteする
			delete audio_manager_instance_;

			//instance_の先をnullptrにする
			audio_manager_instance_ = nullptr;
		}
	}

private:
	/// <summary>
	/// AudioManagerのインスタンス
	/// </summary>
	static AudioManager* audio_manager_instance_;

	/// <summary>
	/// 現在再生中のBGMハンドル
	/// </summary>
	int current_bgm_handle_ = -1;

	/// <summary>
	/// SE音量確認用のプレビューSEハンドル
	/// </summary>
	int preview_se_handle_ = -1;
	
	/// <summary>
	/// BGMの音量（0〜100）
	/// </summary>
	int bgm_volume_ = 0;

	/// <summary>
	/// SEの音量（0〜100）
	/// </summary>
	int se_volume_ = 0;


};