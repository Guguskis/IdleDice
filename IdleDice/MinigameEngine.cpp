#include "pch.h"
#include "MinigameEngine.h"



void MinigameEngine::DrawFrame() {
	for (int i = 0; i < mHeight; i++) {
		for (int j = 0; j < mWidth; j++) {
			if (i == 0 || j == 0 || i == mHeight - 1 || j == mWidth - 1) {
				InsertPixel(mY + i, mX + j, ' ', mFrameColor, col_noColor);
			}
		}
	}
}


void MinigameEngine::MinigameTest() {
	
}
void MinigameEngine::UpdateGraphics() {
	InsertArray(mY + 1, mX + 1, mGame);
}

MinigameEngine::MinigameEngine(
	int y, int x, int height, int width, 
	int frameColor, double tickTime, double tickSpeed,
	bool *gameEngineIsRunning)
{
	mY = y;
	mX = x;
	mHeight = height;
	mWidth = width;
	mFrameColor = frameColor;
	mTickTime = tickTime;
	mTickSpeed = tickSpeed;
	mGameEngineIsRunning = gameEngineIsRunning;

	//initializing game board
	Pixel pixel;
	pixel.symb = ' ';
	pixel.bgCol = col_black;
	pixel.fgCol = col_white;
	
	
	vector<Pixel> line(width-2, pixel);
	mGame.resize(height-2, line);
	
	//allow minigame execution after initializing parameters
	mIsRunning = true;
}
MinigameEngine::MinigameEngine(){
}
MinigameEngine::~MinigameEngine(){
}

