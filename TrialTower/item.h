#pragma once
#include <string>
#include <vector>
#include "WTexture.h"

static WTexture items;
static WTexture itemTxt;
static WTexture itemGlyphs;

static void preloadItems(SDL_Renderer* renderPtr) {
	items.setRenderer(renderPtr);
	if (!items.loadFromFile("resources/itemList.png"))
	{
		printf("Failed to load item texture! SDL_image Error: %s\n", IMG_GetError());
	}
	itemTxt.setRenderer(renderPtr);
	if (!itemTxt.loadFromFile("resources/itemText.png"))
	{
		printf("Failed to load item text texture! SDL_image Error: %s\n", IMG_GetError());
	}
	itemGlyphs.setRenderer(renderPtr);
	if (!itemGlyphs.loadFromFile("resources/itemGlyphs.png"))
	{
		printf("Failed to load item glyph texture! SDL_image Error: %s\n", IMG_GetError());
	}
};
static void renderText(int screenPosX, int screenPosY) {};

enum {
	GLYPH_COST = 0, //0
	GLYPH_PDMG,		//1
	GLYPH_PDEF,		//2
	GLYPH_MDMG,		//3
	GLYPH_MDEF,		//4
	GLYPH_DMG,		//5
	GLYPH_DEF,		//6
	GLYPH_HP,		//7
	GLYPH_HEAL,		//8
	GLYPH_SPD,
	GLYPH_JUMP,
	GLYPH_PHASE,
	GLYPH_INVIN,

	EFFECT_COUNT,	//n
};

enum {
	TYPE_NULL = 0,	//0
	TYPE_WEAP,		//1
	TYPE_SHLD,		//2
	TYPE_HELM,		//3
	TYPE_CHST,		//4
	TYPE_LEGS,		//5
	TYPE_BOOT,		//6
	TYPE_POTN,		//7
	TYPE_RING,		//8
};

class Item {
private:
	SDL_Rect itemSprite;
	SDL_Rect workingTextSprite;
	bool equipped;

	WTexture scroll;

	int type;
	int cost;
	int effect;
	int potency;
public:
	Item(bool e = false, int t = 0, int c = 0, int p = 0, std::string loadedTexture = "", SDL_Renderer* renderPtr = nullptr) {
		equipped = e; type = t; cost = c; potency = p; effect = 0; itemSprite = { 0,0,32,32 }; workingTextSprite = { 0,0,8,8 };
		bindScroll(loadedTexture, renderPtr);
		//std::cout << "Creating item of type " << type << " and cost " << cost << " which " << (equipped ? "was" : "was not") << " equipped by the player.\n";
	}

	~Item() { 
		//std::cout << "Destroying item of type " << type << " and cost " << cost << " which " << (equipped ? "was" : "was not") << " equipped by the player.\n";
	}

	void set_sprite(int x = -1, int y = -1) { if (x >= 0) { itemSprite.x = x * 32; } if (y >= 0) { itemSprite.y = y * 32; } }
	void set_effect(int e) { effect = e; }
	void set_potency(int m) { potency = m; }
	void set_equipped(bool eq) { equipped = eq; }
	void set_cost(int c) { cost = c; }

	void bindScroll(std::string texturePath, SDL_Renderer* renderPtr) {
		scroll.setRenderer(renderPtr);
		if (!scroll.loadFromFile("resources/" + texturePath + ".png"))
		{
			printf("Failed to load scroll texture! SDL_image Error: %s\n", IMG_GetError());
		}
	}

	virtual int echo_type() { return type; }
	int echo_effect() { return effect; }
	int echo_cost() { return cost; }
	int echo_potency() { return potency; }
	bool isEquipped() { return equipped; }


	virtual int getIntMetadata() { return 0; }
	virtual std::string getStrMetadata() { return ""; }


	void render(bool exa = false, int slot = 0, int posX = 0, int posY = 0, int pX = 0, int pY = 0) {

		if (equipped) {
			items.render(160 + ((slot - 1) * 32), WTexture::getGlobalLHeight(), &itemSprite);
			if (effect) {
				workingTextSprite.x = effect * 8;
				itemGlyphs.render(160 + ((slot - 1) * 32), WTexture::getGlobalLHeight(), &workingTextSprite);
				std::string displaypotency = std::to_string(potency);
				for (int i = 0; i < displaypotency.size(); i++) {
					workingTextSprite.x = (displaypotency[i] - '0') * 8;
					itemTxt.render(160 + ((slot - 1) * 32) + (8 * (i + 1)), WTexture::getGlobalLHeight(), &workingTextSprite);
				}
			}
		}
		else
		{		
			if (exa) {
				scroll.render(pX < 10 ? WTexture::getGlobalLWidth() - scroll.getWidth() : 0, pY < 8 ? WTexture::getGlobalLHeight() - scroll.getHeight() : 0);
			}
			items.render(posX * 32, posY * 32, &itemSprite);
			if (cost) {
				workingTextSprite.x = 0;
				itemGlyphs.render(posX * 32, (posY * 32) + 24, &workingTextSprite);
				std::string displaycost = std::to_string(cost);
				for (int i = 0; i < displaycost.size(); i++) {
					workingTextSprite.x = (displaycost[i] - '0') * 8;
					itemTxt.render((posX * 32) + (8 * (i + 1)), (posY * 32) + 24, &workingTextSprite);
				}
			}
			if (effect) {
				workingTextSprite.x = effect * 8;
				itemGlyphs.render(posX * 32, posY * 32, &workingTextSprite);
				std::string displaypotency = std::to_string(potency);
				for (int i = 0; i < displaypotency.size(); i++) {
					workingTextSprite.x = (displaypotency[i] - '0') * 8;
					itemTxt.render((posX * 32) + (8 * (i + 1)), posY * 32, &workingTextSprite);
				}
			}
		}
	}
};

class Weapon : public Item {
private:
	bool active;
public:
	Weapon(int c = 0, int ef = 0, int p = 0, bool e = false, int t = TYPE_WEAP, std::string loadedTexture = "", SDL_Renderer* renderPtr = nullptr) : Item(e, t, c, p, loadedTexture, renderPtr) { set_effect(ef); set_sprite(1); active = true; }

	//int getHealStrength() { return heal; }
	//int getIntMetadata() { return getHealStrength(); }

};

class Boots : public Item {
private:
	bool active;
public:
	Boots(int c = 0, int ef = 0, int p = 0, bool e = false, int t = TYPE_BOOT, std::string loadedTexture = "", SDL_Renderer* renderPtr = nullptr) : Item(e, t, c, p, loadedTexture, renderPtr) { set_effect(ef); set_sprite(6); active = true; }

	//int getHealStrength() { return heal; }
	//int getIntMetadata() { return getHealStrength(); }

};

class Potion : public Item {
private:
	int heal;
public:
	Potion(int c = 0, int h = 0, bool e = false, int t = TYPE_POTN, std::string loadedTexture = "", SDL_Renderer* renderPtr = nullptr) : Item(e, t, c, 0, loadedTexture, renderPtr) { heal = h; set_potency(heal); set_effect(GLYPH_HEAL); set_sprite(7); }

	int getHealStrength() { return heal; }
	int getIntMetadata() { return getHealStrength(); }

};

static bool ShowItemRenders = false;

class Inventory {
	//Not a classic inventory, more like the list of items on the ground 
private:
	std::vector<Item*> items;
	int posX; int posY;
public:
	Inventory(int x = 0, int y = 0) {
		items.reserve(100); posX = x; posY = y;
	}
	~Inventory() {
		for (int i = 0; i < items.size(); i++) {
			delete items[i];
			items[i] = nullptr;
		}
	}

	int getX() { return posX; }
	int getY() { return posY; }
	bool empty() {
		bool empty = true;
		for (int i = 0; i < items.size(); i++) {
			if (items[i] != nullptr) {
				//std::cout << "Item at " << i << "\n";
				empty = false;
			}
		}
		return empty;
	}
	int size() { return items.size(); }

	void addItem(Item* item) {
		items.push_back(item);
		if (items.back() != nullptr && !items.empty())
		{
			//std::cout << "Pushed item of type " << items.back()->echo_type() << " and cost " << items.back()->echo_cost() << " into inventory (" << posX << "," << posY << ")\n";
		}
	}

	Item* itemAt(int i) {
		if (i < items.size() && i >= 0) {
			return items[i];
		}
	}

	void removeItem(int index) {
		if (index >= 0 && index < items.size() && items[index] != nullptr) {
			delete items[index];
			items[index] = nullptr;
		}
	}

	void giveItems(Inventory& targetInv, int& ownedGold) {
		for (int i = 0; i < items.size(); i++) {
			if (items[i] != nullptr) {
				if (items[i]->echo_cost() <= ownedGold) {
					//Buy item (this has no effect if cost == 0)
					ownedGold -= items[i]->echo_cost(); items[i]->set_cost(0);

					//Add item to target and remove it from self
					targetInv.addItem(items[i]); items[i] = nullptr;
				}
			}
		}
	}

	void callAllItems() {
		for (int i = 0; i < items.size(); i++) {
			if (items[i]) {
				items[i]->echo_type();
			}
		}
	}

	void render(bool exa = false, int pX = 0, int pY = 0) {
		for (int i = 0; i < items.size(); i++) {
			if (items[i] != nullptr)
			{
				items[i]->render(exa, 0, posX, posY, pX, pY);
				if (ShowItemRenders) {
					//std::cout << "Rendered item " << i << "from inventory at " << posX << ", " << posY << "!\n;";
				}
			}
		}
	}
};

class InventoryList {
private:
	std::vector<Inventory*> inventories;
public:
	InventoryList() {
		inventories.reserve(1000);
	}
	~InventoryList() {
		clear();
	}
	void addItem(int posX, int posY, Item* item) {
		for (int i = 0; i < inventories.size(); i++) {
			if (inventories[i] != nullptr && inventories[i]->getX() == posX && inventories[i]->getY() == posY) {
				inventories[i]->addItem(item);
				return;
			}
		}
		for (int i = 0; i < inventories.size(); i++) {
			if (inventories[i] == nullptr) {
				//std::cout << "Creating inventory at (" << posX << "," << posY << ")\n";
				inventories[i] = new Inventory(posX, posY);
				inventories[i]->addItem(item);
				return;
			}
		}
		//std::cout << "Creating inventory at (" << posX << "," << posY << ")\n";
		inventories.push_back(new Inventory(posX, posY));
		//std::cout << "Inventory count now at: " << inventories.size() << "\n";
		inventories.back()->addItem(item);
		return;
	}

	Inventory* isInvAt(int x, int y) {
		for (int i = 0; i < inventories.size(); i++) {
			if (inventories[i] != nullptr) {
				//std::cout << "Checking inventory at (" << x << "," << y << ")\n";
				if (inventories[i]->getX() == x && inventories[i]->getY() == y) {
					return inventories[i];
				}
			}
		}
		return nullptr;
	}

	void removeAt(int x, int y) {
		//std::cout << "removeAt triggered\n";
		for (int i = 0; i < inventories.size(); i++) {
			if (inventories[i] != nullptr && inventories[i]->getX() == x && inventories[i]->getY() == y)
			{
				delete inventories[i];
				inventories[i] = nullptr;
				//std::cout << "Deleting Inventory (" << x << "," << y << ")\n";
			}
		}
	}

	void render(int pX, int pY) {
		for (int i = 0; i < inventories.size(); i++) {
			if (inventories[i] != nullptr) {
				if (inventories[i]->getX() == pX && inventories[i]->getY() == pY + 1) {
					inventories[i]->render(true, pX, pY);
				}
				else if (inventories[i]->getX() == pX + 1 && inventories[i]->getY() == pY) {
					inventories[i]->render(true, pX, pY);
				}
				else if (inventories[i]->getX() == pX - 1 && inventories[i]->getY() == pY) {
					inventories[i]->render(true, pX, pY);
				}
				else if (inventories[i]->getX() == pX && inventories[i]->getY() == pY - 1) {
					inventories[i]->render(true, pX, pY);
				}
				else {
					inventories[i]->render();
				}
			}
		}
	}

	void clear() {
		for (int i = 0; i < inventories.size(); i++) {
			if (inventories[i] != nullptr) {
				//std::cout << "Deleting Inventory (" << inventories[i]->getX() << "," << inventories[i]->getY() << ")\n";
				//std::cout << "It contained items: "; inventories[i]->callAllItems(); std::cout << "\n";
				delete inventories[i];
				inventories[i] = nullptr;
			}
		}

	}
};




