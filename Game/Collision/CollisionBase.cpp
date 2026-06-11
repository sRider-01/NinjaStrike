#include "CollisionBase.h"
#include <algorithm>
#include <cmath>

/// <summary>
/// lŠpŒ`“¯m‚ªG‚ê‚½‚©
/// </summary>
bool CollisionBase::IsRectOverlap(const RectF& owner_rect, const RectF& target_rect)
{
    if (owner_rect.right < target_rect.left)
    {
        return false;
    }
    
    if (owner_rect.left > target_rect.right)
    {
        return false; 
    }
    
    
    if (owner_rect.bottom < target_rect.top) 
    {
        return false;
    }
    
    
    if (owner_rect.top > target_rect.bottom)
    {
        return false; 
    }
    
    return true;
}

/// <summary>
/// ‰~“¯m‚ªG‚ê‚½‚©
/// </summary>
bool CollisionBase::IsCircleOverlap(const CircleF& owner_circle, const CircleF& target_circle)
{
    //’†SXÀ•W“¯m‚Ì·‚ğ‚Æ‚é
    const float dx = owner_circle.center_x - target_circle.center_x;
    
    //’†SYÀ•W“¯m‚Ì·‚ğ‚Æ‚é
    const float dy = owner_circle.center_y - target_circle.center_y;
    
    //”¼Œa“¯m‚ğ‘«‚µ‡‚í‚¹‚é
    const float r = owner_circle.radius + target_circle.radius;

    //“ñæ‚µ‚½‹——£(X,Y‚Ì·‚Ì“ñæ)‚ª”¼Œa‚Ì˜a‚Ì“ñæˆÈ“à‚È‚ç“–‚½‚è
    return (dx * dx + dy * dy) <= (r * r);
}

/// <summary>
/// ‰~‚ÆlŠpŒ`‚ªG‚ê‚½‚©
/// </summary>
bool CollisionBase::IsCircleRectOverlap(const CircleF& circle, const RectF& rect)
{
    //‰~‚Ì’†S‚©‚çlŠpŒ`‚Ö‚ÌuÅ‹ß“_v‚ğì¬
    const float nearest_x = std::clamp(circle.center_x, rect.left, rect.right);
    const float nearest_y = std::clamp(circle.center_y, rect.top, rect.bottom);

    const float dx = circle.center_x - nearest_x;
    const float dy = circle.center_y - nearest_y;

    return (dx * dx + dy * dy) <= (circle.radius * circle.radius);
}