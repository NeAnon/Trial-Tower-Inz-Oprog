#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

class WTexture
{
private:
    //The actual hardware texture being stored
    SDL_Texture* mTexture;

	//The pointer to the window renderer
	SDL_Renderer* mRendererPtr;

    //Dimensions
    int mWidth;
    int mHeight;

public:
    //Initializes variables
    WTexture();

    //Deallocates memory
    ~WTexture();

    //Loads image from specified path
    bool loadFromFile(std::string path);

    //Deallocates texture itself
    void free();

    //Renders texture at given point, gives options for clipping from a sprite sheet, rotation, or flipping (just in case)
    void render(int x, int y, SDL_Rect* clip = nullptr, double angle = 0.0, SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE);

    //Gets image dimensions
    int getWidth();
    int getHeight();

	//Set target renderer (required, since we have no global one here)
	void setRenderer(SDL_Renderer* renderPtr);
};

inline WTexture::WTexture() 
{
    //Initialize
    mTexture = nullptr;
	mRendererPtr = nullptr;
    mWidth = 0;
    mHeight = 0;
}

inline WTexture::~WTexture()
{
    //Deallocate stored texture to avoid memory leakage
    free();
}

inline bool WTexture::loadFromFile(std::string path)
{
	//Get rid of preexisting texture if one exists
	free();

	//Initialize final texture
	SDL_Texture* newTexture = nullptr;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());	//Convert std::string to const char*, 
															//necessary to load the image from path	

	if (loadedSurface == nullptr)	//Return an error message if image wasn't loaded
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(mRendererPtr, loadedSurface);
		if (newTexture == nullptr)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of any old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	mTexture = newTexture;
	return mTexture != nullptr;
}

inline void WTexture::free()
{
	//Free texture if it exists
	if (mTexture != nullptr)
	{
		//Destroy texture and re-initialize variables
		SDL_DestroyTexture(mTexture);
		mTexture = nullptr;
		mWidth = 0;
		mHeight = 0;
	}
}

inline void WTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if (clip != nullptr)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(mRendererPtr, mTexture, clip, &renderQuad, angle, center, flip);
}

inline int WTexture::getWidth()
{
	return mWidth;
}

inline int WTexture::getHeight()
{
	return mHeight;
}

inline void WTexture::setRenderer(SDL_Renderer* renderPtr)
{
	mRendererPtr = renderPtr;
}

