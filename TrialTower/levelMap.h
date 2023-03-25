#pragma once

#include <vector>

#include "tile.h"

enum {
	MAP_EMPTY, //0
	MAP_WALL,  //1
	MAP_ENTITY,//2
	MAP_ENEMY, //3
};

class LevelMap {
private:
	std::vector< std::vector< Tile* > > map;
public:
	LevelMap();

	LevelMap(int x, int y);

	~LevelMap();

	void insertObj(Tile* obj);
	Tile* getObj(int x, int y);
	std::string echoObj(int x, int y);

	int getXSize() { return map.size(); }
	int getYSize() { return map[0].size(); }

	void renderAll();
};

LevelMap::LevelMap() {
	map.resize(16);
	for (int i = 0; i < map.size(); i++) {
		map[i].resize(16);
	}

	for (int i = 0; i < map.size(); i++) {
		for (int j = 0; j < map[i].size(); j++) {
			map[i][j] = 0;
		}
	}
}

inline LevelMap::LevelMap(int x, int y)
{
	map.resize(x);
	for (int i = 0; i < map.size(); i++) {
		map[i].resize(y);
	}

	for (int i = 0; i < map.size(); i++) {
		for (int j = 0; j < map[i].size(); j++) {
			map[i][j] = nullptr;
		}
	}
}

inline LevelMap::~LevelMap()
{
	for (int i = 0; i < map.size(); i++) {
		for (int j = 0; j < map[i].size(); j++) {
			if (map[i][j] != nullptr) {
				//std::cout << "Deleting " << map[i][j]->echo() << '\n';
				delete map[i][j];
				map[i][j] = nullptr;
			}
		}
	}
}

inline void LevelMap::insertObj(Tile* obj)
{
	map[obj->getX()][obj->getY()] = obj;
}

inline Tile* LevelMap::getObj(int x, int y)
{
	return map[x][y];
}

inline std::string LevelMap::echoObj(int x, int y)
{
	if (getObj(x, y) == nullptr) {
		return "Empty";
	}
	else {
		return getObj(x,y)->echo();
	}
}
inline void LevelMap::renderAll()
{
	for (int i = 0; i < map.size(); i++) {
		for (int j = 0; j < map[i].size(); j++) {
			if (map[i][j] != nullptr) {
				map[i][j]->render();
			}
		}
	}
}
