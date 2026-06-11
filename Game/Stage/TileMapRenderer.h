#pragma once
#include "TileMap/TileMap.h"     
#include "TileMap/TileDatabase.h"
#include "TileMap/TileSet.h"
#include "DxLib.h" 

/// <summary>
/// タイルマップを切り出して描画するクラス
/// </summary>
class TileMapRenderer
{
public:
    /// <summary>
    /// タイルマップを描画する
    /// </summary>
    /// <param name="map">タイルIDを保持しているマップデータ</param>
    /// <param name="db">タイルID →（タイルセット内の番号 / 当たり判定など）を引くためのデータベース</param>
    /// <param name="tileset">タイルセット</param>
    /// <param name="camera_x">カメラのX座標（画面表示は world_x - camera_x）</param>
    /// <param name="camera_y">カメラのY座標（画面表示は world_y - camera_y）</param>
    void Draw(const TileMap& map, const TileDatabase& db, const TileSet& tileset,
        int camera_x, int camera_y) const;
};