#pragma once

//Base includes
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <vector>

//Wrapped texture class 
#include "WTexture.h"

//Wall handling
#include "wall.h"


//Directionals corresponding to images on the sprite sheet
enum {
	DIRECTION_CENTER,	//0
	DIRECTION_UP,		//1
	DIRECTION_RIGHT,	//2
	DIRECTION_DOWN,		//3
	DIRECTION_LEFT,		//4
};

class Player {
private:
	//Player texture and current sprite
	WTexture mTexture;
	SDL_Rect clip;

	//Player position (in tiles)
	int mPosX;
	int mPosY;

public:
	//Player contructor
	Player();

	//Player destructor
	~Player();

	//Sets renderer for own texture
	void setPlayerRenderer(SDL_Renderer* renderPtr) { mTexture.setRenderer(renderPtr); }

	//Load any media required to display the player
	void loadPlayerMedia()
	{
		//Load player texture	
		if (!mTexture.loadFromFile("resources/playerSpriteSheetTransparent.png"))
		{
			printf("Failed to load player texture!\n");
		}
	}

	//Movement handler
	void move(int direction, std::vector<std::vector<int> > &wallMap);

	//Renders the player at their position
	void render();
};

inline Player::Player() {
	//Set dimensions of player's sprites
	clip = { 0, 0, 32, 32 };

	//Set player position (specified as grid params)
	mPosX = 1; mPosY = 1;
}

inline Player::~Player()
{
	//Deallocate stored texture to avoid memory leakage
	mTexture.free();
}

inline void Player::move(int direction, std::vector<std::vector<int> > &wallMap)
{
	//Set clip square to player's current direction (use case if irregular sprite sizes)
	clip.x = direction * 32;
	
	//Depending on direction pressed, if within boundaries and not about to run into a wall, move the player
	switch (direction) {
	case DIRECTION_UP:
		if ((mPosY - 1 >= 0) && wallMap[mPosX][mPosY - 1] == 0) { mPosY--; }
		break;
	case DIRECTION_RIGHT:
		if ((mPosX + 1 < wallMap.size()) && wallMap[mPosX + 1][mPosY] == 0) { mPosX++; }
		break;
	case DIRECTION_DOWN:
		if ((mPosY + 1 < wallMap[mPosX].size()) && wallMap[mPosX][mPosY + 1] == 0) { mPosY++; }
		break;
	case DIRECTION_LEFT:
		if ((mPosX - 1 >= 0) && wallMap[mPosX-1][mPosY] == 0) { mPosX--; } 
		break;
	default:
		break;
	}
}

inline void Player::render()
{
	//Render the player at given grid coords (may need to change the numbers to adjust for level grid)
	mTexture.render(mPosX * 32, mPosY * 32, &clip);
}