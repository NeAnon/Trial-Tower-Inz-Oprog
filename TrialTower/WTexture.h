#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <cmath>

extern double upscale;
extern int w_offset;
extern int h_offset;

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

	static int LEVEL_WIDTH;
	static int LEVEL_HEIGHT;

	static int GAME_WIDTH;
	static int GAME_HEIGHT;

	static int WINDOW_WIDTH;
	static int WINDOW_HEIGHT;
	static double upscale;
	static int w_offset;
	static int h_offset;

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

	inline static void setGlobalLSize(int w, int h) { LEVEL_WIDTH = w; LEVEL_HEIGHT = h; GAME_WIDTH = LEVEL_WIDTH; GAME_HEIGHT = LEVEL_HEIGHT + 32; }
	inline static int getGlobalLWidth() { return LEVEL_WIDTH; }
	inline static int getGlobalLHeight() { return LEVEL_HEIGHT; }

	inline static void setGlobalWSize(int w, int h)	{ WINDOW_WIDTH = w; WINDOW_HEIGHT = h; }

	inline static void calculate_renderRect() {
		//Find stretch vals needed for both dimensions to fit screen, and choose the smallest to be the stretch val (letterboxing > loss of render space)
		double w_stretch = (WINDOW_WIDTH * 1.0) / GAME_WIDTH; double h_stretch = (WINDOW_HEIGHT * 1.0) / GAME_HEIGHT;
		upscale = (w_stretch <= h_stretch ? w_stretch : h_stretch);
		//std::cout << "Upscale value: " << upscale << "\n";

		//Use upscale value to find centering offset
		w_offset = (WINDOW_WIDTH - (GAME_WIDTH * upscale)) / 2; h_offset = (WINDOW_HEIGHT - (GAME_HEIGHT * upscale)) / 2;
		//std::cout << "Width offset: " << w_offset << "\nHeight offset: " << h_offset << std::endl;
	}

	inline static void callback_renderrect() {
		std::cout << "Scale: " << upscale << "\t\tWOffset: " << w_offset << "\t\t HOffset: " << h_offset << std::endl;
	}


	inline static int getGlobalXOffset() { return w_offset; }
	inline static int getGlobalYOffset() { return h_offset; }
	inline static double getGlobalScale() { return upscale; }
	inline static int getScaledX(int mx_) { return (std::round(mx_ * upscale) + w_offset); }
	inline static int getScaledY(int my_) { return (std::round(my_ * upscale) + h_offset); }

	inline static void outlineAll(SDL_Renderer * renderPtr)
	{
		SDL_SetRenderDrawColor(renderPtr, 0xFF, 0xFF, 0xFF, 0xFF);

		SDL_Rect levelOutline{ w_offset, h_offset, std::round(LEVEL_WIDTH * upscale), std::round(LEVEL_HEIGHT * upscale) };

		SDL_RenderDrawRect(renderPtr, &levelOutline);

		SDL_SetRenderDrawColor(renderPtr, 0x00, 0x00, 0x00, 0xFF);
	}

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
	//std::cout << "Upscale: " << upscale << "\t\tWOffset: " << w_offset << "\t\t HOffset: " << h_offset << std::endl;
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { std::round(x * upscale) + w_offset, std::round(y * upscale) + h_offset, std::round(mWidth * upscale), std::round(mHeight * upscale) };

	//Set clip rendering dimensions
	if (clip != nullptr)
	{
		renderQuad.w = std::round(clip->w * upscale);
		renderQuad.h = std::round(clip->h * upscale);
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

int WTexture::LEVEL_WIDTH;
int WTexture::LEVEL_HEIGHT;

int WTexture::GAME_WIDTH;
int WTexture::GAME_HEIGHT;

int WTexture::WINDOW_WIDTH;
int WTexture::WINDOW_HEIGHT;

double WTexture::upscale = 1;
int WTexture::w_offset = 0;
int WTexture::h_offset = 0;
