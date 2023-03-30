#pragma once

//Base includes
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <vector>

//Wrapped texture class 
#include "WTexture.h"

//Entites inherit from objects
#include "object.h"

//Entities will have to take into account map layout
#include "levelMap.h"

class internalPosList {
private:
	struct pos { int x; int y; };
	std::vector<pos> posList;
public:

};

//Directionals corresponding to images on the sprite sheet
enum {
	DIRECTION_CENTER,		//0
	DIRECTION_UP,			//1
	DIRECTION_RIGHT,		//2
	DIRECTION_DOWN,			//3
	DIRECTION_LEFT,			//4
	DIRECTION_UP_RIGHT,		//5
	DIRECTION_DOWN_RIGHT,	//6
	DIRECTION_DOWN_LEFT,	//7
	DIRECTION_UP_LEFT		//8
};

class Entity : public Object {
private:

public:
	//Entity contructor
	Entity();

	//Entity contructor at pos
	Entity(int x, int y, SDL_Renderer* renderPtr);

	//Entity destructor
	~Entity();


};

inline Entity::Entity() : Object() {

}

inline Entity::Entity(int x, int y, SDL_Renderer* renderPtr = nullptr) : Object(x, y, renderPtr) {

}

inline Entity::~Entity()
{

}

class Enemy : public Entity {

public:
	Enemy();

	Enemy(int x, int y, SDL_Renderer* renderPtr);

	~Enemy();

	virtual void loadEnemyMedia() {
		//Load media specifically used by the enemies
		loadMedia("enemySpriteSheetTransparent.png");
	}

	std::string echo() { return "Enemy"; }

	virtual int next_move(LevelMap& wallMap, int playerX, int playerY) = 0;//Direction of next move (function of movement logic)
	virtual void move(int direction, int& dmgCounter, int playerX, int playerY) = 0;
};

inline Enemy::Enemy() : Entity() {
	printf("Enemy alive!\n");
}

inline Enemy::Enemy(int x, int y, SDL_Renderer* renderPtr = nullptr) : Entity(x, y, renderPtr) { loadEnemyMedia(); }

inline Enemy::~Enemy()
{
	printf("Enemy dead!\n");
}