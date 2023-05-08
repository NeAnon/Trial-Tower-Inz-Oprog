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
	int hitPts; 
	int damageDealt;
	int gold;
public:
	Player();

	Player(int x, int y, SDL_Renderer* renderPtr);

	~Player();

	void loadPlayerMedia() {
		//Load media specifically used by the player
		loadMedia("playerSpriteSheetTransparent.png");
	}

	std::string echo() { return "Player"; }

	void hurt(int damage) { hitPts -= damage; }
	bool isAlive() { return hitPts > 0; }
	int getHP() { return hitPts; }
	void addMoney(int money) { gold += money; }
	int getMoney() { return gold; }
	
	//Movement handler
	void move(int direction, LevelMap& wallMap, enemyList& list, Portal* endPortal);
};

inline Player::Player() :Entity() { hitPts = 100; damageDealt = 10; gold = 0; }

inline Player::Player(int x, int y, SDL_Renderer* renderPtr = nullptr) : Entity(x, y, renderPtr) { loadPlayerMedia();  hitPts = 100; damageDealt = 10; gold = 0; }

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
	int accDamage = 0;
	int collMoney = 0;

	//Depending on direction chosen, if within boundaries and not about to run into a wall, move the entity
	switch (direction) {
	case DIRECTION_UP:
		nextY--;
		//Check for enemies
		if (list.isAt(nextX, nextY)) {
			list.attackSelected(collMoney);
			actionTaken = true;
		}
		if (!actionTaken) {
			if (nextY >= 0 && wallMap.echoObj(nextX, nextY) == "Floor") { setY(nextY); }
			else if (nextY >= 0 && wallMap.echoObj(nextX, nextY) == "SpikeTrap") { setY(nextY); }
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
			list.attackSelected(collMoney);
			actionTaken = true;
		}
		if (!actionTaken) {
			if (nextX < wallMap.getXSize() && wallMap.echoObj(nextX, nextY) == "Floor") { setX(nextX); }
			else if (nextX < wallMap.getXSize() && wallMap.echoObj(nextX, nextY) == "SpikeTrap") { setX(nextX); }
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
			list.attackSelected(collMoney);
			actionTaken = true;
		}
		if (!actionTaken) {
			if (nextY < wallMap.getYSize() && wallMap.echoObj(nextX, nextY) == "Floor") { setY(nextY); }
			else if (nextY < wallMap.getYSize() && wallMap.echoObj(nextX, nextY) == "SpikeTrap") { setY(nextY); }
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
			list.attackSelected(collMoney);
			actionTaken = true;
		}
		if (!actionTaken) {
			if (nextX >= 0 && wallMap.echoObj(nextX, nextY) == "Floor") { setX(nextX); }
			else if (nextX >= 0 && wallMap.echoObj(nextX, nextY) == "SpikeTrap") { setX(nextX); }
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
	hurt(accDamage); addMoney(collMoney);
	if (actionTaken) { std::cout << "Dealt " << damageDealt << " damage!\n"; }
	//std::cout << "Next tile: " << wallMap.echoObj(nextX, nextY) << "\n";
}

class Interface {
	//Player texture and current sprite
	WTexture healthTexture;
	WTexture healthTextureShadow;
	WTexture coinTexture;
	WTexture digitTexture;
	SDL_Rect healthBarState;
	SDL_Rect healthBarMissing;
	SDL_Rect coinState;
	std::vector<SDL_Rect> digits;


public:
	Interface();
	Interface(SDL_Renderer * renderPtr);

	void loadInterface() {
		//Load entity texture	
		if (!healthTexture.loadFromFile("resources/healthbar.png"))
		{
			printf("Failed to load health bar texture! SDL_image Error: %s\n", IMG_GetError());
		}
		if (!healthTextureShadow.loadFromFile("resources/healthbarMissing.png"))
		{
			printf("Failed to load health bar texture! SDL_image Error: %s\n", IMG_GetError());
		}
		if (!coinTexture.loadFromFile("resources/coin.png"))
		{
			printf("Failed to load coin interface texture! SDL_image Error: %s\n", IMG_GetError());
		}
		if (!digitTexture.loadFromFile("resources/digits.png"))
		{
			printf("Failed to load number texture! SDL_image Error: %s\n", IMG_GetError());
		}
	}

	inline void render(int health, int maxHealth, int money)
	{
		healthBarState.w = 32 + health * 96 / maxHealth;

		//Render the entity at given grid coords (may need to change the numbers to adjust for level grid)
		healthTextureShadow.render(0, WTexture::getGlobalLHeight(), &healthBarMissing);
		healthTexture.render(0, WTexture::getGlobalLHeight(), &healthBarState);
		coinTexture.render(WTexture::getGlobalLWidth() - 32, WTexture::getGlobalLHeight());
		int coinOffset = 0;
		if (money == 0) {
			digitTexture.render(WTexture::getGlobalLWidth() - ((16 * coinOffset++) + 48), WTexture::getGlobalLHeight(), &digits[0]);
		}
		else {
			while (money > 0) {
				digitTexture.render(WTexture::getGlobalLWidth() - ((16 * coinOffset++) + 48), WTexture::getGlobalLHeight(), &digits[money%10]);
				money /= 10;
			}
		}


	}
};

Interface::Interface() {
	//Set dimensions of object's sprites
	healthBarState = { 0, 0, 128, 32 };
	healthBarMissing = { 0, 0, 128, 32 };
	coinState = { 0, 0, 32, 32 };
	digits.resize(10);
	for (int i = 0; i < 10; i++) {
		digits[i].x = i * 16;
		digits[i].y = 0;
		digits[i].w = 16;
		digits[i].h = 32;
	}
}

inline Interface::Interface(SDL_Renderer* renderPtr)
{
	healthBarState = { 0, 0, 128, 32 };
	healthBarMissing = { 0, 0, 128, 32 };
	coinState = { 0, 0, 32, 32 };
	digits.resize(10);
	for (int i = 0; i < 10; i++) {
		digits[i].x = i * 16;
		digits[i].y = 0;
		digits[i].w = 16;
		digits[i].h = 32;
	}

	healthTexture.setRenderer(renderPtr);
	healthTextureShadow.setRenderer(renderPtr);
	coinTexture.setRenderer(renderPtr);
	digitTexture.setRenderer(renderPtr);
}
