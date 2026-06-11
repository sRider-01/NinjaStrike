#include "EnemySpawnMap.h"
#include <fstream>
#include <sstream>
#include <string>

/// <summary>
/// EnemySpawnMap.cppで使う定数
/// </summary>
namespace
{
	/// <summary>
	/// ID:何も配置しない
	/// </summary>
	const int kEnemySpawnNone = 0;

	/// <summary>
	/// ID：パトロールエネミー
	/// </summary>
	const int kEnemySpawnPatrol = 1;

	/// <summary>
	/// ID:遠距離攻撃エネミー
	/// </summary>
	const int kEnemySpawnRanged = 2;
	
	/// <summary>
	/// ID:ボスエネミー
	/// </summary>
	const int kEnemySpawnBoss = 3;
}

/// <summary>
/// コンストラクタ
/// </summary>
EnemySpawnMap::EnemySpawnMap()
	: width_(0)
	, height_(0)
{
}

/// <summary>
/// CSVを読みこむ
/// </summary>
bool EnemySpawnMap::LoadFromCSV(const char* file_path)
{
	std::ifstream ifstream_instance(file_path);
	if (!ifstream_instance)
	{
		return false;
	}

	spawn_ids_.clear();
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
		std::vector<int> row;

		while (std::getline(string_stream, token, ','))
		{
			if (!token.empty() && token.back() == '\r')
			{
				token.pop_back();
			}

			int csv_id = -1;
			if (!token.empty())
			{
				csv_id = std::stoi(token);
			}

			const int spawn_id = ConvertCsvIdToSpawnId(csv_id);
			row.push_back(spawn_id);
		}

		if (row.empty())
		{
			continue;
		}

		if (width_ < static_cast<int>(row.size()))
		{
			width_ = static_cast<int>(row.size());
		}

		spawn_ids_.push_back(row);
	}

	height_ = static_cast<int>(spawn_ids_.size());

	for (int y = 0; y < height_; ++y)
	{
		while (static_cast<int>(spawn_ids_[y].size()) < width_)
		{
			spawn_ids_[y].push_back(kEnemySpawnNone);
		}
	}

	return true;
}

/// <summary>
/// スポーンIDを読む
/// </summary>
int EnemySpawnMap::GetSpawnId(int x, int y) const
{
	if (x < 0 || y < 0 || x >= width_ || y >= height_)
	{
		return kEnemySpawnNone;
	}

	return spawn_ids_[y][x];
}

/// <summary>
/// Tiled のCSV値をゲーム側の敵IDに変換
/// </summary>
int EnemySpawnMap::ConvertCsvIdToSpawnId(int csv_id) const
{

	switch (csv_id)
	{
	case -1:// CSV -1 → None
		return kEnemySpawnNone;

	case 0:// CSV 0 → Patrol
		return kEnemySpawnPatrol;

	case 1:// CSV 1 → Ranged
		return kEnemySpawnRanged;

	case 2:// CSV 2 → Boss
		return kEnemySpawnBoss;

	default:
		return kEnemySpawnNone;
	}
}