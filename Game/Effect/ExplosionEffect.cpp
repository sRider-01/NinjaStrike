#include "ExplosionEffect.h"
#include "Camera/Camera2D.h"
#include "DxLib.h"

/// <summary>
/// ExplosionEffect.cppで使う定数
/// </summary>
namespace
{
    /// <summary>
    /// 数値を半分にする
    /// </summary>
    const float kHalf = 0.5f;

    /// <summary>
    /// エフェクトの描画オフセットX
    /// </summary>
    const float kDrawOffsetX = 20;

    /// <summary>
    /// エフェクトの描画オフセットY
    /// </summary>
    const float kDrawOffsetY = 10;

    /// <summary>
    /// 先頭のフレームの値
    /// </summary>
    const int kHeadFrameNum = 0;
}

/// <summary>
/// コンストラクタ
/// </summary>
ExplosionEffect::ExplosionEffect()
    : frames_(nullptr)
    , frame_count_(0)
    , interval_(0.0f)
    , current_frame_(0)
    , elapsed_time_(0.0f)
{
}

/// <summary>
/// 毎フレームの更新処理
/// </summary>
void ExplosionEffect::Update(float delta_time)
{
    //このエフェクトが非アクティブなら何もしない
    if (!effect_info_.is_active_)
    {
        return;
    }
    if (frames_ == nullptr || frame_count_ <= 0 || interval_ <= 0.0f)
    {
        effect_info_.is_active_ = false;
        return;
    }

    elapsed_time_ += delta_time;

    //経過時間が１フレームにかける時間より大きい間
    while (elapsed_time_ >= interval_)
    {
        //１フレームにかける時間分経過時間を引く
        elapsed_time_ -= interval_;

        //表示フレームを次へ
        current_frame_ += 1;

        //最後まで行ったら終了
        if (current_frame_ >= frame_count_)
        {
            effect_info_.is_active_ = false;
            current_frame_ = frame_count_ - 1;
            break;
        }
    }

    //current_frame_が範囲外なら範囲内に丸める
    if (current_frame_ < 0)
    {
        current_frame_ = 0;
    }
    if (current_frame_ >= frame_count_)
    {
        current_frame_ = frame_count_ - 1;
    }

    if (current_frame_ != prev_frame_)
    {
        prev_frame_ = current_frame_;

        effect_handle_ = frames_[current_frame_];
        if (effect_handle_ < 0)
        {
            effect_info_.is_active_ = false;
            return;
        }
    }
    
    //座標更新は毎フレーム実施
    draw_x_ = camera_->ToScreenX(effect_info_.pos_x) + kDrawOffsetX;
    draw_y_ = camera_->ToScreenY(effect_info_.pos_y) + kDrawOffsetY;

}

/// <summary>
/// 毎フレームの描画処理
/// </summary>
void ExplosionEffect::Render()
{
    if (!effect_info_.is_active_)
    {
        return;
    }
    if (frames_ == nullptr || frame_count_ <= 0)
    {
        return;
    }

    DrawGraph(static_cast<int>(draw_x_), static_cast<int>(draw_y_), effect_handle_, true);
}

/// <summary>
/// エフェクトの発生
/// </summary>
void ExplosionEffect::EffectAct(float pos_x, float pos_y, bool turn_flag)
{
    effect_info_.is_active_ = true;

    effect_info_.pos_x = pos_x;
    effect_info_.pos_y = pos_y;

    current_frame_ = 0;
    prev_frame_ = -1;
    elapsed_time_ = 0.0f;
}

/// <summary>
/// 初期化処理
/// </summary>
void ExplosionEffect::Initialize()
{
    if (frames_ == nullptr || frame_count_ <= 0)
    {
        return;
    }

    const int head_handle = frames_[kHeadFrameNum];

    //エフェクトの先頭フレームのハンドルが読み込めてないなら
    if (head_handle < 0)
    {
        return;
    }
    
    GetGraphSize(head_handle, &width_, &height_);
}

/// <summary>
/// フレーム情報のセット
/// </summary>
void ExplosionEffect::SetFrames(const int* frames, int frame_count, float interval)
{
    frames_ = frames;
    frame_count_ = frame_count;
    interval_ = interval;
}

