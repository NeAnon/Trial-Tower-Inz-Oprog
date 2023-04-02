#pragma once

//Base includes
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <vector>

//Wrapped texture class 
#include "WTexture.h"

class Object {
private:
	//Player texture and current sprite
	WTexture mTexture;
	SDL_Rect clip;

	//Player position (in tiles)
	int mPosX;
	int mPosY;
public:
	Object();

	Object(int x, int y);

	Object(int x, int y, SDL_Renderer* renderPtr);

	~Object();

	//Load any media required to display the entity
	void loadMedia(std::string path)
	{
		//Load entity texture	
		if (!mTexture.loadFromFile("resources/" + path))
		{
			printf("Failed to load texture! File: %s SDL_image Error: %s\n", path.c_str(), IMG_GetError());
		}
	}

	//Getters and setters
	int getX() { return mPosX; }	void setX(int x) { mPosX = x; }
	int getY() { return mPosY; }	void setY(int y) { mPosY = y; }

	//Set tile_size clip to x and y coords on the sprite sheet
	void setClip(int x, int y) { clip.x = x * 32; clip.y = y * 32; }

	//Sets renderer for own texture
	void setRenderer(SDL_Renderer* renderPtr) { mTexture.setRenderer(renderPtr); }

	inline void render()
	{
		//Render the entity at given grid coords (may need to change the numbers to adjust for level grid)
		mTexture.render(mPosX * 32, mPosY * 32, &clip);
	}

	virtual std::string echo() = 0;
};

Object::Object() {
	//Set dimensions of object's sprites
	clip = { 0, 0, 32, 32 };

	//Set object position (specified as grid params)
	mPosX = 1; mPosY = 1;
}

inline Object::Object(int x, int y)
{
	//Set dimensions of object's sprites
	clip = { 0, 0, 32, 32 };

	//Set object position (specified as grid params)
	mPosX = x; mPosY = y;
}

inline Object::Object(int x, int y, SDL_Renderer* renderPtr)
{
	//Set dimensions of object's sprites
	clip = { 0, 0, 32, 32 };

	//Set object position (specified as grid params)
	mPosX = x; mPosY = y;

	mTexture.setRenderer(renderPtr);
}

inline Object::~Object()
{
	//Deallocate stored texture to avoid memory leakage
	mTexture.free();
}
