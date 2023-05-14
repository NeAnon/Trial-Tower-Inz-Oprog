//Base includes
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <list>

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

//Title
#include "title.h"

int GAME_WIDTH = 640;
int GAME_HEIGHT = 512;

int WINDOW_WIDTH = GAME_WIDTH;
int WINDOW_HEIGHT = GAME_HEIGHT;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Loads the sample level from path
void loadLevel(std::string levelPath, Player& player, LevelMap& lvlMap, enemyList& eList, InventoryList &invList, Portal*& portal);

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

void loadLevel(std::string levelPath, Player &player, LevelMap &lvlMap, enemyList &eList, InventoryList &invlist, Portal*& portal) {
	parseLevel(levelPath, player, lvlMap, eList, invlist, gRenderer);

	//Set level size to level dims
	WTexture::setGlobalLSize(lvlMap.getXSize() * 32, lvlMap.getYSize() * 32);
	WTexture::calculate_renderRect();

	//reload media for selected enemiesS
	eList.reloadMedia();

	portal = lvlMap.getPortalPtr();
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
			bool paused = false;

			bool inLevel = false;
			std::vector<std::string> lvlList;
			int lvCounter = 0;

			lvlList.reserve(100);
			lvlList.push_back("levels/introLevel.lvl");
			lvlList.push_back("levels/sampleLevel.lvl");
			lvlList.push_back("levels/sampleLevel2.lvl");

			//Set level and window sizes for WTexture to use
			WTexture::setGlobalLSize(WINDOW_WIDTH, WINDOW_WIDTH);
			
			WTexture::setGlobalWSize(WINDOW_WIDTH, WINDOW_HEIGHT);
			 
			WTexture::calculate_renderRect();
			preloadItems(gRenderer);

			Title titleScreen(gRenderer);

			//Player character
			Player player(0, 0, gRenderer);

			enemyList eList;
			eList.setRenderer(gRenderer);
			LevelMap lvlMap;
			Portal* portal = new Portal(0, 0, gRenderer);
			InventoryList allItems;
			
			Interface HUD(gRenderer);
			HUD.loadInterface();

			if (inLevel) {
				delete portal;
				loadLevel(lvlList[lvCounter], player, lvlMap, eList, allItems, portal);
			}

			Uint8 testvar = 0;
			SDL_Rect testrect = { (WTexture::getGlobalLWidth() / 2) - 32, WTexture::getGlobalLHeight() - 24, 64, 16 };

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
			
			int mx_ = 0;
			int my_ = 0;
			

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
					//Checking if the user is in the game
					if (inLevel) {
						//Checking if the user has paused the game
						if (paused) {
							//If so, handle inventory (?)
							if (e.type == SDL_KEYDOWN)
							{
								switch (e.key.keysym.sym)
								{
								case SDLK_ESCAPE:
									paused = false;
									break;
								default:
									break;
								}
							}
						}
						else
						{
							//Otherwise, continue gameplay
							if (e.type == SDL_KEYDOWN)
							{
								//Accumulated damage (player)
								int pDamageAcc = 0;
								//Move player based on key press
								switch (e.key.keysym.sym)
								{
								case SDLK_UP:
									lvlMap.updateTiles();
									player.move(DIRECTION_UP, lvlMap, eList, portal);
									eList.moveAll(lvlMap, pDamageAcc, player.getX(), player.getY());
									lvlMap.activateTrap(player.getX(), player.getY(), pDamageAcc);
									player.hurt(pDamageAcc);
									if (pDamageAcc > 0) { std::cout << "Player hit, hp remaining: " << player.getHP() << '\n'; }
									break;

								case SDLK_DOWN:
									lvlMap.updateTiles();
									player.move(DIRECTION_DOWN, lvlMap, eList, portal);
									eList.moveAll(lvlMap, pDamageAcc, player.getX(), player.getY());
									lvlMap.activateTrap(player.getX(), player.getY(), pDamageAcc);
									player.hurt(pDamageAcc);
									if (pDamageAcc > 0) { std::cout << "Player hit, hp remaining: " << player.getHP() << '\n'; }
									break;

								case SDLK_LEFT:
									lvlMap.updateTiles();
									player.move(DIRECTION_LEFT, lvlMap, eList, portal);
									eList.moveAll(lvlMap, pDamageAcc, player.getX(), player.getY());
									lvlMap.activateTrap(player.getX(), player.getY(), pDamageAcc);
									player.hurt(pDamageAcc);
									if (pDamageAcc > 0) { std::cout << "Player hit, hp remaining: " << player.getHP() << '\n'; }
									break;
								case SDLK_RIGHT:
									lvlMap.updateTiles();
									player.move(DIRECTION_RIGHT, lvlMap, eList, portal);
									eList.moveAll(lvlMap, pDamageAcc, player.getX(), player.getY());
									lvlMap.activateTrap(player.getX(), player.getY(), pDamageAcc);
									player.hurt(pDamageAcc);
									if (pDamageAcc > 0) { std::cout << "Player hit, hp remaining: " << player.getHP() << '\n'; }
									break;
								case SDLK_ESCAPE:
									paused = true;
									break;
								default:
									break;
								}
								//player.addMoney(-player.getMoney());	player.addMoney(mt() % 1000000);
							}
						}
					}
					else {
						int action = -1;
						if (e.type == SDL_KEYDOWN) {
							switch (e.key.keysym.sym) {
							case SDLK_SPACE:
								action = titleScreen.selectOpt(SELECT_CENTER);
								break;
							case SDLK_RETURN:
								action = titleScreen.selectOpt(SELECT_CENTER);
								break;
							case SDLK_ESCAPE:
								action = titleScreen.selectOpt(OPT_QUIT, true);
								break;
							case SDLK_UP:
								action = titleScreen.selectOpt(SELECT_UP);
								break;
							case SDLK_DOWN:
								action = titleScreen.selectOpt(SELECT_DOWN);
								break;
							}
						}
						switch (action) {
						case OPT_START:
							delete portal;
							loadLevel(lvlList[lvCounter], player, lvlMap, eList, allItems, portal);
							inLevel = true;
							break;
						case OPT_QUIT:
							quit = true;
						}
					}
				}

				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
				SDL_RenderClear(gRenderer);

				if (inLevel) {
					if (inLevel && portal->isFinished()) {
						lvCounter++; 
						if (lvCounter < lvlList.size()) {
							loadLevel(lvlList[lvCounter], player, lvlMap, eList, allItems, portal);
						}
						else {
							std::cout << std::endl << "\t\tYOU'RE WINNER!!!" << std::endl;
							quit = true;
						}
					}

					//Render the map
					lvlMap.renderAll();

					//Render all remaining enemies
					eList.renderAll();

					allItems.render();

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
					
					HUD.render(player, paused);

					//Outline everything that's rendered
					WTexture::outlineAll(gRenderer);
				}
				else {
					titleScreen.render();
				}

				/*
				* Code for a decreasing health bar that turns redder as health is drained:
				* 
				SDL_SetRenderDrawColor(gRenderer, 255 - testvar, testvar, 0, SDL_ALPHA_OPAQUE);
				//SDL_RenderDrawRect(gRenderer, &testrect);
				SDL_RenderFillRect(gRenderer, &testrect);
				testvar--;
				testrect.w = std::ceil((testvar * 1.0 / 255) * 64);
				*/

				if (paused) {
				}

				//Update screen (must be after rendering everything prior!!!)
                SDL_RenderPresent(gRenderer);
			}
		}
	}

	//Free global resources and close SDL
	close();

	return 0;
}