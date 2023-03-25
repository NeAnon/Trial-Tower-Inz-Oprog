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

//Objects
#include "entity.h"
#include "tile.h"

//Level map (experimantal)
#include "levelMap.h"

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
		gWindow = SDL_CreateWindow("TrialTower v0.0.0.0.3", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
			Player player(2, 2, gRenderer);

			//Enemies
			Enemy* enemy1 = new Enemy(10, 5, gRenderer);
			Enemy* enemy2 = new Enemy(7, 7, gRenderer);

			std::vector< Entity* > enemyList;
			enemyList.resize(2);
			enemyList[0] = enemy1;
			enemyList[1] = enemy2;

			LevelMap lvlMap(SCREEN_WIDTH / TILE_SIZE, SCREEN_HEIGHT / TILE_SIZE);
			lvlMap.insertObj(new Wall(0, 3, gRenderer));
			lvlMap.insertObj(new Wall(1, 4, gRenderer));
			lvlMap.insertObj(new Wall(2, 3, gRenderer));
			lvlMap.insertObj(new Wall(3, 3, gRenderer));
			lvlMap.insertObj(new Wall(3, 1, gRenderer));
			lvlMap.insertObj(new Wall(3, 0, gRenderer));
			lvlMap.insertObj(new Wall(4, 5, gRenderer));
			lvlMap.insertObj(new Wall(7, 6, gRenderer));

			Portal* portal = new Portal(14, 10, gRenderer);
			lvlMap.insertObj(portal);

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
							player.move(DIRECTION_UP, lvlMap, enemyList, portal);
							break;

						case SDLK_DOWN:
							player.move(DIRECTION_DOWN, lvlMap, enemyList, portal);
							break;

						case SDLK_LEFT:
							player.move(DIRECTION_LEFT, lvlMap, enemyList, portal);
							break;

						case SDLK_RIGHT:
							player.move(DIRECTION_RIGHT, lvlMap, enemyList, portal);
							break;

						default:
							break;
						}
					}
                }

                //Clear screen
                SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
                SDL_RenderClear(gRenderer);

				for (int i = 0; i < enemyList.size(); i++) {
					if (enemyList[i] != nullptr) {
						enemyList[i]->render();
					}
				}

				if (portal->isFinished()) {
					std::cout << std::endl << "\t\tYOU'RE WINNER!!!" << std::endl;
					quit = true;
				}


				lvlMap.renderAll();

				player.render();
             
				//Update screen (must be after rendering everything prior!!!)
                SDL_RenderPresent(gRenderer);
			}
			if (enemyList[0] != nullptr) { delete enemyList[0]; }
			if (enemyList[1] != nullptr) { delete enemyList[1]; }
		}
	}

	//Free global resources and close SDL
	close();

	return 0;
}