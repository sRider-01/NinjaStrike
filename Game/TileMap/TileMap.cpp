#include "TileMap.h"
#include "TileDatabase.h"
#include <fstream>
#include <sstream>

/// <summary>
/// TileMap.cppで使う定数
/// </summary>
namespace
{
    /// <summary>
    /// ヒット状態を維持する秒数
    /// </summary>
    const float kHitFlashSeconds = 0.03f;
}

/// <summary>
/// コンストラクタ
/// </summary>
TileMap::TileMap()
{
}

/// <summary>
/// デストラクタ
/// </summary>
TileMap::~TileMap()
{
}

/// <summary>
/// CSVファイルを読み込む
/// </summary>
bool TileMap::LoadFromCSV(const char* file_path)
{
    //ファイルを開く
    std::ifstream ifstream_instance(file_path);

    if (!ifstream_instance)
    {
        //ファイル読み込み失敗
        return false;
    }

    //タイルマップの横幅、高さ、配列をクリア
    tiles_.clear();
    tile_hp_.clear();
    tile_hit_timer_.clear();
    tile_base_tile_id_.clear();
    tile_hit_tile_id_.clear();
    width_ = 0;
    height_ = 0;


    std::string line;
    while (std::getline(ifstream_instance, line))
    {
        if (line.empty())
        {
            continue;
        }

        std::stringstream string_stream(line);
        std::string token;

        //','がstring_streamから出るまでの文字を読み取りtokenに入れる ←が失敗するとき＝もう読み取れない場合
        std::vector<int> row;
        while (std::getline(string_stream, token, ','))
        {
            if (!token.empty() && token.back() == '\r')
            {
                token.pop_back();
            }

            int csv_id = 0;
            if (!token.empty())
            {
                csv_id = std::stoi(token);
            }
            const int game_tile_id = ConvertCsvIdToGameTileId(csv_id);
            row.push_back(game_tile_id);
        }

        if (row.empty())
        {
            continue;
        }

        if (width_ < static_cast<int>(row.size()))
        {
            width_ = static_cast<int>(row.size());
        }

        tiles_.push_back(row);
    }

    height_ = static_cast<int>(tiles_.size());

    //行ごとに長さが違うCSVだと崩れるので、足りない分は0で埋める
    for (int y = 0; y < height_; ++y)
    {
        while (static_cast<int>(tiles_[y].size()) < width_)
        {
            tiles_[y].push_back(0);
        }
    }

    return true;
}

/// <summary>
/// タイルのIDを受け取る
/// </summary>
int TileMap::GetTileId(int x, int y) const
{
    //マップ範囲外なら
    if (x < 0 || y < 0 || x >= width_ || y >= height_)
    {
        //何もしない
        return 0;
    }

    //y行目のx列目のタイルIDを返す
    return tiles_[y][x];
}

/// <summary>
/// タイルIDをセットする
/// </summary>
void TileMap::SetTileId(int x, int y, int tile_id)
{
    tiles_[y][x] = tile_id;
}

/// <summary>
/// タイルのHP配列の初期化
/// </summary>
void TileMap::InitializeTileHp(const TileDatabase& tile_database)
{
    tile_hp_.clear();
    tile_hp_.resize(static_cast<std::size_t>(width_ * height_), 0);

    tile_hit_timer_.clear();
    tile_hit_timer_.resize(static_cast<std::size_t>(width_ * height_), 0.0f);

    tile_hit_tile_id_.clear();
    tile_hit_tile_id_.resize(static_cast<std::size_t>(width_ * height_), 0);

    tile_base_tile_id_.clear();
    tile_base_tile_id_.resize(static_cast<std::size_t>(width_ * height_), 0);

    for (int y = 0; y < height_; ++y)
    {
        for (int x = 0; x < width_; ++x)
        {
            const int index = ToHpIndex(x, y);
            tile_hp_[index] = tile_database.GetMaxHp(tiles_[y][x]);
        }
    }
}

/// <summary>
/// 指定タイルにダメージ
/// </summary>
bool TileMap::ApplyDamageToTile(int tile_x, int tile_y, int damage, const TileDatabase& tile_database)
{

    //HP配列が未初期化なら何もしない
    if (static_cast<int>(tile_hp_.size()) != width_ * height_)
    {
        return false;
    }

    const int tile_id = tiles_[tile_y][tile_x];

    //当たり判定をとるタイルでなければ何もしない
    if (!tile_database.IsHitable(tile_id))
    {
        return false;
    }

    const int index = ToHpIndex(tile_x, tile_y);

    //初期HPが0のまま等のケースがあれば、最大HPで補正
    if (tile_hp_[index] <= 0)
    {
        tile_hp_[index] = tile_database.GetMaxHp(tile_id);
    }

    tile_hp_[index] -= damage;

    //HPが0になったら
    if (tile_hp_[index] <= 0)
    {
        //空タイル(0)にする
        tiles_[tile_y][tile_x] = 0;
        tile_hp_[index] = 0;

        //ヒット状態情報もクリア
        if (static_cast<int>(tile_hit_timer_.size()) == width_ * height_)
        {
            tile_hit_timer_[index] = 0.0f;
            tile_base_tile_id_[index] = 0;
            tile_hit_tile_id_[index] = 0;
        }

        return true;
    }
    else
    {
        //ヒット状態管理配列が初期化済みのときだけ実行
        if (static_cast<int>(tile_hit_timer_.size()) == width_ * height_)
        {
            //TileDefを取得
            const TileDef& tile_def = tile_database.GetDef(tile_id);

            const int hit_tile_id = tile_def.hit_tile_id;
            if (hit_tile_id >= 0)
            {
                //初回ヒット時だけ「元のID」を保存して見た目を差し替え
                if (tile_hit_timer_[index] <= 0.0f)
                {
                    tile_base_tile_id_[index] = tile_id;
                    tile_hit_tile_id_[index] = hit_tile_id;
                    tiles_[tile_y][tile_x] = hit_tile_id;
                }

                //連続ヒット時はタイマーだけ延長
                tile_hit_timer_[index] = kHitFlashSeconds;
            }
        }
        return false;
    }

}

/// <summary>
/// ヒット状態の更新（時間経過で元に戻す）
/// </summary>
void TileMap::UpdateHitTiles(float delta_time)
{
    if (static_cast<int>(tile_hit_timer_.size()) != width_ * height_)
    {
        return;
    }

    for (int y = 0; y < height_; ++y)
    {
        for (int x = 0; x < width_; ++x)
        {
            const int index = ToHpIndex(x, y);

            if (tile_hit_timer_[index] <= 0.0f)
            {
                continue;
            }

            tile_hit_timer_[index] -= delta_time;
            if (tile_hit_timer_[index] > 0.0f)
            {
                continue;
            }

            tile_hit_timer_[index] = 0.0f;

            //破壊済みなら次へ
            if (tiles_[y][x] == 0)
            {
                continue;
            }

            //ヒット状態の見た目になっている場合だけ戻す
            if (tiles_[y][x] == tile_hit_tile_id_[index])
            {
                tiles_[y][x] = tile_base_tile_id_[index];
            }

            tile_hit_tile_id_[index] = 0;
        }
    }
}

/// <summary>
/// CSVの値をゲーム画面表示用IDに変換する
/// </summary>
int TileMap::ConvertCsvIdToGameTileId(int csv_id) const
{
    switch (csv_id)
    {
        case -1: return 0;   // 空
        case 0:  return 4;   // 地面左端
        case 1:  return 1;   // 地面
        case 2:  return 5;   // 地面右端
        case 11: return 6;   // 地面中左
        case 12: return 7;   // 地面中中
        case 13: return 8;   // 地面中右
        case 20: return 9;   // 地面中中石あり
        case 22: return 10;  // 地面下左
        case 23: return 11;  // 地面下中
        case 24: return 12;  // 地面下右
        case 63: return 2;   // 破壊可能
        case 56: return 3;   // 破壊可能ヒット
        default: return 0;   // 不明なら空
    }
}