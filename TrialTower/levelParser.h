#pragma once

#include <fstream>
#include <string>
#include <iostream>

#include "item.h"
#include "enemy.h"
#include "levelMap.h"
#include "player.h"

void parseLevel(std::string filepath, Player& player, LevelMap& lvlMap, enemyList& eList, InventoryList& invlist, SDL_Renderer* renderPtr) {
	std::cout << "Beginning parse...\n";
	std::ifstream file{ filepath };

	std::cout << "File " << filepath << " opened...\n";

	int xSize = 0; int ySize = 0;
	std::string buf;

	file >> buf;
	//std::cout << "xSize read: " << buf << "\n"; std::cout << "converting to int...\n";
	xSize = std::stoi(buf);
	file >> buf;
	//std::cout << "ySize read: " << buf << "\n"; std::cout << "converting to int...\n";
	ySize = std::stoi(buf);

	std::cout << "X: " << xSize << " Y: " << ySize << "\n\n";

	lvlMap.remakeLevel(xSize, ySize);

	bool playerAdded = false;
	eList.clear();
	invlist.clear();

	std::cout << "Beginning read of rest of file...\n";
	for (int y = 0; y < ySize; y++) {
		for (int x = 0; x < xSize; x++) {
			file >> buf;
			//std::cout << buf << "\t\t buf read, pushed at x: " << x << ", y: " << y << "\n";
			//	Tile
			if (buf[0] == 'T') {
				if (buf[1] == 'W') {
					lvlMap.insertObj(new Wall(x, y, renderPtr));
				}
				else if (buf[1] == 'P') {
					lvlMap.insertPortal(new Portal(x, y, renderPtr));
				}
				else if (buf[1] == 'S') {
					Trap* temp = nullptr;
					if (buf[2] == 'A') { 
						temp = new SpikeTrap(x, y, renderPtr, true);
					}
					else {
						std::cout << "At " << x << ", " << y << " the trap is not traversible!\n";
						temp = new SpikeTrap(x, y, renderPtr, false);
					}
					lvlMap.insertObj(temp);
					lvlMap.addTrap(temp);
				}
			}
			else {
				//	Entity/Enemy
				if (buf[0] == 'E') {
					if (buf[1] == 'G') {
						eList.addEnemy(x, y);
					}
					else if (buf[1] == 'S') {
						eList.addEnemy(x, y, "Slide", buf[2] - '0');
					}
					else if (buf[1] == 'W') {
						eList.addEnemy(x, y, "Wander");
					}
					else if (buf[1] == 'M') {
						eList.addEnemy(x, y, "PotionSeller");
					}
					else { eList.addEnemy(x, y); }
				}
				//	Player
				if (buf[0] == 'P' && !playerAdded) {
					player.setX(x); player.setY(y);
					playerAdded = true;
				}
				lvlMap.insertObj(new Floor(x, y, renderPtr));
			}
		}
	}
	std::cout << "Level loaded, loading items...\n";
	buf = "";
	//format: type, cost, x, y
	std::string type = ""; std::string cost = ""; std::string effect = ""; std::string potency = ""; 
	std::string itemX = "", itemY = "";
	while (!file.eof()) {
		file >> buf;
		if (buf != "")
		{
				if (type == "") {
					type = buf;
				}
				else if (cost == "") {
					cost = buf;
				}
				else if (effect == "") {
					effect = buf;
				}
				else if (potency == "") {
					potency = buf;
				}
				else if (itemX == "") {
					itemX = buf;
				}
				else {
					itemY = buf;
					Item* newItem = nullptr;
					switch(std::stoi(type)){
					case TYPE_BOOT:
						newItem = new Boots(std::stoi(cost), std::stoi(effect), std::stoi(potency), false);
						break;
					case TYPE_POTN:
						newItem = new Potion(std::stoi(cost), std::stoi(potency));
						break;
					default:
						newItem = new Item(false, std::stoi(type), std::stoi(cost));
					}
					invlist.addItem(stoi(itemX), stoi(itemY), newItem);
					type = ""; cost = ""; effect = ""; potency = ""; itemX = ""; itemY = "";
					buf.clear();
				}
		}
	}

	if (type != "") {
		if (cost == "") {
			cost = "0";
		}
		if (effect == "") {
			cost = "0";
		}
		if (potency == "") {
			potency = "0";
		}
		if (itemX == "") {
			itemX = "0";
		}
		if (itemY == "") {
			itemY = "0";
		}
		Item* newItem = new Item(false, std::stoi(type), std::stoi(cost));
		invlist.addItem(stoi(itemX), stoi(itemY), newItem);
	}

	std::cout << "Ending file read.\n";
}