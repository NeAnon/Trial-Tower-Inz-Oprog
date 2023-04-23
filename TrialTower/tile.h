#pragma once

//Base includes
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <list>

//Wrapped texture class 
#include "WTexture.h"

//Tiles inherit from objects
#include "object.h"

const int TILE_SIZE = 32;

class Tile : public Object {
private:
	//Whether the tile is walkable
	bool isWalkable;
public:
	Tile();

	Tile(int x, int y, SDL_Renderer* renderPtr);

	~Tile();

	virtual void renderText(int posX, int posY, int screenW, int screenY) = 0;

	virtual void handlePlayer() {};
};

Tile::Tile() : Object() {
	isWalkable = true;
}

inline Tile::Tile(int x, int y, SDL_Renderer* renderPtr) : Object(x, y, renderPtr)
{
	isWalkable = true;
}

inline Tile::~Tile()
{

}

//Walls are treated as impassable tiles
class Wall : public Tile {
private:

public:
	//Wall contructor
	Wall();

	//Wall contructor at pos
	Wall(int x, int y, SDL_Renderer* renderPtr);

	//Wall destructor
	~Wall();

	void loadWallMedia() {
		//Load media specifically used by the walls
		loadMedia("wall_rock.png");
	}

	std::string echo() { return "Wall"; }

	void setPos(int x, int y) { setX(x); setY(y); }

	void renderText(int x, int y, int screenW, int screenY) {};
};

inline Wall::Wall()
{
}

inline Wall::Wall(int x, int y, SDL_Renderer* renderPtr = nullptr) : Tile(x, y, renderPtr)
{
	loadWallMedia();
}

inline Wall::~Wall()
{
}

//Walls are treated as impassable tiles
class Floor : public Tile {
private:

public:
	//Wall contructor
	Floor();

	//Wall contructor at pos
	Floor(int x, int y, SDL_Renderer* renderPtr);

	//Wall destructor
	~Floor();

	void loadFloorMedia() {
		//Load media specifically used by the walls
		loadMedia("floor_wood.png");
	}

	std::string echo() { return "Floor"; }

	void setPos(int x, int y) { setX(x); setY(y); }

	void renderText(int x, int y, int screenW, int screenY) {};
};

inline Floor::Floor() {

}

inline Floor::Floor(int x, int y, SDL_Renderer* renderPtr): Tile(x, y, renderPtr)
{
	loadFloorMedia();
}

inline Floor::~Floor()
{
}

class Portal : public Tile {
private:
	//Text shown before level completion
	WTexture mText;

	//Flag for whether the level was finished
	bool active;

public:
	//Portal contructor
	Portal();

	//Portal contructor at pos
	Portal(int x, int y, SDL_Renderer* renderPtr);

	//Portal destructor
	~Portal();

	//Load any media required to display the wall
	void loadPortalMedia()
	{

		//Load media specifically used by the enemies
		loadMedia("portal.png");
		//Load portal texture	
		if (!mText.loadFromFile("resources/objective.png"))
		{
			printf("Failed to load message texture!\n");
		}
	}

	//Renders the portal's text at its position
	void renderText(int posX, int posY, int screenW, int screenY);

	std::string echo() { return "Portal"; }

	void setPos(int x, int y) { setX(x); setY(y); }

	bool isFinished() {return active;}
	void setFinishState(bool finish) { active = finish; }
};

inline Portal::Portal() {
	bool active = false;
}

inline Portal::Portal(int x, int y, SDL_Renderer* renderPtr = nullptr) : Tile(x, y, renderPtr)
{
	bool active = false;
	mText.setRenderer(renderPtr);
	loadPortalMedia();
}

inline Portal::~Portal()
{
	mText.free();
}


inline void Portal::renderText(int posX, int posY, int screenW, int screenY)
//posX, posY refer to the portal position within the map
{
	int renderX = 0; int renderY = 0;
	
	//portalX/portalY refer to the rendered top-left anchors of portals 
	int portalX = posX * TILE_SIZE; int portalY = posY * TILE_SIZE;

	//X-position on screen required for centering the text
	int centeredTextX = 3* mText.getWidth() / 8;

	//Assign X pos
	if ((portalX - centeredTextX) < 0) { renderX = 0; }
	else if ((portalX - centeredTextX + mText.getWidth()) > screenY) { renderX = posX * TILE_SIZE - mText.getWidth() + TILE_SIZE; }
	else { renderX = portalX - centeredTextX; }
	
	//Assign Y pos (default above portal)
	if (portalY - mText.getHeight() > 0) {
		renderY = portalY - mText.getHeight();
	}
	else {
		renderY = portalY + TILE_SIZE;
	}
	//Render text at decided pos
	mText.render(renderX, renderY);
}

class Trap : public Tile {
	int state;
	bool activated;
public:
	Trap() {}

	Trap(int x, int y, SDL_Renderer* renderPtr) : Tile(x, y, renderPtr) { state = 0; activated = false; }

	int getState() { return state; }
	void setState(int n) { state = n; }
	void setActive(bool val) { activated = val; } 
	bool checkActive() { return activated; }

	virtual void activate(bool, int&) {};
	virtual void updateClip() {}
};

class SpikeTrap : public Trap {
private:
	bool isPassable;
	
public:
	//Wall contructor
	SpikeTrap();

	//Wall contructor at pos
	SpikeTrap(int x, int y, SDL_Renderer* renderPtr, bool passable = false);

	//Wall destructor
	~SpikeTrap();

	void loadFloorMedia() {
		//Load media specifically used by the walls
		loadMedia("spikeTrap.png");
	}

	std::string echo() { return "SpikeTrap"; }

	void setPos(int x, int y) { setX(x); setY(y); }

	void renderText(int x, int y, int screenW, int screenY) {};

	bool isSoft() { return isPassable; }
	void activate(bool triggered, int &damageCounter);
	void updateClip() { if (getState() <= 0 && isSoft()) { setClip(0, 0); } else { setClip(1, 0); } }
};

inline SpikeTrap::SpikeTrap(){
	bool isPassable = false;
	int state = 0;
}

inline SpikeTrap::SpikeTrap(int x, int y, SDL_Renderer* renderPtr, bool passable) : Trap(x, y, renderPtr)
{
	isPassable = passable;
	setState(0);
	if (!passable) { setClip(1, 0); }
	loadFloorMedia();
}

inline SpikeTrap::~SpikeTrap()
{
}


inline void SpikeTrap::activate(bool triggered, int &damageCounter)
{	//0 is resting state - logic needs to be determined separately for "soft" and "hard" spike-traps
	if (isPassable) {
		if (triggered) {
			if (getState() == 0) { 
				setState(getState() + 1); 
				if (!checkActive()) { setActive(true); }
				return; 
			}
			else {
				setState(2); setClip(1, 0); damageCounter += 25;
			}
		}
		else {
			setState(getState() + 1);
			if (getState() >= 1) { setClip(1, 0); }
			if (getState() == 5) {
				setState(0);
				setClip(0, 0);
			}
		}
		if (!checkActive()) { setActive(true); }
		return;
	} else {
		damageCounter += 25;
	}
	if (!checkActive()) { setActive(true); }
	return;
}

class trapList {
private:
	std::vector<Trap*> list;
	std::vector<Trap*> activeList;
	SDL_Renderer* global_renderer;
public:
	trapList();

	~trapList();

	void setRenderer(SDL_Renderer* renderPtr) { global_renderer = renderPtr; };

	void addTrap(Trap* selectedTrap, int metadata);

	void clear();

	void activateAt(int x, int y, int& damage);

	//void reloadMedia();

	//void renderAll();

	void updateAll();
};

trapList::trapList() { list.reserve(1000); activeList.reserve(1000); global_renderer = NULL; }

inline trapList::~trapList()
{
	for (int i = 0; i < list.size(); i++) {
		if (list[i] != nullptr) {
			//std::cout << "Deleting ref from pos " << i << '\n';
			list[i] = nullptr;
		}
	}
}

inline void trapList::addTrap(Trap* selectedTrap, int metadata = 0)
{
	list.push_back(selectedTrap);
}

inline void trapList::clear()
{
	for (int i = 0; i < list.size(); i++) {
		list[i] = nullptr;
	}
}

inline void trapList::activateAt(int x, int y, int& damage)
{
	//Placeholder for checking for another instance of a tile being active
	int free_spot = -1;
	//std::cout << "Activation at " << x << " " << y << "\n";
	for (int i = 0; i < list.size(); i++) {
		if (list[i]->getX() == x && list[i]->getY() == y) {
			//std::cout << "Located trap...\n";
			list[i]->activate(true, damage);
			for (int j = 0; j < activeList.size(); j++) {
				if (activeList[j] == nullptr) {
					if (free_spot == -1) {
						free_spot = j;
					}
				} else if (list[i]->getX() == activeList[j]->getX() && list[i]->getY() == activeList[j]->getY()) { return; }
			}
			if (free_spot >= 0) { activeList[free_spot] = list[i]; }
			else { activeList.push_back(list[i]); }
		}
	}
}

inline void trapList::updateAll()
{
	int dummy = 0;
	for (int i = 0; i < activeList.size(); i++) {
		if (activeList[i] != nullptr && activeList[i]->checkActive() == false){
			activeList[i]->activate(false, dummy);
			//std::cout << activeList[i]->echo() << " activated at (" << activeList[i]->getX() << ", " << activeList[i]->getY() << "), state = " << activeList[i]->getState() << "\n";
			if (activeList[i]->getState() == 0) {
				//std::cout << "Tile at " << activeList[i]->getX() << ", " << activeList[i]->getY() << " has state " << activeList[i]->getState() << "\n";
				//std::cout << "Tile removed from activation list\n";
				activeList[i] = nullptr;
			}
			
		}
		if (activeList[i] != nullptr) {
			activeList[i]->setActive(false);
			activeList[i]->updateClip();
		}
	}
}
