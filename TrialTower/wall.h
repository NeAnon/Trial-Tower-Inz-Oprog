#pragma once

//Base includes
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

//Wrapped texture class 
#include "WTexture.h"

const int TILE_SIZE = 32;

class Wall {
private:
	//Player texture and current sprite
	WTexture mTexture;
	SDL_Rect clip;

	//Player position (in tiles)
	int mPosX;
	int mPosY;

	//Whether the wall is walkable
	bool isWalkable;

public:
	//Player contructor
	Wall();

	//Player destructor
	~Wall();

	//Sets renderer for own texture
	void setRenderer(SDL_Renderer* renderPtr) { mTexture.setRenderer(renderPtr); }

	//Load any media required to display the wall
	void loadMedia()
	{
		//Load wall texture	
		if (!mTexture.loadFromFile("resources/wall_dark.png"))
		{
			printf("Failed to load wall texture!\n");
		}
	}

	//Renders the player at their position
	void render(int posX, int posY);

	void setPos(int x, int y) { mPosX = x; mPosY = y; }
};

inline Wall::Wall()
{
	//Set wall position (specified as grid params)
	mPosX = 0; mPosY = 0;
	
	//Set wall sprite (for this it's just the whole sprite, this will come in useful with tilesets)
	clip = { 0,0,TILE_SIZE,TILE_SIZE };
}

inline Wall::~Wall()
{
	//Deallocate stored texture to avoid memory leakage
	mTexture.free();
}

inline void Wall::render(int posX, int posY)
{
	//Render the wall at given grid coords (may need to change the numbers to adjust for level grid)
	mTexture.render(posX * TILE_SIZE, posY * TILE_SIZE, &clip);
}

class Portal {
private:
	//Player texture and current sprite
	WTexture mTexture;
	WTexture mText;
	SDL_Rect clip;

	//Player position (in tiles)
	int mPosX;
	int mPosY;

	//Whether the wall is walkable
	bool isWalkable;

public:
	//Portal contructor
	Portal();

	//Portal destructor
	~Portal();

	//Sets renderer for own texture
	void setRenderer(SDL_Renderer* renderPtr) { mTexture.setRenderer(renderPtr); mText.setRenderer(renderPtr); }

	//Load any media required to display the wall
	void loadMedia()
	{
		//Load portal texture	
		if (!mTexture.loadFromFile("resources/portal.png"))
		{
			printf("Failed to load portal texture!\n");
		}
		//Load portal texture	
		if (!mText.loadFromFile("resources/objective.png"))
		{
			printf("Failed to load message texture!\n");
		}
	}

	//Renders the portal at their position
	void render(int posX, int posY, bool isActive);

	void setPos(int x, int y) { mPosX = x; mPosY = y; }
};

Portal::Portal() {
	//Set wall position (specified as grid params)
	mPosX = 0; mPosY = 0;

	//Set wall sprite (for this it's just the whole sprite, this will come in useful with tilesets)
	clip = { 0,0,TILE_SIZE,TILE_SIZE };
}

inline Portal::~Portal()
{
	mTexture.free();
	mText.free();
}

inline void Portal::render(int posX, int posY, bool isActive = false)
{
	//Render the wall at given grid coords (may need to change the numbers to adjust for level grid)
	mTexture.render(posX * TILE_SIZE, posY * TILE_SIZE, &clip);
	if (isActive) {
		mText.render(posX * TILE_SIZE - (mText.getWidth()*3 / 8), posY * TILE_SIZE - mText.getHeight());
	}
}