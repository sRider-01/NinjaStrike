#include "SlashEffect.h"
#include "Camera/Camera2D.h"
#include "DxLib.h"

/// <summary>
/// SlashEffect.cppで使う定数
/// </summary>
namespace
{
    /// <summary>
    /// 数値を半分にする
    /// </summary>
    const float kHalf = 0.5f;

    /// <summary>
    /// 左向き攻撃エフェクト発生位置調整用
    /// </summary>
    const float kLeftAttackEffectAdjust = 250.0f;

    /// <summary>
    /// PivotXを調整する値
    /// </summary>
    const float kPivotXAdjust = 0.15f;
   
}
/// <summary>
/// コンストラクタ
/// </summary>
SlashEffect::SlashEffect()
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
void SlashEffect::Update(float delta_time)
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

        GetGraphSize(effect_handle_, &width_, &height_);

        pivot_x_ = static_cast<float>(width_) * kPivotXAdjust;
        pivot_y_ = static_cast<float>(height_);

        draw_x_ = camera_->ToScreenX(effect_info_.pos_x);
        draw_y_ = camera_->ToScreenY(effect_info_.pos_y) + width_ * kHalf;
    }

}

/// <summary>
/// 毎フレームの描画処理
/// </summary>
void SlashEffect::Render()
{
    if (!effect_info_.is_active_)
    {
        return;
    }
    if (frames_ == nullptr || frame_count_ <= 0)
    {
        return;
    }

    DrawRotaGraph2F(
        draw_x_, draw_y_,        //画面上の中心を置く座標
        pivot_x_, pivot_y_,      //画像内の回転中心
        1.0f,                    //拡大率
        angle_,                  //回転角
        effect_handle_,          //画像ハンドル
        TRUE,                    //透過
        !turn_flag_               //左右反転
    );
}

/// <summary>
/// エフェクトの発生
/// </summary>
void SlashEffect::EffectAct(float pos_x, float pos_y,bool turn_flag)
{
    effect_info_.is_active_ = true;
    
    turn_flag_ = turn_flag;

    //左右で角度とエフェクト発生位置を切り替え
    if (turn_flag_)
    {
        effect_info_.pos_x = pos_x - kLeftAttackEffectAdjust;
        effect_info_.pos_y = pos_y;
        angle_ = 0;
    }
    else
    {
        effect_info_.pos_x = pos_x;
        effect_info_.pos_y = pos_y;
        angle_ = 0;
    }

    current_frame_ = 0;
    elapsed_time_ = 0.0f;
}


/// <summary>
/// フレーム情報のセット
/// </summary>
void SlashEffect::SetFrames(const int* frames, int frame_count, float interval)
{
    frames_ = frames;
    frame_count_ = frame_count;
    interval_ = interval;
}

