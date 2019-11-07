#pragma once
#include "ResourceManagers/ResourceManager.h"
#include <string>
#include <fstream>

namespace server
{
class EliminationHeatMap
{
public:
	enum ID
	{
		DefaultMap,
	};

private:
	struct Tile
	{
		int x;
		int y;

		bool operator < (const Tile& rhs) const;
	};

public:
	bool loadFromFile(const ::std::string& filename);

	void addKillAtTile(int col, int row);

private:
	::std::map<Tile, uint32_t> mHeatMap;
	::std::string mFileName;
};

typedef ::mw::ResourceManager<EliminationHeatMap, EliminationHeatMap::ID> HeatMapManager;

}