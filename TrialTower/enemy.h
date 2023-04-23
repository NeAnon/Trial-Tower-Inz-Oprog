#pragma once

//Base includes
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <random>
#include <chrono>

//This file will contain different enemy types, which inherit from the base enemy class
#include "entity.h"

//Entities will have to take into account map layout
#include "levelMap.h"

enum {
	ENEMY_GENERIC,
	ENEMY_SLIDING
};

std::mt19937 mt{ static_cast<unsigned int>(
		std::chrono::steady_clock::now().time_since_epoch().count()
		) };

class enemyGeneric : public Enemy {
public:
	inline enemyGeneric() : Enemy() {}
	inline enemyGeneric(int x, int y, SDL_Renderer* renderPtr = nullptr) : Enemy(x, y, renderPtr) {}
	inline ~enemyGeneric() {}

	int next_move(LevelMap& wallMap, int playerX, int playerY) { return 0; }
	void move(int direction, int& dmgCounter, int playerX, int playerY) {}
};

class enemySlide : public Enemy {
private:
	int mDirection;
public:
	inline enemySlide(int x, int y, int direction, SDL_Renderer* renderPtr = nullptr) : Enemy(x, y, renderPtr) { mDirection = direction; }
	
	void loadEnemyMedia() {
		//Load media specifically used by the enemies
		loadMedia("enemySliderSheet.png");
	}

	void swapDirection() { mDirection = ((mDirection + 1) % 4) + 1;}

	int next_move(LevelMap& wallMap, int playerX, int playerY) {
		switch (mDirection) {
		case DIRECTION_UP:
			if (getY() == 0 || wallMap.echoObj(getX(), getY() - 1) == "Wall") { swapDirection(); return 0;}
			break;

		case DIRECTION_RIGHT:
			if ((getX()+1) >= wallMap.getXSize() || wallMap.echoObj(getX() + 1, getY()) == "Wall") { swapDirection(); return 0;}
			break;

		case DIRECTION_DOWN:
			if ((getY()+1) >= wallMap.getYSize() || wallMap.echoObj(getX(), getY() + 1) == "Wall") { swapDirection(); return 0;}
			break;

		case DIRECTION_LEFT:
			if (getX() == 0 || wallMap.echoObj(getX() - 1, getY()) == "Wall") { swapDirection(); return 0; }
			break;
		default:
			break;
		}
		return mDirection;
	}
	
	void move(int direction, int& dmgCounter, int playerX, int playerY) {
		//Set clip square to entity's current direction (use case if irregular sprite sizes)
		setClip(direction, 0);

		int nextX = getX();
		int nextY = getY();

		bool actionTaken = false;	int atkVal = 10;

		switch (direction) {
		case DIRECTION_UP:
			nextY--;
			if (nextX == playerX && nextY == playerY) {
				dmgCounter += atkVal;
				actionTaken = true;
			}
			if (!actionTaken)
			{
				setX(nextX); setY(nextY);
			}
			break;

		case DIRECTION_RIGHT:
			nextX++;
			if (nextX == playerX && nextY == playerY) {
				dmgCounter += atkVal;
				actionTaken = true;
			}
			if (!actionTaken)
			{
				setX(nextX); setY(nextY);
			}
			break;

		case DIRECTION_DOWN:
			nextY++;
			if (nextX == playerX && nextY == playerY) {
				dmgCounter += atkVal;
				actionTaken = true;
			}
			if (!actionTaken)
			{
				setX(nextX); setY(nextY);
			}
			break;

		case DIRECTION_LEFT:
			nextX--;
			if (nextX == playerX && nextY == playerY) {
				dmgCounter += atkVal;
				actionTaken = true;
			}
			if (!actionTaken)
			{
				setX(nextX); setY(nextY);
			}
			break;
		default:
			break;
		}
	}
};	

class enemyWander : public Enemy {
	std::vector<int> directions;
public:
	inline enemyWander(int x, int y, SDL_Renderer* renderPtr = nullptr) : Enemy(x, y, renderPtr) { directions.reserve(4); }

	void loadEnemyMedia() {
		//Load media specifically used by the enemies
		loadMedia("enemyWanderSheet.png");
	}

	int next_move(LevelMap& wallMap, int playerX, int playerY) {
		directions.clear();

		if (getY() - 1 >= 0 && wallMap.echoObj(getX(), getY() - 1) != "Wall") { directions.push_back(DIRECTION_UP); }
		if (getX() + 1 < wallMap.getXSize() && wallMap.echoObj(getX() + 1, getY()) != "Wall") { directions.push_back(DIRECTION_RIGHT); }
		if (getY() + 1 < wallMap.getYSize() && wallMap.echoObj(getX(), getY() + 1) != "Wall") { directions.push_back(DIRECTION_DOWN); }		
		if (getX() - 1 >= 0 && wallMap.echoObj(getX() - 1, getY()) != "Wall") { directions.push_back(DIRECTION_LEFT); }
		
		if (directions.empty()) {
			return 0;
		}
		else {
			int direction = mt() % directions.size();
			return directions[direction];
		}
	}

	void move(int direction, int& dmgCounter, int playerX, int playerY) {
		//Set clip square to entity's current direction (use case if irregular sprite sizes)
		setClip(direction, 0);

		int nextX = getX();
		int nextY = getY();

		bool actionTaken = false;	int atkVal = 10;

		switch (direction) {
		case DIRECTION_UP:
			nextY--;
			if (nextX == playerX && nextY == playerY) {
				dmgCounter += atkVal;
				actionTaken = true;
			}
			if(!actionTaken)
			{
				setX(nextX); setY(nextY);
			}
			break;

		case DIRECTION_RIGHT:
			nextX++;
			if (nextX == playerX && nextY == playerY) {
				dmgCounter += atkVal;
				actionTaken = true;
			}
			if (!actionTaken)
			{
				setX(nextX); setY(nextY);
			}
			break;

		case DIRECTION_DOWN:
			nextY++;
			if (nextX == playerX && nextY == playerY) {
				dmgCounter += atkVal;
				actionTaken = true;
			}
			if (!actionTaken)
			{
				setX(nextX); setY(nextY);
			}
			break;

		case DIRECTION_LEFT:
			nextX--;
			if (nextX == playerX && nextY == playerY) {
				dmgCounter += atkVal;
				actionTaken = true;
			}
			if (!actionTaken)
			{
				setX(nextX); setY(nextY);
			}
			break;
		default:
			break;
		}
	}
};

class enemyList {
private:
	std::vector<Enemy*> list;
	SDL_Renderer* global_renderer;
	int enemySelected;
public:
	enemyList();

	~enemyList();

	void setRenderer(SDL_Renderer* renderPtr) { global_renderer = renderPtr; };

	void addEnemy(int x, int y, std::string type, int metadata);

	bool isAt(int x, int y);

	void attackSelected();

	int enemiesLeft();

	void reloadMedia();

	void renderAll();

	void clear();

	void moveAll(LevelMap& map, int& dmgCounter, int playerX, int playerY);
};

//If you're putting over 1000 enemies into a single level, you have a problem
enemyList::enemyList() { list.reserve(1000); global_renderer = NULL; enemySelected = -1; }

inline enemyList::~enemyList()
{
	for (int i = 0; i < list.size(); i++) {	
		if (list[i] != nullptr) {
			std::cout << "Deleting from " << i << '\n';
			delete list[i];
			list[i] = nullptr;
		}
	}
}

inline void enemyList::addEnemy(int x, int y, std::string type = "Generic", int metadata = 0)
{
	Enemy* newEnemy;
	if (type == "Generic") {
		newEnemy = new enemyGeneric(x, y, global_renderer);
	}
	if (type == "Slide") {
		newEnemy = new enemySlide(x, y, metadata, global_renderer);
	}
	if (type == "Wander") {
		newEnemy = new enemyWander(x, y, global_renderer);
	}
	list.push_back(newEnemy);
}

inline bool enemyList::isAt(int x, int y) {
	{
		for (int i = 0; i < list.size(); i++) {
			if (list[i] != nullptr) {
				if (x == list[i]->getX() && y == list[i]->getY()) {
					enemySelected = i;
					//std::cout << "Enemy at (" << x << "," << y << ")\n";
					return true;
				}
			}
		}
		// std::cout << "No enemy at (" << x << "," << y << ")\n";
		return false;
	}
}

inline void enemyList::attackSelected() {
	if (enemySelected == -1) { return; }
	if (list[enemySelected] != nullptr) {
		delete list[enemySelected];
		list[enemySelected] = nullptr;
	}
}

inline int enemyList::enemiesLeft() {
	int counter = 0;
	for (int i = 0; i < list.size(); i++) {
		if (list[i] != nullptr) { counter++; }
	}
	//std::cout << counter << " enemies left\n";
	return counter;
}

inline void enemyList::reloadMedia() {
	for (int i = 0; i < list.size(); i++) {
		if (list[i] != nullptr) {
			list[i]->loadEnemyMedia();
		}
	}
}

inline void enemyList::renderAll() {
	for (int i = 0; i < list.size(); i++) {
		if (list[i] != nullptr) {
			list[i]->render();
		}
	}
}

inline void enemyList::clear()
{
	for (int i = 0; i < list.size(); i++) {
		if (list[i] != nullptr) {
			std::cout << "Deleting from " << i << '\n';
			delete list[i];
			list[i] = nullptr;
		}
	}
	list.clear();
	list.shrink_to_fit();
	list.reserve(1000);
	std::cout << "Enemy list size: " << list.size() << "\n";
}

inline void enemyList::moveAll(LevelMap& map, int& dmgCounter, int playerX, int playerY) {
	int eDmgAcc = 0;
	for (int i = 0; i < list.size(); i++) {
		eDmgAcc = 0;
		if (list[i] != nullptr) {
			list[i]->move(list[i]->next_move(map, playerX, playerY), dmgCounter, playerX, playerY);
			map.activateTrap(list[i]->getX(), list[i]->getY(), eDmgAcc);
			list[i]->damage(eDmgAcc);
			if (list[i]->getHP() <= 0) {
				delete list[i];
				list[i] = nullptr;
			}
		}
	}
}