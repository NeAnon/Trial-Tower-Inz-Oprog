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

const int CHAR_W = 24;
const int CHAR_H = 36;
const int MARGIN = 8; const int TITLE_OFFSET = 52;
const int NEW_NAME_X_OFFSET = 104;
const int NEW_NAME_Y_OFFSET = 254;

enum {
	SELECT_CENTER,		//0
	SELECT_UP,			//1
	SELECT_RIGHT,		//2
	SELECT_DOWN,		//3
	SELECT_LEFT,		//4
};

enum {
	OPT_START = 0,
	OPT_HI_SCORE,
	OPT_QUIT
};

class Title {
	//Player texture and current sprite
	WTexture mTitle;
	WTexture mStartButton;
	WTexture mQuitButton;
	WTexture mHiScoreButton;

	SDL_Rect titleClip;
	SDL_Rect startButtonClip;
	SDL_Rect quitButtonClip;
	SDL_Rect hiScoreClip;

	int startX;
	int startY;

	int HSX;
	int HSY;
	
	int quitX;
	int quitY;

	int selected;
	int selectLevel;
	int items;

	//All leaderboard handling
	WTexture mLeaderboardText;
	WTexture mLeaderboardFrame;
	int HSFrameX; int HSFrameY;
	SDL_Rect selCharClip;
	std::vector<std::string> names;
	std::vector<std::string> scores;
	
	//Game over screen
	WTexture mGameOver;
	SDL_Rect gameOverClip;
	int GOX; int GOY;
	int minScore; bool newScore;
	
	bool single_use_switch;


	void initLeaderboard() {
		names.resize(10);
		scores.resize(10);
		int currScores = 0;

		std::ifstream leaderboard_read{ "leaderboard.score" };

		while (!leaderboard_read.eof() && currScores < 10) {
			leaderboard_read >> names[currScores];
			leaderboard_read >> scores[currScores];

			//std::cout << "At index: " << currScores << " Name: " << names[currScores] << ", Score: " << scores[currScores] << "\n";

			if (!currScores && scores[currScores] != "") { minScore = std::stoi(scores[currScores]); }
			else { if (scores[currScores] != "" && minScore > std::stoi(scores[currScores])) { minScore = std::stoi(scores[currScores]); } }
			currScores++;
		}
		leaderboard_read.close();
		//std::cout << "Min score: " << minScore << '\n';

		if (scores[9] == "") {	minScore = 0; }

		//std::cout << "Leaderboard closed.\n";
		//for (int i = 0; i < 10; i++) {std::cout << names[i] << '\t' << scores[i] << '\n';}
	}
	
	int calcButtonY(int menuPos) {
		int w = WTexture::getGlobalLHeight();
		int y = mTitle.getHeight();
		int z = startButtonClip.h;	//Same for every menu option
		int x = ((w - (y + 64)) - (z * items)) / (items + 1);
		//Offset from the title + (amount of x-spaces) + (amount of other buttons)
		return y + 64 + (x * (menuPos + 1)) + (z * menuPos);
	}
		
	void renderNameScore(std::string& name, std::string& score, int pos) {
		//render name
		for (int i = 0; i < name.size(); i++) {
			renderChar(HSFrameX + MARGIN + (i * CHAR_W), HSFrameY + MARGIN + TITLE_OFFSET + (pos * (CHAR_H + 4)), name[i]);
		}
		int scoreOffset = 16 - score.size();
		//render score
		for (int i = 0; i < score.size(); i++) {
			renderChar(HSFrameX + MARGIN + ((scoreOffset + i) * CHAR_W), HSFrameY + MARGIN + TITLE_OFFSET + (pos * (CHAR_H + 4)), score[i]);
		}
		if(name.size() && score.size()){
			renderChar(HSFrameX + MARGIN + (17 * CHAR_W), HSFrameY + MARGIN + TITLE_OFFSET + (pos * (CHAR_H + 4)), 'G');
		}
	}


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
		startY = calcButtonY(0);
		
		if (!mHiScoreButton.loadFromFile("resources/hi-score_button.png"))
		{
			printf("Failed to load start button texture! SDL_image Error: %s\n", IMG_GetError());
		}
		

		HSX = WTexture::getGlobalLWidth() / 2 - mHiScoreButton.getWidth() / 2; 
		HSY = calcButtonY(1);
		//startButtonClip.w = mStartButton.getWidth(); startButtonClip.h = mStartButton.getHeight();
		if (!mQuitButton.loadFromFile("resources/quit_button.png"))
		{
			printf("Failed to load quit button texture! SDL_image Error: %s\n", IMG_GetError());
		}
		quitX = WTexture::getGlobalLWidth() / 2 - mQuitButton.getWidth() / 2;
		quitY = calcButtonY(2);
		
		if (!mLeaderboardText.loadFromFile("resources/font7x12_6x6.png"))
		{
			printf("Failed to load font texture! SDL_image Error: %s\n", IMG_GetError());
		}
		if (!mLeaderboardFrame.loadFromFile("resources/hi-score_frame.png"))
		{
			printf("Failed to load hi-scoreframe texture! SDL_image Error: %s\n", IMG_GetError());
		}
		HSFrameX = (WTexture::getGlobalLWidth() / 2) - (mLeaderboardFrame.getWidth() / 2);
		HSFrameY = (WTexture::getGlobalLHeight() / 2) - (mLeaderboardFrame.getHeight() / 2);

		if (!mGameOver.loadFromFile("resources/gameover.png"))
		{
			printf("Failed to load game end texture! SDL_image Error: %s\n", IMG_GetError());
		}
		
		GOX = (WTexture::getGlobalLWidth() / 2) - (mGameOver.getWidth() / 2);
		GOY = (WTexture::getGlobalLHeight() / 2) - (mGameOver.getHeight() / 2);
		gameOverClip.w = mGameOver.getWidth();
		gameOverClip.h = mGameOver.getHeight();

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

	int getSLvl() { return selectLevel; }
	void incSelLvl() { selectLevel++; }
	void decSelLvl() { selectLevel--; }

	int selectOpt(int dir, bool override = false) {
		if (!override) {
			if (selected == -1) {
				selected = 0;
				highlightSelection();
			}
			else {
				if(!getSLvl()){
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
			}
			highlightSelection();
			return -1;
		}
		else {
			if (!getSLvl()) { selected = dir;  }
			highlightSelection();
			return dir;
		}
	}

	void highlightSelection() {
		switch (selected) {
			case 0:
				startButtonClip.y = 96;
				hiScoreClip.y = 0;
				quitButtonClip.y = 0;
				break;
			case 1:
				startButtonClip.y = 0;
				hiScoreClip.y = 96;
				quitButtonClip.y = 0;
				break;
			case 2:
				startButtonClip.y = 0;
				hiScoreClip.y = 0;
				quitButtonClip.y = 96;
				break;
		}
	}
	
	//DEPRECATED MOUSE HANDLING FUNCS
	/*
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
	*/

	void render() {
		if(selectLevel == 0)
		{
			mTitle.render(WTexture::getGlobalLHeight() / 2 - mTitle.getWidth() / 2, 64);
			mStartButton.render(startX, startY, &startButtonClip);
			mHiScoreButton.render(HSX, HSY, &hiScoreClip);
			mQuitButton.render(quitX, quitY, &quitButtonClip);
		}
		else if (selectLevel == 1 && selected == 1) {
			mLeaderboardFrame.render(HSFrameX, HSFrameY);
			for (int i = 0; i < 10; i++) {
				renderNameScore(names[i], scores[i], i);
			}
		}
	}

	void renderChar(int x, int y, char c) {
		if (c >= 'A' && c <= 'Z') {
			selCharClip.x = ((c - 'A')%6) * CHAR_W;
			selCharClip.y = ((c - 'A') / 6) * CHAR_H;
			mLeaderboardText.render(x, y, &selCharClip);
		}
		if (c >= '0' && c <= '9') {
			selCharClip.x = ((c - '0' + 2) % 6) * CHAR_W;
			selCharClip.y = ((c - '0' + 26) / 6) * CHAR_H;
			mLeaderboardText.render(x, y, &selCharClip);
		}
	}

	void checkScore(int playerScore) {
		if (playerScore > minScore) { newScore = true; }
		else { gameOverClip.h = 80; }
	}

	void renderGameOver(std::string &name) {
		if (single_use_switch) {
			single_use_switch = false;
			std::cout << "GOX: " << GOX << " \tGOY: " << GOY << "\t\n";
		}
		mGameOver.render(GOX, GOY-36, &gameOverClip);
		if(newScore){
			for (int i = 0; i < name.size(); i++) {
				renderChar(GOX + NEW_NAME_X_OFFSET + (CHAR_W * i), GOX + NEW_NAME_Y_OFFSET, name[i]);
			}
		}
	}

	void saveScores(std::string playername, int newHighScore) {
		checkScore(newHighScore);
		if(newScore)
		{
			int index = 9;
			names[9] = playername;
			scores[9] = std::to_string(newHighScore);
			
			while (index > 0 && (scores[index-1] == "" || newHighScore > std::stoi(scores[index - 1]))) {
				std::swap(names[index], names[index - 1]);
				std::swap(scores[index], scores[index - 1]); 
				index--;
			}
			std::cout << "Index " << index << " has name " << names[index] << " and score " << scores[index] << '\n';
			std::cout << "starting write\n";
			//Rewrite leaderboards from the ground up based on vector
			std::ofstream newfile;
			newfile.open("leaderboard.score", std::ios::out | std::ios::trunc );

			for(int i = 0; i < names.size(); i++){
				std::cout << "Writing score " << i + 1 << '\n';
				newfile << names[i] << ' ' << scores[i] << '\n';
			}

			newfile.close();
			std::cout << "file closed\n\n\n";
		}
	}

};

Title::Title() {
	titleClip = { 0,0,0,0 };			//Will be possibly used for title animation
	startButtonClip = { 0, 0, 192, 96 };
	hiScoreClip = { 0, 0, 192, 96 };
	quitButtonClip = { 0, 0, 192, 96 };
	selCharClip = { 0, 0, CHAR_W, CHAR_H };

	startX = 0; startY = 0; HSX = 0; HSY = 0; quitX = 0; quitY = 0;
	selected = -1; selectLevel = 0; items = 3;
}

inline Title::Title(SDL_Renderer* renderPtr)
{
	mTitle.setRenderer(renderPtr);
	mStartButton.setRenderer(renderPtr);
	mHiScoreButton.setRenderer(renderPtr);
	mQuitButton.setRenderer(renderPtr);
	mLeaderboardText.setRenderer(renderPtr);
	mLeaderboardFrame.setRenderer(renderPtr);
	mGameOver.setRenderer(renderPtr);

	startButtonClip = { 0, 0, 192, 96 };
	hiScoreClip = { 0, 0, 192, 96 };
	quitButtonClip = { 0, 0, 192, 96 };
	selCharClip = { 0, 0, CHAR_W, CHAR_H };
	gameOverClip = { 0,0,0,0 };

	startX = 0; startY = 0; HSX = 0; HSY = 0; quitX = 0; quitY = 0; GOX = 0; GOY = 0;
	selected = -1; selectLevel = 0; items = 3;
	
	HSFrameX = 0; HSFrameY = 0;
	minScore = 0; newScore = false;

	single_use_switch = true;

	loadTitleScreen();
	initLeaderboard();
}

inline Title::~Title()
{
}