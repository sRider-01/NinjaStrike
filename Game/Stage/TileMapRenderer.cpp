#include "TileMapRenderer.h"

/// <summary>
/// タイルマップを描画する
/// </summary>
void TileMapRenderer::Draw(const TileMap& map, const TileDatabase& db, const TileSet& tileset,
    int camera_x, int camera_y) const
{
    const int tile_size = map.GetTileSize();
    const int width = map.GetWidth();
    const int height = map.GetHeight();

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            //CSVのタイルIDを取得
            const int id = map.GetTileId(x, y);

            //タイルIDから「どの絵を描くか（tile_index）」を取得
            const TileDef& def = db.GetDef(id);

            //tile_index が -1 ならスキップ
            if (def.tile_index < 0)
            {
                continue;
            }

            //タイルセット画像から切り出すsrcを計算
            int src_x = 0;
            int src_y = 0;
            int src_w = 0;
            int src_h = 0;
            tileset.GetSrcRect(def.tile_index, src_x, src_y, src_w, src_h);

            //タイルのマップ上の座標
            const int world_x = x * tile_size;
            const int world_y = y * tile_size;

            //カメラを引いて画面座標に変換
            const int screen_x = world_x - camera_x;
            const int screen_y = world_y - camera_y;

            //タイルセット画像から矩形を切り出して描画
            DrawRectGraph(screen_x, screen_y,
                src_x, src_y, src_w, src_h,
                tileset.GetHandle(), TRUE);
        }
    }
}