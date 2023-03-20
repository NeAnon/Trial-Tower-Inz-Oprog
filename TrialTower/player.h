#pragma once

//Base includes
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <vector>

//Wrapped texture class 
#include "WTexture.h"

//Entites are objects, so
#include "object.h"


//Directionals corresponding to images on the sprite sheet
enum {
	DIRECTION_CENTER,	//0
	DIRECTION_UP,		//1
	DIRECTION_RIGHT,	//2
	DIRECTION_DOWN,		//3
	DIRECTION_LEFT,		//4
};

class Entity : public Object {
private:

public:
	//Entity contructor
	Entity();

	//Entity contructor at pos
	Entity(int x, int y);

	//Entity destructor
	~Entity();

	//Movement handler
	void move(int direction, std::vector<std::vector<int> > &wallMap, std::vector< Entity* > &enemyList);
};

inline Entity::Entity() : Object() {

}

inline Entity::Entity(int x, int y) : Object(x, y) {

}

inline Entity::~Entity()
{

}

inline void Entity::move(int direction, std::vector<std::vector<int> > &wallMap, std::vector< Entity* > &enemyList)
{
	//Set clip square to entity's current direction (use case if irregular sprite sizes)
	setClip(direction, 0);

	int currX = getX();
	int currY = getY();

	//Depending on direction chosen, if within boundaries and not about to run into a wall, move the entity
	switch (direction) {
	case DIRECTION_UP:
		if ((currY - 1 >= 0) && wallMap[currX][currY - 1] == 0) { setY(--currY); }
		else{
			for (int i = 0; i < enemyList.size(); i++) {
				if (enemyList[i] != nullptr && currX == enemyList[i]->getX() && currY - 1 == enemyList[i]->getY()) {
					wallMap[enemyList[i]->getX()][enemyList[i]->getY()] = 0;
					delete enemyList[i]; enemyList[i] = nullptr;
				}
			}
		}
		break;
	case DIRECTION_RIGHT:
		if ((currX + 1 < wallMap.size()) && wallMap[currX + 1][currY] == 0) { setX(++currX); }
		else{
			for (int i = 0; i < enemyList.size(); i++) {
				if (enemyList[i] != nullptr && currX + 1 == enemyList[i]->getX() && currY == enemyList[i]->getY()) {
					wallMap[enemyList[i]->getX()][enemyList[i]->getY()] = 0;
					delete enemyList[i]; enemyList[i] = nullptr;
				}
			}
		}
		break;
	case DIRECTION_DOWN:
		if ((currY + 1 < wallMap[currX].size()) && wallMap[currX][currY + 1] == 0) { setY(++currY); }
		else {
			for (int i = 0; i < enemyList.size(); i++) {
				if (enemyList[i] != nullptr && currX == enemyList[i]->getX() && currY + 1 == enemyList[i]->getY()) {
					wallMap[enemyList[i]->getX()][enemyList[i]->getY()] = 0;
					delete enemyList[i]; enemyList[i] = nullptr;
				}
			}
		}
		break;
	case DIRECTION_LEFT:
		if ((currX - 1 >= 0) && wallMap[currX - 1][currY] == 0) { setX(--currX); }
		else {
			for (int i = 0; i < enemyList.size(); i++) {
				if (enemyList[i] != nullptr && currX - 1 == enemyList[i]->getX() && currY == enemyList[i]->getY()) {
					wallMap[enemyList[i]->getX()][enemyList[i]->getY()] = 0;
					delete enemyList[i]; enemyList[i] = nullptr;
				}
			}
		}
		break;
	default:
		break;
	}
}

class Player : public Entity {

public:
	Player();

	Player(int x, int y);

	~Player();

	void loadPlayerMedia() {
		//Load media specifically used by the player
		loadMedia("playerSpriteSheetTransparent.png");
	}

	std::string echo() { return "Player"; }
};

inline Player::Player():Entity(){}

inline Player::Player(int x, int y):Entity(x, y){}

inline Player::~Player()
{
}

class Enemy : public Entity {

public:
	Enemy();

	Enemy(int x, int y);

	~Enemy();

	void loadEnemyMedia() {
		//Load media specifically used by the enemies
		loadMedia("wall_light.png");
	}

	std::string echo() { return "Enemy"; }
};

inline Enemy::Enemy() :Entity() {
	printf("Enemy alive!\n");
}

inline Enemy::Enemy(int x, int y) : Entity(x, y) {}

inline Enemy::~Enemy()
{
	printf("Enemy dead!\n");
}