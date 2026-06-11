#pragma once
#include <vector>

/// <summary>
/// 背景画像についてのクラス
/// </summary>
class BackGround
{
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    BackGround();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~BackGround();

public:
    /// <summary>
    /// 初期化処理
    /// </summary>
    void Initialize();

    /// <summary>
    /// 背景レイヤー画像を追加してロードする
    /// </summary>
    /// <param name="path">画像パス</param>
    /// <returns>成功/失敗</returns>
    bool AddLayer(const char* path);

    /// <summary>
    /// 背景を描画する（画面サイズに拡大して、登録順に重ね描き）
    /// </summary>
    /// <param name="screen_w">画面幅</param>
    /// <param name="screen_h">画面高さ</param>
    void Draw(int screen_w, int screen_h) const;

    /// <summary>
    /// 画像の後始末
    /// </summary>
    void Finalize();

private:
    /// <summary>
    /// レイヤーのハンドル
    /// </summary>
    std::vector<int> layer_handles_;
};