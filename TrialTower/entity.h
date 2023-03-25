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
	Entity(int x, int y, SDL_Renderer* renderPtr);

	//Entity destructor
	~Entity();

	//Movement handler
	void move(int direction, LevelMap& wallMap, std::vector< Entity* >& enemyList, Portal* endPortal);

};

inline Entity::Entity() : Object() {

}

inline Entity::Entity(int x, int y, SDL_Renderer* renderPtr = nullptr) : Object(x, y, renderPtr) {

}

inline Entity::~Entity()
{

}

inline void Entity::move(int direction, LevelMap& wallMap, std::vector< Entity* >& enemyList, Portal* endPortal)
{
	//Set clip square to entity's current direction (use case if irregular sprite sizes)
	setClip(direction, 0);

	int nextX = getX();
	int nextY = getY();

	bool actionTaken = false;

	//Depending on direction chosen, if within boundaries and not about to run into a wall, move the entity
	switch (direction) {
	case DIRECTION_UP:
		nextY--;
		//Check for enemies
		for (int i = 0; i < enemyList.size(); i++) {
			if (enemyList[i] != nullptr && nextX == enemyList[i]->getX() && nextY == enemyList[i]->getY()) {
				delete enemyList[i]; enemyList[i] = nullptr;
				actionTaken = true;
			}
		}
		if (!actionTaken) {
			if (nextY >= 0 && wallMap.getObj(nextX, nextY) == nullptr) { setY(nextY); }
			else if (nextY >= 0 && wallMap.echoObj(nextX, nextY) == "Portal") {
				setY(nextY); bool finished = true;
				for(int i = 0; i < enemyList.size(); i++) {
					if (enemyList[i] != nullptr) {
						finished = false;
					}	
				}
				if (!finished) {
					endPortal->renderText(nextX, nextY);
				}
				else {
					endPortal->setFinishState(finished);
				}
			}
		}
		break;
	case DIRECTION_RIGHT:
		nextX++;
		for (int i = 0; i < enemyList.size(); i++) {
			if (enemyList[i] != nullptr && nextX == enemyList[i]->getX() && nextY == enemyList[i]->getY()) {
				delete enemyList[i]; enemyList[i] = nullptr;
				actionTaken = true;
			}
		}
		if (!actionTaken) {
			if (nextX < wallMap.getXSize() && wallMap.getObj(nextX, nextY) == nullptr) { setX(nextX); }
			else if (nextX < wallMap.getXSize() && wallMap.echoObj(nextX, nextY) == "Portal") {
				setX(nextX); bool finished = true;
				for (int i = 0; i < enemyList.size(); i++) {
					if (enemyList[i] != nullptr) {
						finished = false;
					}
				}
				if (!finished) {
					endPortal->renderText(nextX, nextY);
				}
				else {
					endPortal->setFinishState(finished);
				}
			}
		}
		break;
	case DIRECTION_DOWN:
		nextY++;
		//Check for enemies
		for (int i = 0; i < enemyList.size(); i++) {
			if (enemyList[i] != nullptr && nextX == enemyList[i]->getX() && nextY == enemyList[i]->getY()) {
				delete enemyList[i]; enemyList[i] = nullptr;
				actionTaken = true;
			}
		}
		if (!actionTaken) {
			if (nextY < wallMap.getYSize() && wallMap.getObj(nextX, nextY) == nullptr) { setY(nextY); }
			else if (nextY < wallMap.getYSize() && wallMap.echoObj(nextX, nextY) == "Portal") { 
				setY(nextY); bool finished = true;
				for (int i = 0; i < enemyList.size(); i++) {
					if (enemyList[i] != nullptr) {
						finished = false;
					}
				}
				if (!finished) {
					endPortal->renderText(nextX, nextY);
				}
				else {
					endPortal->setFinishState(finished);
				}
			}
		}
		break;
	case DIRECTION_LEFT:
		nextX--;
		for (int i = 0; i < enemyList.size(); i++) {
			if (enemyList[i] != nullptr && nextX == enemyList[i]->getX() && nextY == enemyList[i]->getY()) {
				delete enemyList[i]; enemyList[i] = nullptr;
				actionTaken = true;
			}
		}
		if (!actionTaken) {
			if (nextX >= 0 && wallMap.getObj(nextX, nextY) == nullptr) { setX(nextX); }
			else if (nextX >= 0 && wallMap.echoObj(nextX, nextY) == "Portal") { 
				setX(nextX); bool finished = true;
				for (int i = 0; i < enemyList.size(); i++) {
					if (enemyList[i] != nullptr) {
						finished = false;
					}
				}
				if (!finished) {
					endPortal->renderText(nextX, nextY);
				}
				else {
					endPortal->setFinishState(finished);
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

	Player(int x, int y, SDL_Renderer* renderPtr);

	~Player();

	void loadPlayerMedia() {
		//Load media specifically used by the player
		loadMedia("playerSpriteSheetTransparent.png");
	}

	std::string echo() { return "Player"; }
};

inline Player::Player():Entity(){}

inline Player::Player(int x, int y, SDL_Renderer* renderPtr = nullptr) : Entity(x, y, renderPtr) { loadPlayerMedia(); }

inline Player::~Player()
{
}

class Enemy : public Entity {

public:
	Enemy();

	Enemy(int x, int y, SDL_Renderer* renderPtr);

	~Enemy();

	void loadEnemyMedia() {
		//Load media specifically used by the enemies
		loadMedia("enemySpriteSheetTransparent.png");
	}

	std::string echo() { return "Enemy"; }
};

inline Enemy::Enemy() :Entity() {
	printf("Enemy alive!\n");
}

inline Enemy::Enemy(int x, int y, SDL_Renderer* renderPtr = nullptr) : Entity(x, y, renderPtr) { loadEnemyMedia(); }

inline Enemy::~Enemy()
{
	printf("Enemy dead!\n");
}