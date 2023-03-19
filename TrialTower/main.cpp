//Base includes
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <vector>

//debugging
#include <iostream>
#include "debugFuncs.h"

//Wrapped texture class 
#include "WTexture.h"

//Player character
#include "player.h"

//Walls
#include "wall.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window the game will be rendering to
SDL_Window* gWindow = NULL;

//The window renderer (resposible for drawing everything)
SDL_Renderer* gRenderer = NULL;

bool init()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        //Create window
		gWindow = SDL_CreateWindow("TrialTower v0.0.0.0.2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
    }

    return success;
}

bool loadMedia()
{
    //Loading success flag
    bool success = true;

	//Here global sprites would be loaded, however we (most likely) want each class to handle its own sprite

    return success;
}

void close()
{
    //Destroy window	
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* args[])
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{	
		//Load media (if any)
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//Player character
			Player player;
			player.setPlayerRenderer(gRenderer);
			player.loadPlayerMedia();

			Wall sampleWall;
			sampleWall.setRenderer(gRenderer);
			sampleWall.loadMedia();

			std::vector< std::vector<int> > wallMap;
			wallMap.resize(SCREEN_WIDTH / TILE_SIZE);
			for (int i = 0; i < wallMap.size(); i++) {
				wallMap[i].resize(SCREEN_HEIGHT / TILE_SIZE);
			}

			for (int i = 0; i < wallMap.size(); i++) {
				for (int j = 0; j < wallMap[i].size(); j++) {
					wallMap[i][j] = 0;
				}
			}
			wallMap[3][3] = 1;
			wallMap[6][7] = 1;
			wallMap[4][5] = 1;

			//std::wcout << "---------------path------------------" << std::endl;
			//std::wcout << ExePath() << std::endl;
			//std::wcout << "---------------path------------------" << std::endl;

			//While the application is running...
            while (!quit)
            {
                //Handle events on queue if there are any
                while (SDL_PollEvent(&e) != 0)
                {
                    //User presses the X button
                    if (e.type == SDL_QUIT)
                    {
                        quit = true;
                    }
					//User presses a key
					else if (e.type == SDL_KEYDOWN)
					{
						//Move player based on key press
						switch (e.key.keysym.sym)
						{
						case SDLK_UP:
							player.move(DIRECTION_UP, wallMap);
							break;

						case SDLK_DOWN:
							player.move(DIRECTION_DOWN, wallMap);
							break;

						case SDLK_LEFT:
							player.move(DIRECTION_LEFT, wallMap);
							break;

						case SDLK_RIGHT:
							player.move(DIRECTION_RIGHT, wallMap);
							break;

						default:
							break;
						}
					}
                }

                //Clear screen
                SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
                SDL_RenderClear(gRenderer);

				for (int i = 0; i < wallMap.size(); i++) {
					for (int j = 0; j < wallMap[i].size(); j++) {
						if (wallMap[i][j] > 0) { sampleWall.render(i, j); }
					}
				}

				player.render();
             
				//Update screen (must be after rendering everything prior!!!)
                SDL_RenderPresent(gRenderer);
			}
		}
	}

	//Free global resources and close SDL
	close();

	return 0;
}