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
	// Weapon		weapon;
	// Shield		shield;
	// Helmet		helmet;
	// Chestplate	chestplate;
	// Leggings		leggings;
	Boots		boots;
	Potion		potion;
	// Ring			ringLeft;
	// Ring			ringRight;
	//
	bool moveTechActive;

	std::vector<int> activeEffects;
	std::vector<int> effectPotencies;

public:
	Player();

	Player(int x, int y, SDL_Renderer* renderPtr);

	~Player();

	void loadPlayerMedia() {
		//Load media specifically used by the player
		loadMedia("playerSpriteSheetTransparent.png");
	}

	std::string echo() { return "Player"; }

	void hurt(int damage) { hitPts -=	(damage - (effectPotencies[GLYPH_PDEF]+effectPotencies[GLYPH_DEF]) > 0 ? 
										 damage - (effectPotencies[GLYPH_PDEF]+effectPotencies[GLYPH_DEF]) : 0);
							
							if (hitPts <= 0 && potion.isEquipped()) { 
								hitPts = ((potion.echo_potency() < maxHP) ? potion.echo_potency() : maxHP); 
								std::cout << "Quaffed potion!\n"; 
								potion.set_equipped(false);
							} 
	}
	bool isAlive() { return hitPts > 0; }
	int getHP() { return hitPts; }
	int getMaxHP() { return maxHP; }
	void addMoney(int money) { gold += money; }
	int getMoney() { return gold; }
	bool hasItem(int i){
		switch (i) {
		case 6:
			return boots.isEquipped();
		case 7:
			return potion.isEquipped();
		default:
			return false;
		}
	}
	Item* getItem(int i) {
		switch (i) {
		case 6:
			return &boots;
		case 7:
			return &potion;
		default:
			return nullptr;
		}
	}
	
	void recalc_effects() {
		for (int i = 0; i < effectPotencies.size(); i++) {
			effectPotencies[i] = 0;
		}
		if (boots.isEquipped()) {
			//std::cout << "Effect modified: " << boots.echo_effect() << " of vector with size " << effectPotencies.size() << "\n";
			effectPotencies[boots.echo_effect()] += boots.echo_potency();
		}
	}

	void replaceItem(Item*& destItem, Item& templateItem) {
		destItem->set_cost(templateItem.echo_cost());
		destItem->set_effect(templateItem.echo_effect());
		destItem->set_equipped(templateItem.isEquipped());
		destItem->set_potency(templateItem.echo_potency());
	}

	//Movement handler
	void move(int direction, LevelMap& wallMap, enemyList& list, InventoryList& invList, Portal* endPortal);
};

inline Player::Player() : Entity() { 
	hitPts = 100; maxHP = 100; damageDealt = 10; gold = 0; 
	effectPotencies.resize(EFFECT_COUNT); recalc_effects(); moveTechActive = false;
}

inline Player::Player(int x, int y, SDL_Renderer* renderPtr = nullptr) : Entity(x, y, renderPtr) { 
	loadPlayerMedia();  
	hitPts = 100; maxHP = 100; damageDealt = 10; gold = 0;
	effectPotencies.resize(EFFECT_COUNT); recalc_effects(); moveTechActive = false;
}

inline Player::~Player()
{
}

inline void Player::move(int direction, LevelMap& wallMap, enemyList& list, InventoryList& invlist, Portal* endPortal)
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
		break;
	case DIRECTION_RIGHT:
		nextX++;
		break;
	case DIRECTION_DOWN:
		nextY++;
		break;
	case DIRECTION_LEFT:
		nextX--;
		break;
	default:
		break;
	}

	//Check for enemies
	if (list.isAt(nextX, nextY)) {
		list.attackSelected(collMoney);
		actionTaken = true;
		std::cout << "Dealt " << damageDealt << " damage!\n";
	}
	
	Inventory tempInv; Item* tempItem = nullptr; int currItem = 0;
	Inventory* sourceInv = invlist.isInvAt(nextX, nextY);
	
	if (sourceInv) {
		//Take all items you can from the inventory
		sourceInv->giveItems(tempInv, gold);

		if (tempInv.empty()) { actionTaken = true; }
		//If no items were taken, pass, otherwise...
		else {
			//tempInv.callAllItems();
			for (int i = 0; i < tempInv.size(); i++) {
				//For every item, check if it can be equipped.
				//If it can copy its stats and set a new item in the slot.
				//If the slot is occupied, save the stats and put the item back into the ptr we are pulling from.
				tempItem = tempInv.itemAt(i);
				currItem = tempItem->echo_type();

				switch (currItem) {
				case TYPE_BOOT:
					if (boots.isEquipped()) {
						sourceInv->addItem(new Boots(0, boots.echo_effect(), boots.echo_potency(), false));
						boots = Boots(0, tempItem->echo_effect(), tempItem->echo_potency(), true);
						tempItem = nullptr;
						tempInv.removeItem(i);
					}
					else {
						//Set potion params
						boots = Boots(0,tempItem->echo_effect(), tempItem->echo_potency(), true);
						tempItem = nullptr;
						tempInv.removeItem(i);
					}
					recalc_effects();
					break;
				case TYPE_POTN:
					if (potion.isEquipped()) {
						sourceInv->addItem(new Potion(0, potion.echo_potency(), false));
						potion = Potion(0, tempItem->echo_potency(), true);
						tempItem = nullptr;
						tempInv.removeItem(i);
					}
					else {
						//Set potion params
						potion = Potion(0, tempItem->echo_potency(), true);
						tempItem = nullptr;
						tempInv.removeItem(i);
					}
					break;
				default:
					break;
				}
			}
			if (sourceInv->empty()) { invlist.removeAt(nextX, nextY); }
		}
	}
	//Check if player can move
	if (!actionTaken) {
		if (nextX >= 0 && nextY >= 0 && nextX < wallMap.getXSize() && nextY < wallMap.getYSize()) 
		{
			if (wallMap.echoObj(nextX, nextY) == "Floor") { setX(nextX); setY(nextY); }
			else if (wallMap.echoObj(nextX, nextY) == "Wall" && effectPotencies[GLYPH_PHASE] > 0) { setX(nextX); setY(nextY); }
			else if (wallMap.echoObj(nextX, nextY) == "SpikeTrap") { setX(nextX); setY(nextY); }
			else if (wallMap.echoObj(nextX, nextY) == "Portal") {
				setX(nextX); setY(nextY); bool finished = true;
				if (list.enemiesLeft() > 0) {
					finished = false;
				}
				if (finished) {
					endPortal->setFinishState(finished);
				}
			}
		}
	}
	if (collMoney < 0) { collMoney = 0; } 



#if 0
	if (collMoney == -50) {
		if (hasPotion) { collMoney += 50; }
		else {
			if (gold >= 50) { hasPotion = true; std::cout << "Acquired potion!\n"; }
			else { collMoney = 0; }
		}
	}
#endif // 0



	hurt(accDamage); addMoney(collMoney);
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

		if (paused) {
			if(paused) {
				pauseOverlay.render(0, 0);
			}
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

		for (int i = 1; i <= 9; i++) {
			if (player.hasItem(i)) {
				player.getItem(i)->render(i);
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
