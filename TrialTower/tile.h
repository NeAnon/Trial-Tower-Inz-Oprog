#pragma once

//Base includes
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

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