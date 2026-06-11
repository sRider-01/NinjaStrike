#pragma once
#include <array>
#include <iostream>
/// <summary>
/// フェードアウトのクラス
/// </summary>
class FadeManager
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	FadeManager();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~FadeManager();

	/// <summary>
	/// インスタンスを取得する
	/// （シングルトンデザインパターン）
	/// </summary>
	/// <returns>インスタンス</returns>
	static FadeManager* GetInstance()
	{
		//生成されてないなら
		if (fade_instance_ == nullptr) {
			//動的生成
			fade_instance_ = new FadeManager();
		}

		//インスタンスを返す
		return fade_instance_;
	}

	/// <summary>
	/// インスタンスを削除
	/// ゲーム終了時に必ず呼ぶ
	/// </summary>
	static void Destroy()
	{
		//生成済みなら
		if (fade_instance_ != nullptr) {

			//Deleteする
			delete fade_instance_;

			//instance_の先をnullptrにする
			fade_instance_ = nullptr;
		}
	}
	/// <summary>
	/// フェードイン開始関数
	/// </summary>
	/// <param name="time">フェードインの時間</param>
	void StartFadeIn(float time);

	/// <summary>
	/// フェードアウト開始関数
	/// </summary>
	/// <param name="time">フェードアウトの時間</param>
	void StartFadeOut(float time);

	/// <summary>
	/// 経過時間の更新処理
	/// </summary>
	/// <param name="delta_time">前回実行フレームからの経過時間（秒）</param>
	void Update(float delta_time);


	/// <summary>
	/// 画像のロード
	/// </summary>
	void LoadGraphics();

	/// <summary>
	/// 画像の削除
	/// </summary>
	void DeleteGraphics();

	/// <summary>
	/// 黒画の描画
	/// </summary>
	void DrawGraphics();

	/// <summary>
	/// フェードが終わったか
	/// </summary>
	bool IsFadeFinished()
	{
		return is_fade_finished_;
	}

private:

	/// <summary>
	/// フェードするかどうか
	/// </summary>
	enum class FadeMode
	{
		None, //なし
		In,   //フェードイン
		Out,  //フェードアウト
	};

	/// <summary>
	/// インスタンス
	/// </summary>
	static FadeManager* fade_instance_;

	/// <summary>
	/// 画像のハンドル
	/// </summary>
	int image_handle_ = 0;

	/// <summary>
	/// フェードにかけた時間
	/// </summary>
	float alpha_elapsed_time_ = 0.0f;

	/// <summary>
	/// このフェードの総時間
	/// </summary>
	float fade_seconds_ = 0.0f;

	/// <summary>
	/// 経過時間
	/// </summary>
	float elapsed_time_ = 0.0f;

	/// <summary>
	/// 透明度の計算に使う割合値
	/// </summary>
	float alpha_rate = 0.0f;

	/// <summary>
	/// 実際のフェードに使うalpha値
	/// </summary>
	float fade_alpha = 0.0f;

	/// <summary>
	/// フェードするかどうか。初期は「なし」
	/// </summary>
	FadeMode fade_mode_ = FadeMode::None;

	/// <summary>
	/// フェードが終わったか
	/// </summary>
	bool is_fade_finished_ = false;
};
