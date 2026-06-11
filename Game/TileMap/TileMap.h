#pragma once
#include <vector>
#include <string>

/// <summary>
/// 前方宣言
/// </summary>
class TileDatabase;

/// <summary>
/// タイルマップについてのクラス
/// </summary>
class TileMap
{
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    TileMap();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~TileMap();
public:
    /// <summary>
    /// CSVファイルを読み込む
    /// </summary>
    /// <param name="file_path">CSVファイルのパス</param>
    /// <returns>成功/失敗</returns>
    bool LoadFromCSV(const char* file_path);

    /// <summary>
    /// タイルIDをセットする
    /// </summary>
    /// <param name="x">タイルのX座標</param>
    /// <param name="y">タイルのY座標</param>
    /// <param name="tile_id">セットするタイルID</param>
    void SetTileId(int tile_x, int tile_y, int tile_id);

    /// <summary>
    /// タイルのIDを受け取る
    /// </summary>
    /// <param name="x">タイルのX座標</param>
    /// <param name="y">タイルのY座標</param>
    /// <returns>タイルID</returns>
    int GetTileId(int x, int y) const;

    /// <summary>
    /// タイルのHP配列の初期化
    /// </summary>
    /// <param name="tile_database">使うタイルデータベース</param>
    void InitializeTileHp(const TileDatabase& tile_database);

    /// <summary>
    /// 指定タイルにダメージ
    /// </summary>
    /// <param name="tile_x">指定タイルX座標</param>
    /// <param name="tile_y">指定タイルY座標</param>
    /// <param name="damage">ダメージ量</param>
    /// <param name="tile_database">使うタイルデータベース</param>
    /// <returns>成功／失敗</returns>
    bool ApplyDamageToTile(int tile_x, int tile_y, int damage, const TileDatabase& tile_database);

    /// <summary>
    /// ヒット状態の更新（時間経過で元に戻す）
    /// </summary>
    void UpdateHitTiles(float delta_time);

    /// <summary>
    /// CSVの値をゲーム画面表示用IDに変換する
    /// </summary>
    /// <param name="csv_id">CSVの値</param>
    /// <returns>ゲーム画面表示用ID</returns>
    int ConvertCsvIdToGameTileId(int csv_id) const;

    /// <summary>
    /// タイルのサイズを設定する
    /// </summary>
    /// <param name="tile_size">タイルのサイズ</param>
    void SetTileSize(int tile_size)
    {
        tile_size_ = tile_size;
    }

    /// <summary>
    /// タイルのサイズを受け取る
    /// </summary>
    /// <returns>タイルのサイズ</returns>
    int GetTileSize() const
    {
        return tile_size_;
    }

    /// <summary>
    /// タイルの幅を受け取る
    /// </summary>
    /// <returns>タイルの幅</returns>
    int GetWidth() const
    {
        return width_;
    }

    /// <summary>
    /// タイルの高さを受け取る
    /// </summary>
    /// <returns>タイルの高さ</returns>
    int GetHeight() const
    {
        return height_;
    }

    /// <summary>
    /// タイル座標を、tile_hp_用のインデックスに変換
    /// </summary>
    /// <param name="tile_x">タイルのX座標</param>
    /// <param name="tile_y">タイルのY座標</param>
    /// <returns>tile_hp_用のインデックス</returns>
    int ToHpIndex(int tile_x, int tile_y) const
    {
        return tile_y * width_ + tile_x;
    }

private:
    /// <summary>
    /// タイルのサイズ
    /// </summary>
    int tile_size_ = 0;

    /// <summary>
    /// タイルマップの横幅
    /// </summary>
    int width_ = 0;

    /// <summary>
    /// タイルマップの高さ
    /// </summary>
    int height_ = 0;

    /// <summary>
    /// タイルマップの配列
    /// </summary>
    std::vector<std::vector<int> > tiles_;

    /// <summary>
    /// 各タイルの現在HP
    /// </summary>
    std::vector<int> tile_hp_;

    /// <summary>
    /// ヒット状態の残り時間（秒）
    /// </summary>
    std::vector<float> tile_hit_timer_;

    /// <summary>
    /// ヒット状態が終わったら戻す元のtile_id
    /// </summary>
    std::vector<int> tile_base_tile_id_;

    /// <summary>
    /// ヒット状態にしている tile_id（現在がヒット状態か判定するため）
    /// < / summary>
    std::vector<int> tile_hit_tile_id_;
};