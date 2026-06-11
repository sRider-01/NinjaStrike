#pragma once

/// <summary>
/// 当たり判定を扱うクラス
/// </summary>
class CollisionBase
{
public:
    /// <summary>
    /// 当たり判定の四角形の情報
    /// </summary>
    struct RectF
    {
        /// <summary>
        /// 左端
        /// </summary>
        float left;

        /// <summary>
        /// 上限
        /// </summary>
        float top;

        /// <summary>
        /// 右端
        /// </summary>
        float right;

        /// <summary>
        /// 下限
        /// </summary>
        float bottom;
    };

    /// <summary>
    /// 当たり判定の円の情報
    /// </summary>
    struct CircleF
    {
        /// <summary>
        /// 中心X座標
        /// </summary>
        float center_x;

        /// <summary>
        /// 中心Y座標
        /// </summary>
        float center_y;

        /// <summary>
        /// 半径
        /// </summary>
        float radius;
    };

    /// <summary>
    /// 四角形同士が触れたか
    /// </summary>
    /// <param name="owner_rect">呼び出し元の四角形</param>
    /// <param name="target_rect">ターゲットとなる四角形</param>
    /// <returns>触れたかどうか</returns>
    static bool IsRectOverlap(const RectF& owner_rect, const RectF& target_rect);

    /// <summary>
    /// 円同士が触れたか
    /// </summary>
    /// <param name="owner_circle">呼び出し元の円</param>
    /// <param name="target_circle">対象の円</param>
    /// <returns>触れたかどうか</returns>
    static bool IsCircleOverlap(const CircleF& owner_circle, const CircleF& target_circle);

    /// <summary>
    /// 円と四角形が触れたか
    /// </summary>
    /// <param name="circle">円</param>
    /// <param name="rect">四角形</param>
    /// <returns>触れたかどうか</returns>
    static bool IsCircleRectOverlap(const CircleF& circle, const RectF& rect);
};
