#pragma once
#include <string>
#include <vector>

class Item {
private:
	std::string type;
	int cost;
public:
	std::string echo_type() { return type; }
};

class Inventory {
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