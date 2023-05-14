#pragma once
#include <string>
#include <vector>
#include "WTexture.h"

static WTexture items;
static WTexture itemTxt;
static WTexture itemGlyphs;

static void preloadItems(SDL_Renderer* renderPtr) {
	items.setRenderer(renderPtr);	
	if (!items.loadFromFile("resources/unknownItem.png"))
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
};



class Item {
private:
	SDL_Rect itemSprite;
	SDL_Rect workingTextSprite;
	bool equipped;
	std::string type;
	int cost;
	int effect;
	int potency;
public:
	Item(bool e = false, std::string t = "Null", int c = 0) {
		equipped = e; type = t; cost = c; effect = 0; itemSprite = { 0,0,32,32 }; workingTextSprite = { 0,0,8,8 };
	std::cout << "Creating item of type " << type << " and cost " << cost << " which " << (equipped ? "was" : "was not") << " equipped by the player.\n"; }

	~Item() { std::cout << "Destroying item of type " << type << " and cost " << cost << " which " << (equipped ? "was" : "was not") << " equipped by the player.\n";}

	void set_effect(int e) { effect = e; }
	void set_potency(int m) { potency = m; }

	virtual std::string echo_type() { return type; }
	int echo_cost() { return cost; }


	virtual int getIntMetadata() { return 0; }
	virtual std::string getStrMetadata() { return ""; }
	
	
	void render(int slot = 0, int posX = 0, int posY = 0) {
		if (equipped) {
			items.render(160 + (slot * 32), WTexture::getGlobalLHeight(), &itemSprite);
		}
		else
		{
			items.render(posX * 32, posY * 32, &itemSprite);
			if (cost) {
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
					workingTextSprite.x = (displaypotency[i] - '0')*8;
					itemTxt.render((posX * 32) + (8 * (i + 1)), posY * 32, &workingTextSprite);
				}
			}
		}
	}
};

class Potion : public Item {
private:
	int heal;
public:
	Potion(int c = 0, int h = 0, bool e = false, std::string t = "Potion") : Item(e, t, c) { heal = h; set_potency(heal); set_effect(GLYPH_HEAL); }

	int getHealStrength() { return heal; }
	int getIntMetadata(){ return getHealStrength(); }
};

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
	void addItem(Item* item) {
		items.push_back(item);
		std::cout	<< "Pushed item of type " << items.back()->echo_type()
					<< " and cost " << items.back()->echo_cost()
					<< " into inventory (" << posX << "," << posY << ")\n";
	}

	void callAllItems() {
		for (int i = 0; i < items.size(); i++) {
			items[i]->echo_type();
		}
	}

	void render() {
		for (int i = 0; i < items.size(); i++) {
			if(items[i] != nullptr)
			{
				items[i]->render(0, posX, posY);
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
				std::cout << "Creating inventory at (" << posX << "," << posY << ")\n";
				inventories[i] = new Inventory(posX, posY);
				inventories[i]->addItem(item);
				return;	
			}
		}
		std::cout << "Creating inventory at (" << posX << "," << posY << ")\n";
		inventories.push_back(new Inventory(posX, posY));
		inventories.back()->addItem(item);
		return;
	}

	void render() {
		for (int i = 0; i < inventories.size(); i++) {
			if (inventories[i] != nullptr) {
				inventories[i]->render();
				return;
			}
		}
	}

	void clear() {
		for (int i = 0; i < inventories.size(); i++) {
			if (inventories[i] != nullptr) {
				std::cout << "Deleting Inventory (" << inventories[i]->getX() << "," << inventories[i]->getY() << ")\n";
				std::cout << "It contained items: "; inventories[i]->callAllItems(); std::cout << "\n";
				delete inventories[i];
				inventories[i] = nullptr;
			}
		}
		
	}
};




