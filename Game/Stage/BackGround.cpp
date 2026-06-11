#include "BackGround.h"
#include "DxLib.h"

/// <summary>
/// BackGround.cppで使う定数
/// </summary>
namespace
{
    /// <summary>
    /// 背景のパスリスト
    /// </summary>
    const char* kBgPaths[] =
    {
        "Assets/Image/BackGround/layer 1 florest.png",
        "Assets/Image/BackGround/layer 2 florest fog.png",
        "Assets/Image/BackGround/layer 3 grass.png",
        "Assets/Image/BackGround/layer 4 mountain.png",
        "Assets/Image/BackGround/layer 5 mountain and sky.png",
    };
}

/// <summary>
/// コンストラクタ
/// </summary>
BackGround::BackGround()
{
}

/// <summary>
/// デストラクタ
/// </summary>
BackGround::~BackGround()
{
}

/// <summary>
/// 初期化処理
/// </summary>
void BackGround::Initialize()
{
    layer_handles_.clear();

    const int count = static_cast<int>(sizeof(kBgPaths) / sizeof(kBgPaths[0]));
    for (int i = 0; i < count; ++i)
    {
        const bool ok = AddLayer(kBgPaths[i]);
    }
}
/// <summary>
/// 背景レイヤー画像を追加してロードする
/// </summary>
bool BackGround::AddLayer(const char* path)
{
    const int handle = LoadGraph(path);
    if (handle < 0)
    {
        return false;
    }

    layer_handles_.push_back(handle);
    return true;
}

/// <summary>
/// 背景を描画する（画面サイズに拡大して、登録順に重ね描き）
/// </summary>
void BackGround::Draw(int screen_w, int screen_h) const
{
    //登録順に描く（奥→手前にしたいなら、奥から順にAddLayerする）
    const int count = static_cast<int>(layer_handles_.size());
    for (int i = 0; i < count; ++i)
    {
        const int handle = layer_handles_[i];
        if (handle < 0)
        {
            continue;
        }

        //600x450 の画像を 1280x720 に拡大して貼る
        DrawExtendGraph(0, 0, screen_w, screen_h, handle, TRUE);
    }
}

/// <summary>
/// 画像の後始末
/// </summary>
void BackGround::Finalize()
{
    const int count = static_cast<int>(layer_handles_.size());
    for (int i = 0; i < count; ++i)
    {
        if (layer_handles_[i] >= 0)
        {
            DeleteGraph(layer_handles_[i]);
            layer_handles_[i] = -1;
        }
    }
    layer_handles_.clear();
}