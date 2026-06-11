#pragma once
#include <vector>

/// <summary>
/// 敵のスポーンマップについてのクラス
/// </summary>
class EnemySpawnMap
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	EnemySpawnMap();

	/// <summary>
	/// CSVを読みこむ
	/// </summary>
	/// <param name="file_path">CSVファイルのパス</param>
	/// <returns>成功／失敗</returns>
	bool LoadFromCSV(const char* file_path);

	/// <summary>
	/// スポーンIDを読む
	/// </summary>
	/// <param name="x">X座標</param>
	/// <param name="y">Y座標</param>
	/// <returns>読んだ値</returns>
	int GetSpawnId(int x, int y) const;

	/// <summary>
	/// リストの幅の取得
	/// </summary>
	/// <returns>リストの幅</returns>
	int GetWidth() const
	{
		return width_;
	}

	/// <summary>
	/// リストの高さを取得
	/// </summary>
	/// <returns>リストの高さ</returns>
	int GetHeight() const
	{
		return height_;
	}

	/// <summary>
	/// CSVのIDをスポーンIDに変換
	/// </summary>
	/// <param name="csv_id">CSVのID</param>
	/// <returns>スポーンID</returns>
	int ConvertCsvIdToSpawnId(int csv_id) const;

private:
	/// <summary>
	/// スポーンIDの配列
	/// </summary>
	std::vector<std::vector<int>> spawn_ids_{};
	
	/// <summary>
	/// リストの幅
	/// </summary>
	int width_ = 0;

	/// <summary>
	/// リストの高さ
	/// </summary>
	int height_ = 0;
};