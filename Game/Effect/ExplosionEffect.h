#pragma once
#include "EffectBase.h"

/// <summary>
/// 前方宣言
/// </summary>
class Camera2D;

class ExplosionEffect : public EffectBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ExplosionEffect();

public:
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
    /// エフェクトの発生
    /// </summary>
    void EffectAct(float pos_x, float pos_y, bool turn_flag) override;

    /// <summary>
    /// 初期化処理
    /// </summary>
    void Initialize();

    /// <summary>
    /// フレーム情報のセット
    /// </summary>
    /// <param name="frames">フレームのハンドル</param>
    /// <param name="frame_count">フレーム数</param>
    /// <param name="interval">１フレームにかける時間</param>
    void SetFrames(const int* frames, int frame_count, float interval);

    /// <summary>
    /// カメラのセット
    /// </summary>
    /// <param name="camera">カメラのポインタ</param>
    void SetCamera(Camera2D* camera)
    {
        camera_ = camera;
    }

private:

    /// <summary>
    /// フレームのハンドル
    /// </summary>
    const int* frames_;

    /// <summary>
    /// フレーム数
    /// </summary>
    int frame_count_ = 0;

    /// <summary>
    /// １フレームにかける時間
    /// </summary>
    float interval_ = 0.0f;

    /// <summary>
    /// 今のフレーム
    /// </summary>
    int current_frame_ = 0;

    /// <summary>
    /// ひとつ前のフレーム（フレームが切り替わったかを見るのに使う）
    /// </summary>
    int prev_frame_ = -1;

    /// <summary>
    /// 経過時間
    /// </summary>
    float elapsed_time_ = 0.0f;

    /// <summary>
    /// 描画するX座標
    /// </summary>
    float draw_x_ = 0.0f;

    /// <summary>
    /// 描画するY座標
    /// </summary>
    float draw_y_ = 0.0f;

    /// <summary>
    /// エフェクトのハンドル
    /// </summary>
    int effect_handle_ = -1;

    /// <summary>
    /// エフェクトの幅
    /// </summary>
    int width_ = 0;

    /// <summary>
    /// エフェクトの高さ
    /// </summary>
    int height_ = 0;

    /// <summary>
    /// 左右反転フラグ
    /// </summary>
    bool turn_flag_ = false;

    /// <summary>
    /// カメラのポインタ
    /// </summary>
    Camera2D* camera_ = nullptr;
};