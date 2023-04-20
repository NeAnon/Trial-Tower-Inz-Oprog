//Base includes
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <vector>

//Wrapped texture class 
#include "WTexture.h"

//Macro for scaling
#define scaleX WTexture::getScaledX
#define scaleY WTexture::getScaledY


enum {
	SELECT_CENTER,		//0
	SELECT_UP,			//1
	SELECT_RIGHT,		//2
	SELECT_DOWN,		//3
	SELECT_LEFT,		//4
};

enum {
	OPT_START = 0,
	OPT_QUIT = 1
};

class Title {
	//Player texture and current sprite
	WTexture mTitle;
	WTexture mStartButton;
	WTexture mQuitButton;

	SDL_Rect titleClip;
	SDL_Rect startButtonClip;
	SDL_Rect quitButtonClip;

	int startX;
	int startY;

	int quitX;
	int quitY;

	int selected;
	int items;
public:
	Title();

	Title(SDL_Renderer* renderPtr);

	~Title();

	void loadTitleScreen() {
		//Load entity texture	
		if (!mTitle.loadFromFile("resources/title_screen.png"))
		{
			printf("Failed to load title texture! SDL_image Error: %s\n", IMG_GetError());
		}
		//titleClip.w = mTitle.getWidth(); titleClip.h = mTitle.getHeight();
		if (!mStartButton.loadFromFile("resources/start_button.png"))
		{
			printf("Failed to load start button texture! SDL_image Error: %s\n", IMG_GetError());
		}

		startX = WTexture::getGlobalLWidth() / 2 - mStartButton.getWidth() / 2; 
		startY = WTexture::getGlobalLHeight() / 2 - (mStartButton.getHeight() / 4 + 32);
		//startButtonClip.w = mStartButton.getWidth(); startButtonClip.h = mStartButton.getHeight();
		if (!mQuitButton.loadFromFile("resources/quit_button.png"))
		{
			printf("Failed to load quit button texture! SDL_image Error: %s\n", IMG_GetError());
		}
		quitX = WTexture::getGlobalLWidth() / 2 - mQuitButton.getWidth() / 2;
		quitY = WTexture::getGlobalLHeight() / 2 + (mQuitButton.getHeight() / 4 + 32);


		//quitButtonClip.w = mQuitButton.getWidth(); quitButtonClip.h = mQuitButton.getHeight();
	}
	void callButtonPos() {
		std::cout << "Width offset: " << WTexture::getGlobalXOffset() << "\n";
		std::cout << "Height offset: " << WTexture::getGlobalYOffset() << "\n";
		std::cout << "Scale: " << WTexture::getGlobalScale() << "\n";

		std::cout << "Start Button X: " << scaleX(startX) << " - " << scaleX(startX + mStartButton.getWidth()) << std::endl;
		std::cout << "Start Button Y: " << scaleY(startY) << " - " << scaleY(startY + mStartButton.getHeight()) << std::endl;
		std::cout << "Quit Button X: " << quitX << " - " << quitX + mQuitButton.getWidth() << std::endl;
		std::cout << "Quit Button Y: " << quitY << " - " << quitY + mQuitButton.getHeight() << std::endl;
	}
	/*void renderInternalButtonPos(SDL_Renderer* rptr) {
		SDL_SetRenderDrawColor(rptr, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_Rect tempStartRect = { startX, startY, mStartButton.getWidth(), mStartButton.getHeight() };
		SDL_Rect tempQuitRect = { quitX, quitY, mQuitButton.getWidth(), mQuitButton.getHeight() };
		SDL_RenderDrawRect(rptr, &tempStartRect);
		SDL_RenderDrawRect(rptr, &tempQuitRect);
	}*/


	int selectOpt(int dir, bool override = false) {
		if (!override) {
			if (selected == -1) {
				selected = 0;
				highlightSelection();
			}
			else {
				switch (dir) {
				case SELECT_CENTER:
					return selected;
				case SELECT_UP:
					selected = (selected - 1 < 0 ? items - 1 : selected - 1);
					break;
				case SELECT_DOWN:
					selected = (selected + 1 == items ? 0 : selected + 1);
					break;
				default:
					break;
				}
			}
			highlightSelection();
			return -1;
		}
		else {
			selected = dir;
			highlightSelection();
			return dir;
		}
	}

	void highlightSelection() {
		switch (selected) {
			case 0:
				startButtonClip.y = 96;
				quitButtonClip.y = 0;
				break;
			case 1:
				startButtonClip.y = 0;
				quitButtonClip.y = 96;
				break;
		}
	}
	

	bool withinStart(int m_X, int m_Y) {
		int scaledMouseX = scaleX(m_X);
		int scaledMouseY = scaleY(m_Y);

		std::cout << "(Start) Scaled X: " << scaledMouseX << "\tScaled Y: " << scaledMouseY << '\n';
		if (scaledMouseX < startX || scaledMouseX > startX + mStartButton.getWidth()) {
			//std::cout << "Outside Start in X axis!\n";
			return false;
		}
		if (scaledMouseY < startY || scaledMouseY > startY + mStartButton.getHeight()) {
			//std::cout << "Outside Start in Y axis!\n";
			return false;
		}
		//std::cout << "Inside Start!\n";
		return false;
		return true;
	}

	bool withinQuit(int m_X, int m_Y) {
		int scaledMouseX = round(m_X * WTexture::getGlobalScale()) - WTexture::getGlobalXOffset();
		int scaledMouseY = round(m_Y * WTexture::getGlobalScale()) - WTexture::getGlobalYOffset();

		std::cout << "(Quit) Scaled X: " << scaledMouseX << "\tScaled Y: " << scaledMouseY << '\n';
		if (scaledMouseX < quitX || scaledMouseX > quitX + mQuitButton.getWidth() ) {
			//std::cout << "Outside Quit in X axis!\n";
			return false;
		}
		if (scaledMouseY < quitY || scaledMouseY > quitY + mQuitButton.getHeight()) {
			//std::cout << "Outside Quit in Y axis!\n";
			return false;
		}
		//std::cout << "Inside Quit!\n";
		return false;
		return true;
	}

	void render() {
		mTitle.render(WTexture::getGlobalLHeight()/2 - mTitle.getWidth()/2, 64);
		mStartButton.render(startX, startY, &startButtonClip);
		mQuitButton.render(quitX, quitY, &quitButtonClip);
	}
};

Title::Title() {
	//titleClip = {0,0,0,0}			Will be possibly used for title animation
	startButtonClip = { 0, 0, 192, 96 };
	quitButtonClip = { 0, 0, 192, 96 };

	startX = 0; startY = 0; quitX = 0; quitY = 0;
	selected = -1; items = 2;
}

inline Title::Title(SDL_Renderer* renderPtr)
{
	mTitle.setRenderer(renderPtr);
	mStartButton.setRenderer(renderPtr);
	mQuitButton.setRenderer(renderPtr);

	startButtonClip = { 0, 0, 192, 96 };
	quitButtonClip = { 0, 0, 192, 96 };

	startX = 0; startY = 0; quitX = 0; quitY = 0;
	selected = -1; items = 2;

	loadTitleScreen();
}

inline Title::~Title()
{
}