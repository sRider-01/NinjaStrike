#pragma once
#include <vector>

/// <summary>
/// タイルの情報
/// </summary>
struct TileDef
{
    /// <summary>
    /// タイルセット内の番号
    /// </summary>
    int tile_index = 0;
    
    /// <summary>
    /// 当たり判定を持つか？
    /// </summary>
    bool is_solid = 0;
    
    /// <summary>
    /// 攻撃がヒットするか？
    /// </summary>
    bool is_hitable = 0;

    /// <summary>
    /// この種類のタイルの最大HP（壊れないなら0）
    /// </summary>
    int max_hp = 0;

    /// <summary>
    /// ヒット時に一瞬だけ切り替える tile_id（無ければ -1）
    /// </summary>
    int hit_tile_id = -1;
};

/// <summary>
/// タイルの情報が何かを記載したクラス
/// </summary>
class TileDatabase
{
public:
    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize();

    /// <summary>
    /// タイルIDが地面か？
    /// </summary>
    /// <param name="tile_id">タイルID</param>
    /// <returns>地面/地面でない</returns>
    bool IsSolid(int tile_id) const;

    /// <summary>
    /// タイルIDが攻撃ヒット判定をもつか？
    /// </summary>
    /// <param name="tile_id">タイルID</param>
    /// <returns>攻撃判定をもつ/もたない</returns>
    bool IsHitable(int tile_id) const;

    /// <summary>
    /// タイルIDの最大HPを取得する
    /// </summary>
    /// <param name="tile_id">タイルID</param>
    /// <returns>最大HP</returns>
    int GetMaxHp(int tile_id) const;

    /// <summary>
    /// タイルIDを取得する
    /// </summary>
    /// <param name="id">タイル</param>
    /// <returns>タイルID</returns>
    const TileDef& GetDef(int id) const
    {
        //異常値なら
        if (id < 0 || id >= static_cast<int>(tile_defs_.size()))
        {
            //空として返す
            return tile_defs_[0];
        }
        return tile_defs_[id];
    }

private:
    /// <summary>
    /// タイル定義の配列
    /// </summary>
    std::vector<TileDef> tile_defs_;
};