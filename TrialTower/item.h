#pragma once
#include <string>
#include <vector>

class Item {
private:
	bool equipped;
	std::string type;
	int cost;
public:
	Item(bool e = false, std::string t = "Null", int c = 0) { equipped = e; type = t; cost = c; }

	~Item() { std::cout << "Destroying item of type " << type << " and cost " << cost << " which " << (equipped ? "was" : "was not") << " equipped by the player.\n"; }

	virtual std::string echo_type() { return type; }
	int echo_cost() { return cost; }
	virtual int getIntMetadata() { return 0; }
	virtual std::string getStrMetadata() { return ""; }
};

class Potion : Item {
private:
	bool heal;
public:
	Potion(int c = 0, int h = 0, bool e = false, std::string t = "Potion") : Item(e, t, c) { heal = h; }

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
};

class InventoryList {
private:
	std::vector<Inventory> inventories;
public:
	InventoryList() {
		inventories.reserve(100);
	}
	~InventoryList() {
		inventories.clear();
	}

};




