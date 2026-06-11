#pragma once

/// <summary>
/// タイルセットを扱うクラス
/// </summary>
class TileSet
{
public:
    /// <summary>
    /// タイルセットをロードする
    /// </summary>
    /// <param name="path">タイルセットのパス</param>
    /// <param name="tile_w">タイルの幅</param>
    /// <param name="tile_h">タイルの高さ</param>
    /// <param name="spacing">タイル間の隙間</param>
    /// <param name="margin">タイル間の余白</param>
    /// <returns>成功/失敗</returns>
    bool Load(const char* path, int tile_w, int tile_h, int spacing = 0, int margin = 0);


    /// <summary>
    /// タイルセット画像の切り出す位置を計算する
    /// </summary>
    /// <param name="tile_index">何番目のタイル番号を使うか</param>
    /// <param name="src_x">切り出し開始位置X</param>
    /// <param name="src_y">切り出し開始位置Y</param>
    /// <param name="w">切り出す幅</param>
    /// <param name="h">切り出す高さ</param>
    void GetSrcRect(int tile_index, int& src_x, int& src_y, int& width, int& height) const;


    /// <summary>
    /// タイルの幅を取得
    /// </summary>
    /// <returns>タイルの幅</returns>
    int GetTileWidth() const 
    {
        return tile_w_; 
    }
    
    /// <summary>
    /// タイルの高さを取得
    /// </summary>
    /// <returns>タイルの高さ</returns>
    int GetTileHeight() const 
    { 
        return tile_h_; 
    }

    /// <summary>
    /// ハンドルを取得する
    /// </summary>
    /// <returns>ハンドル</returns>
    int GetHandle() const
    {
        return tile_set_handle_;
    }

private:
    /// <summary>
    /// タイルセットのハンドル
    /// </summary>
    int tile_set_handle_ = -1;

    /// <summary>
    /// タイルの幅
    /// </summary>
    int tile_w_ = 0;

    /// <summary>
    /// タイルの高さ
    /// </summary>
    int tile_h_ = 0;

    /// <summary>
    /// タイルとタイルの間隔
    /// </summary>
    int spacing_ = 0;

    /// <summary>
    /// 画像端の余白
    /// </summary>
    int margin_ = 0; 

    /// <summary>
    /// 元画像の幅
    /// </summary>
    int image_w_ = 0;

    /// <summary>
    /// 元画像の高さ
    /// </summary>
    int image_h_ = 0;
};