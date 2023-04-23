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
	bool hasPortal; int portalX; int portalY; Portal* portalPtr;
	trapList traps;
public:
	LevelMap();

	LevelMap(int x, int y);

	~LevelMap();

	void remakeLevel(int x, int y);

	void insertObj(Tile* obj);
	void insertPortal(Portal* obj);
	Tile* getObj(int x, int y);
	std::string echoObj(int x, int y);

	int getXSize() { return map.size(); }
	int getYSize() { return map[0].size(); }

	bool portalExists() { return hasPortal; }
	int getPortalX()	{ return portalX; }
	int getPortalY()	{ return portalY; }
	Portal* getPortalPtr() { return portalPtr; }

	//void setTrapRenderer(SDL_Renderer* rPtr) { traps.setRenderer(rPtr); }
	void addTrap(Trap* trap, int metadata = 0) { traps.addTrap(trap, metadata); }
	void activateTrap(int pX, int pY, int& damage) { traps.activateAt(pX, pY, damage); }
	void clearTraplist() { traps.clear(); }
	void updateTiles() { traps.updateAll(); };

	void renderAll();
};

LevelMap::LevelMap() {
	map.resize(10);
	for (int i = 0; i < map.size(); i++) {
		map[i].resize(10);
	}

	for (int i = 0; i < map.size(); i++) {
		for (int j = 0; j < map[i].size(); j++) {
			map[i][j] = 0;
		}
	}
	hasPortal = false; portalPtr = nullptr; portalX = -1; portalY = -1;
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
	hasPortal = false; portalPtr = nullptr; portalX = -1; portalY = -1;
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

inline void LevelMap::remakeLevel(int x, int y)
{
	//Delete existing level
	for (int i = 0; i < map.size(); i++) {
		for (int j = 0; j < map[i].size(); j++) {
			if (map[i][j] != nullptr) {
				//std::cout << "Deleting " << map[i][j]->echo() << '\n';
				delete map[i][j];
				map[i][j] = nullptr;
			}
		}
	}

	hasPortal = false;
	traps.clear();

	//Resize to new, blank level
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

inline void LevelMap::insertObj(Tile* obj)
{
	if (echoObj(obj->getX(), obj->getY()) != "Empty") {
		delete map[obj->getX()][obj->getY()];
	}
	map[obj->getX()][obj->getY()] = obj;
}

inline void LevelMap::insertPortal(Portal* obj)
{
	insertObj(obj);
	if (hasPortal) {
		delete map[portalX][portalY];
		map[portalX][portalY] = nullptr;
	}
	portalX = obj->getX();
	portalY = obj->getY();
	hasPortal = true;
	portalPtr = obj;
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
