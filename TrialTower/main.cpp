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
#include "player.h"

//Mapping
#include "levelMap.h"
#include "enemy.h"
#include "levelParser.h"

int GAME_WIDTH = 640;
int GAME_HEIGHT = 480;

int WINDOW_WIDTH = GAME_WIDTH;
int WINDOW_HEIGHT = GAME_HEIGHT;

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
		gWindow = SDL_CreateWindow("TrialTower v0.0.0.0.5", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, GAME_WIDTH, GAME_HEIGHT, SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
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

			//Set level and window sizes for WTexture to use
			WTexture::setGlobalLSize(GAME_WIDTH, GAME_HEIGHT);
			
			WTexture::setGlobalWSize(WINDOW_WIDTH, WINDOW_HEIGHT);

			WTexture::calculate_renderRect();

			//Player character
			Player player(2, 2, gRenderer);

			enemyList eList;
			eList.setRenderer(gRenderer);

			LevelMap lvlMap;

			parseLevel("levels/sampleLevel.lvl", player, lvlMap, eList, gRenderer);

			eList.reloadMedia();
			
			//Enemies
		//	Enemy* enemy1 = new Enemy(10, 5, gRenderer);
		//	Enemy* enemy2 = new Enemy(7, 7, gRenderer);
			/*
			eList.addEnemy(10, 5);
			eList.addEnemy(7, 7, "Slide", 1);
			eList.addEnemy(9, 11, "Wander");
			eList.addEnemy(0, 0, "Slide", DIRECTION_LEFT);
			eList.addEnemy(1, 12, "Wander");


		//	enemyList.resize(2);
		//	enemyList[0] = enemy1;
		//	enemyList[1] = enemy2;
			lvlMap.insertObj(new Wall(0, 3, gRenderer));
			lvlMap.insertObj(new Wall(1, 4, gRenderer));
			lvlMap.insertObj(new Wall(2, 3, gRenderer));
			lvlMap.insertObj(new Wall(3, 3, gRenderer));
			lvlMap.insertObj(new Wall(3, 1, gRenderer));
			lvlMap.insertObj(new Wall(3, 0, gRenderer));
			lvlMap.insertObj(new Wall(4, 5, gRenderer));
			lvlMap.insertObj(new Wall(7, 6, gRenderer));
			lvlMap.insertPortal(new Portal(1, 1, gRenderer));
			*/
			//std::wcout << "---------------path------------------" << std::endl;
			//std::wcout << ExePath() << std::endl;
			//std::wcout << "---------------path------------------" << std::endl;
			

			Portal* portal = lvlMap.getPortalPtr();

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
					//User tries to resize the window
					else if (e.type == SDL_WINDOWEVENT) {
						if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
							SDL_GetWindowSize(gWindow, &WINDOW_WIDTH, &WINDOW_HEIGHT);
							std::cout << "New width: " << WINDOW_WIDTH << "\nNew height: " << WINDOW_HEIGHT << std::endl;
							WTexture::setGlobalWSize(WINDOW_WIDTH, WINDOW_HEIGHT);
							WTexture::calculate_renderRect();

							//Debug: callback the scale and offset to the renderer
							WTexture::callback_renderrect();
						}
					}
					//User presses a key
					else if (e.type == SDL_KEYDOWN)
					{
						int pDamageAcc = 0;
						//Move player based on key press
						switch (e.key.keysym.sym)
						{
						case SDLK_UP:
							player.move(DIRECTION_UP, lvlMap, eList, portal);
							eList.moveAll(lvlMap, pDamageAcc, player.getX(), player.getY());
							player.hurt(pDamageAcc);
							if (pDamageAcc > 0) { std::cout << "Player hit, hp remaining: " << player.getHP() << '\n'; }
							break;

						case SDLK_DOWN:
							player.move(DIRECTION_DOWN, lvlMap, eList, portal);
							eList.moveAll(lvlMap, pDamageAcc, player.getX(), player.getY());
							player.hurt(pDamageAcc);
							if (pDamageAcc > 0) { std::cout << "Player hit, hp remaining: " << player.getHP() << '\n'; }
							break;

						case SDLK_LEFT:
							player.move(DIRECTION_LEFT, lvlMap, eList, portal);
							eList.moveAll(lvlMap, pDamageAcc, player.getX(), player.getY());
							player.hurt(pDamageAcc);
							if (pDamageAcc > 0) { std::cout << "Player hit, hp remaining: " << player.getHP() << '\n'; }
							break;

						case SDLK_RIGHT:
							player.move(DIRECTION_RIGHT, lvlMap, eList, portal);
							eList.moveAll(lvlMap, pDamageAcc, player.getX(), player.getY());
							player.hurt(pDamageAcc);
							if (pDamageAcc > 0) { std::cout << "Player hit, hp remaining: " << player.getHP() << '\n'; }
							break;

						default:
							break;
						}
					}
                }

                //Clear screen
                SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
                SDL_RenderClear(gRenderer);


				if (portal->isFinished()) {
					std::cout << std::endl << "\t\tYOU'RE WINNER!!!" << std::endl;
					quit = true;
				}

				//Render the map
				lvlMap.renderAll();

				//Render all remaining enemies
				eList.renderAll();
				
				if (player.getX() == portal->getX() && player.getY() == portal->getY() && quit == false) {
					portal->renderText(portal->getX(), portal->getY(), GAME_WIDTH, GAME_HEIGHT);
				}

				//Render the player
				if (player.isAlive()) {
					player.render();
				}
				else {
					quit = true;
					std::cout << std::endl << "\t\tYOU LOSE!!!" << std::endl;
				}

				//Outline everything that's rendered
				WTexture::outlineAll(gRenderer);

				//Update screen (must be after rendering everything prior!!!)
                SDL_RenderPresent(gRenderer);
			}
		}
	}

	//Free global resources and close SDL
	close();

	return 0;
}