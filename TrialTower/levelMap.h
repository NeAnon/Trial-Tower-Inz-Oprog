#pragma once

#include <vector>

#include "object.h"

enum {
	MAP_EMPTY, //0
	MAP_WALL,  //1
	MAP_ENTITY,//2
	MAP_ENEMY, //3
};

class LevelMap {
private:
	std::vector< std::vector< Object* > > map;
public:
	LevelMap();

	LevelMap(int x, int y);

	~LevelMap();

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
			map[i][j] = 0;
		}
	}
}

inline LevelMap::~LevelMap()
{
	for (int i = 0; i < map.size(); i++) {
		for (int j = 0; j < map[i].size(); j++) {
			if (map[i][j] != nullptr) {
				delete map[i][j];
				map[i][j] = nullptr;
			}
		}
	}
}
