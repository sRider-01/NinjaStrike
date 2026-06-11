#include "TileDatabase.h"

/// <summary>
/// TileDatabase.cppで使う定数
/// </summary>
namespace
{
    /// <summary>
    /// 値がない時用
    /// </summary>
    const int kNull = -1;
    
    /// <summary>
    /// タイルの種類
    /// </summary>
    const int kTileType = 13;

    /// <summary>
    /// 描画タイプ（描画しない）
    /// </summary>
    const int kTypeNotDraw = -1;

    /// <summary>
    /// 描画タイプ（地面左端）
    /// </summary>
    const int kTypeGroundLeftEdge = 0;
    
    /// <summary>
    /// 描画タイプ（地面）
    /// </summary>
    const int kTypeGround = 1;

    /// <summary>
    /// 描画タイプ（地面右端）
    /// </summary>
    const int kTypeGroundRightEdge = 2;

    /// <summary>
    /// 描画タイプ（地面中左）
    /// </summary>
    const int kTypeMidleGroundLeft = 11;

    /// <summary>
    /// 描画タイプ（地面中中）
    /// </summary>
    const int kTypeMidleGroundCenter = 12;

    /// <summary>
    /// 描画タイプ（地面中右）
    /// </summary>
    const int kTypeMidleGroundRight = 13;

    /// <summary>
    /// 描画タイプ（地面中中石あり）
    /// </summary>
    const int kTypeMidleGroundCenterStone = 20;

    /// <summary>
    /// 描画タイプ（地面下左端）
    /// </summary>
    const int kTypeLowGroundLeftEdge = 22;

    /// <summary>
    /// 描画タイプ（地面下中）
    /// </summary>
    const int kTypeLowGroundCenter = 23;

    /// <summary>
    /// 描画タイプ（地面下右端）
    /// </summary>
    const int kTypeLowGroundRightEdge = 24;

    /// <summary>
    /// 描画タイプ（破壊可能オブジェクト：ヒット状態）
    /// </summary>
    const int kTypeBreakableHit = 56;

    /// <summary>
    /// 描画タイプ（破壊可能オブジェクト）
    /// </summary>
    const int kTypeBreakable = 63;

    /// <summary>
    /// 破壊可能オブジェクト：ヒット状態のインデックス
    /// </summary>
    const int kIndexBreakableHit = 3;

    /// <summary>
    /// 破壊可能オブジェクトのHP
    /// </summary>
    const int kBreakableHp = 3;

}

/// <summary>
/// 初期化
/// </summary>
void TileDatabase::Initialize()
{
    tile_defs_.clear();   //初期化のたびに増殖しないように
    tile_defs_.reserve(kTileType);

    //0 = 空（描画なし）
    TileDef empty;
    empty.tile_index = kTypeNotDraw;
    empty.is_solid = false;
    empty.is_hitable = false;
    empty.max_hp = 0;
    empty.hit_tile_id = kNull;
    tile_defs_.push_back(empty);

    //1 = 地面
    TileDef ground;
    ground.tile_index = kTypeGround;
    ground.is_solid = true;
    ground.is_hitable = false;
    ground.max_hp = 0;
    ground.hit_tile_id = kNull;
    tile_defs_.push_back(ground);

    //2 = 破壊可能オブジェクト
    TileDef breakable;
    breakable.tile_index = kTypeBreakable;
    breakable.is_solid = true;
    breakable.is_hitable = true;
    breakable.max_hp = kBreakableHp;
    breakable.hit_tile_id = kIndexBreakableHit;
    tile_defs_.push_back(breakable);

    //3 = 破壊可能オブジェクト（ヒット状態）
    TileDef breakable_hit;
    breakable_hit.tile_index = kTypeBreakableHit;
    breakable_hit.is_solid = true;
    breakable_hit.is_hitable = true;
    breakable_hit.max_hp = kBreakableHp;
    breakable_hit.hit_tile_id = kIndexBreakableHit;
    tile_defs_.push_back(breakable_hit);

    //4 = 地面左端
    TileDef ground_left_edge;
    ground_left_edge.tile_index = kTypeGroundLeftEdge;
    ground_left_edge.is_solid = true;
    ground_left_edge.is_hitable = false;
    ground_left_edge.max_hp = 0;
    ground_left_edge.hit_tile_id = kNull;
    tile_defs_.push_back(ground_left_edge);

    //5 = 地面右端
    TileDef ground_right_edge;
    ground_right_edge.tile_index = kTypeGroundRightEdge;
    ground_right_edge.is_solid = true;
    ground_right_edge.is_hitable = false;
    ground_right_edge.max_hp = 0;
    ground_right_edge.hit_tile_id = kNull;
    tile_defs_.push_back(ground_right_edge);

    //6 = 地面中左
    TileDef ground_midle_left_edge;
    ground_midle_left_edge.tile_index = kTypeMidleGroundLeft;
    ground_midle_left_edge.is_solid = true;
    ground_midle_left_edge.is_hitable = false;
    ground_midle_left_edge.max_hp = 0;
    ground_midle_left_edge.hit_tile_id = kNull;
    tile_defs_.push_back(ground_midle_left_edge);

    //7 = 地面中中(黒)
    TileDef ground_midle_center;
    ground_midle_center.tile_index = kTypeMidleGroundCenter;
    ground_midle_center.is_solid = true;
    ground_midle_center.is_hitable = false;
    ground_midle_center.max_hp = 0;
    ground_midle_center.hit_tile_id = kNull;
    tile_defs_.push_back(ground_midle_center);

    //8 = 地面中右
    TileDef ground_midle_right;
    ground_midle_right.tile_index = kTypeMidleGroundRight;
    ground_midle_right.is_solid = true;
    ground_midle_right.is_hitable = false;
    ground_midle_right.max_hp = 0;
    ground_midle_right.hit_tile_id = kNull;
    tile_defs_.push_back(ground_midle_right);

    //9 = 地面中中(石あり)
    TileDef ground_midle_center_stone;
    ground_midle_center_stone.tile_index = kTypeMidleGroundCenterStone;
    ground_midle_center_stone.is_solid = true;
    ground_midle_center_stone.is_hitable = false;
    ground_midle_center_stone.max_hp = 0;
    ground_midle_center_stone.hit_tile_id = kNull;
    tile_defs_.push_back(ground_midle_center_stone);

    //10 = 地面下左
    TileDef ground_low_left;
    ground_low_left.tile_index = kTypeLowGroundLeftEdge;
    ground_low_left.is_solid = true;
    ground_low_left.is_hitable = false;
    ground_low_left.max_hp = 0;
    ground_low_left.hit_tile_id = kNull;
    tile_defs_.push_back(ground_low_left);

    //11 = 地面下中
    TileDef ground_low_center;
    ground_low_center.tile_index = kTypeLowGroundCenter;
    ground_low_center.is_solid = true;
    ground_low_center.is_hitable = false;
    ground_low_center.max_hp = 0;
    ground_low_center.hit_tile_id = kNull;
    tile_defs_.push_back(ground_low_center);

    //12 = 地面下右
    TileDef ground_low_right;
    ground_low_right.tile_index = kTypeLowGroundRightEdge;
    ground_low_right.is_solid = true;
    ground_low_right.is_hitable = false;
    ground_low_right.max_hp = 0;
    ground_low_right.hit_tile_id = kNull;
    tile_defs_.push_back(ground_low_right);
}

/// <summary>
/// タイルIDが地面か？
/// </summary>
bool TileDatabase::IsSolid(int tile_id) const
{
    if (tile_id < 0 || tile_id >= static_cast<int>(tile_defs_.size()))
    {
        return false;
    }
    return tile_defs_[tile_id].is_solid;
}

/// <summary>
/// タイルIDが攻撃ヒット判定をもつか？
/// </summary>
bool TileDatabase::IsHitable(int tile_id) const
{
    if (tile_id < 0 || tile_id >= static_cast<int>(tile_defs_.size()))
    {
        return false;
    }
    return tile_defs_[tile_id].is_hitable;
}

/// <summary>
/// タイルIDの最大HPを取得する
/// </summary>
int TileDatabase::GetMaxHp(int tile_id) const
{
    if (tile_id < 0 || tile_id >= static_cast<int>(tile_defs_.size()))
    {
        return 0;
    }
    return tile_defs_[tile_id].max_hp;
}