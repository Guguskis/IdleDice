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
void MinigameEngine::DrawGuide(string goal, string controls, int timer){
	DrawFrame();
	InsertTextBox(mY + 1, mX + 1, goal, mHeight - 2, mWidth - 2, col_black, col_white);
	
	InsertTextBox(mY + 2+(goal.length()/(mWidth-2))+1, mX + 1, controls, mHeight - 2- (goal.length() / (mWidth - 2)) - 2, mWidth - 2, col_black, col_white);

	for (int i = 0; i < timer; i++) {
		string number = to_string(timer - i) + " ";
		int numberYPoz = (goal.length() / (mWidth - 2)) + (controls.length() / (mWidth - 2)) + 5;
		InsertLine(min(mY + numberYPoz, mY+mHeight-2), mX + (mWidth - 2) / 2, number, col_black, col_white);
		if(*mGameEngineIsRunning)
			Sleep(1000);
	}
}

void MinigameEngine::GameOverAnimation(){
	for (int i = 0; i < 10; i++) {
		if (i % 2 == 0) mFrameColor = col_red;
		else mFrameColor = col_darkRed;

		DrawFrame();
		Sleep(150);
	}
}

void MinigameEngine::Starter(){
	auto start = chrono::steady_clock::now();
	auto end = chrono::steady_clock::now();
	auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);


	while (elapsed.count() < 1000 * mStartTimer && *mGameEngineIsRunning) {
		end = chrono::steady_clock::now();
		elapsed = chrono::duration_cast<chrono::milliseconds> (end - start);
		Sleep(100);
	}
	
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

