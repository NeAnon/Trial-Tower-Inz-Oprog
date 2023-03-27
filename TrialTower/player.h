#pragma once

//Base includes
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <vector>

//This file will contain different enemy types, which inherit from the base enemy class
#include "entity.h"
#include "enemy.h"

//Entities will have to take into account map layout
#include "levelMap.h"


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

	//Movement handler
	void move(int direction, LevelMap& wallMap, enemyList& list, Portal* endPortal);
};

inline Player::Player() :Entity() {}

inline Player::Player(int x, int y, SDL_Renderer* renderPtr = nullptr) : Entity(x, y, renderPtr) { loadPlayerMedia(); }

inline Player::~Player()
{
}

inline void Player::move(int direction, LevelMap& wallMap, enemyList& list, Portal* endPortal)
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
		if (list.isAt(nextX, nextY)) {
			list.attackSelected();
			actionTaken = true;
		}
		if (!actionTaken) {
			if (nextY >= 0 && wallMap.getObj(nextX, nextY) == nullptr) { setY(nextY); }
			else if (nextY >= 0 && wallMap.echoObj(nextX, nextY) == "Portal") {
				setY(nextY); bool finished = true;
				if (list.enemiesLeft() > 0) {
					finished = false;
				}
				if (finished) {
					endPortal->setFinishState(finished);
				}
			}
		}
		break;
	case DIRECTION_RIGHT:
		nextX++;
		//Check for enemies
		if (list.isAt(nextX, nextY)) {
			list.attackSelected();
			actionTaken = true;
		}
		if (!actionTaken) {
			if (nextX < wallMap.getXSize() && wallMap.getObj(nextX, nextY) == nullptr) { setX(nextX); }
			else if (nextX < wallMap.getXSize() && wallMap.echoObj(nextX, nextY) == "Portal") {
				setX(nextX); bool finished = true;
				if (list.enemiesLeft() > 0) {
					finished = false;
				}
				if (finished) {
					endPortal->setFinishState(finished);
				}
			}
		}
		break;
	case DIRECTION_DOWN:
		nextY++;
		//Check for enemies
		//Check for enemies
		if (list.isAt(nextX, nextY)) {
			list.attackSelected();
			actionTaken = true;
		}
		if (!actionTaken) {
			if (nextY < wallMap.getYSize() && wallMap.getObj(nextX, nextY) == nullptr) { setY(nextY); }
			else if (nextY < wallMap.getYSize() && wallMap.echoObj(nextX, nextY) == "Portal") {
				setY(nextY); bool finished = true;
				if (list.enemiesLeft() > 0) {
					finished = false;
				}
				if (finished) {
					endPortal->setFinishState(finished);
				}
			}
		}
		break;
	case DIRECTION_LEFT:
		nextX--;
		//Check for enemies
		if (list.isAt(nextX, nextY)) {
			list.attackSelected();
			actionTaken = true;
		}
		if (!actionTaken) {
			if (nextX >= 0 && wallMap.getObj(nextX, nextY) == nullptr) { setX(nextX); }
			else if (nextX >= 0 && wallMap.echoObj(nextX, nextY) == "Portal") {
				setX(nextX); bool finished = true;
				if (list.enemiesLeft() > 0) {
					finished = false;
				}
				if (finished) {
					endPortal->setFinishState(finished);
				}
			}
		}
		break;
	default:
		break;
	}
}