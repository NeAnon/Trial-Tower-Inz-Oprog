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

#include "item.h"

//Entities will have to take into account map layout
#include "levelMap.h"


class Player : public Entity {
	int hitPts; 
	int maxHP;
	int damageDealt;
	int gold;
	int hasPotion;
	Potion potion;
public:
	Player();

	Player(int x, int y, SDL_Renderer* renderPtr);

	~Player();

	void loadPlayerMedia() {
		//Load media specifically used by the player
		loadMedia("playerSpriteSheetTransparent.png");
	}

	std::string echo() { return "Player"; }

	void hurt(int damage) { hitPts -= damage; if (hitPts <= 0 && hasPotion) { hitPts = maxHP; hasPotion = false; std::cout << "Quaffed potion!\n";} }
	bool isAlive() { return hitPts > 0; }
	int getHP() { return hitPts; }
	int getMaxHP() { return maxHP; }
	void addMoney(int money) { gold += money; }
	int getMoney() { return gold; }
	bool hasPot() { return hasPotion; }
	
	//Movement handler
	void move(int direction, LevelMap& wallMap, enemyList& list, Portal* endPortal);
};

inline Player::Player() : Entity() { hitPts = 100; maxHP = 100; damageDealt = 10; gold = 0; }

inline Player::Player(int x, int y, SDL_Renderer* renderPtr = nullptr) : Entity(x, y, renderPtr) { loadPlayerMedia();  hitPts = 100; maxHP = 100; damageDealt = 10; gold = 0; }

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
	if (collMoney == -50) {
		if (hasPotion) { collMoney += 50; }
		else {
			if (gold >= 50) { hasPotion = true; std::cout << "Acquired potion!\n"; }
			else { collMoney = 0; }
		}
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
	WTexture playerInventory;
	WTexture pauseOverlay;
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
		if (!playerInventory.loadFromFile("resources/Player_Inventory.png"))
		{
			printf("Failed to load inventory texture! SDL_image Error: %s\n", IMG_GetError());
		}
		if (!pauseOverlay.loadFromFile("resources/pauseOverlay.png"))
		{
			printf("Failed to load pause texture! SDL_image Error: %s\n", IMG_GetError());
		}
	}

	inline void render(Player& player, bool paused)
	{
		//interfaceBG.render(0, WTexture::getGlobalLHeight());
		healthBarState.w = 32 + player.getHP() * 96 / player.getMaxHP();
		int money = player.getMoney();
		//Render the entity at given grid coords (may need to change the numbers to adjust for level grid)
		healthTextureShadow.render(0, WTexture::getGlobalLHeight(), &healthBarMissing);
		healthTexture.render(0, WTexture::getGlobalLHeight(), &healthBarState);
		playerInventory.render(160, WTexture::getGlobalLHeight());
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

		if (player.hasPot()) { healthBarMissing.w = 160; }
		else { healthBarMissing.w = 128; }

		if (paused) {
			pauseOverlay.render(0, 0);
			int HP = player.getHP();
			if (HP == 0) {
				digitTexture.render(64, WTexture::getGlobalLHeight(), &digits[0]);
			}
			else {
				int HPOffset = 80 + std::trunc(std::log10(std::abs(HP))-1)*8;
				while (HP > 0) { 
					digitTexture.render(HPOffset, WTexture::getGlobalLHeight(), &digits[HP % 10]);
					HP /= 10; HPOffset -= 16;
				}
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
	playerInventory.setRenderer(renderPtr);
	pauseOverlay.setRenderer(renderPtr);
}
