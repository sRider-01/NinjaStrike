#include "TileSet.h"
#include "DxLib.h"
#include <algorithm>


/// <summary>
/// タイルセットをロードする
/// </summary>
bool TileSet::Load(const char* path, int tile_w, int tile_h, int spacing, int margin)
{
    tile_set_handle_ = LoadGraph(path);
    if (tile_set_handle_ < 0)
    {
        return false;
    }

    tile_w_ = tile_w;
    tile_h_ = tile_h;
    spacing_ = spacing;
    margin_ = margin;

    GetGraphSize(tile_set_handle_, &image_w_, &image_h_);
    return true;
}

/// <summary>
/// タイルセット画像の切り出す位置を計算する
/// </summary>
void TileSet::GetSrcRect(int tile_index, int& src_x, int& src_y, int& width, int& height) const
{
    //切り出す幅、高さをセット
    width = tile_w_;
    height = tile_h_;

    //１タイル進むごとの移動量(タイル幅＋隙間)
    const int stride_x = tile_w_ + spacing_;
    const int stride_y = tile_h_ + spacing_;

    //余白を除いた「有効な領域」
    const int usable_w = image_w_ - margin_ * 2;
    const int usable_h = image_h_ - margin_ * 2;

    //異常値なら
    if (stride_x <= 0 || stride_y <= 0 || usable_w <= 0 || usable_h <= 0)
    {
        //0として返す
        src_x = 0;
        src_y = 0;
        return;
    }

    //横に何枚並ぶか
    const int columns = (usable_w + spacing_) / stride_x;
    const int rows = (usable_h + spacing_) / stride_y;

    //異常値なら
    if (columns <= 0 || rows <= 0)
    {
        //0として返す
        src_x = 0;
        src_y = 0;
        return;
    }

    //tile_index が範囲外なら、範囲内に丸める
    const int max_index = columns * rows - 1;

    tile_index = std::clamp(tile_index, 0, max_index);

    //tile_index を「列(tx)・行(ty)」に変換
    const int tx = tile_index % columns;
    const int ty = tile_index / columns;

    //画像内の切り出し開始位置（左上）を計算
    src_x = margin_ + tx * stride_x;
    src_y = margin_ + ty * stride_y;
}