
#include "pch.h"
#include "MinigameLaser.h"


void MinigameLaser::GameControl(){
	while (mIsRunning && *mGameEngineIsRunning) {
		if (KeyPressed("Space")) {
			mMutex.lock();
			
			laserOn = !laserOn;

			for (int j = 0; j < mWidth - 2; j++) {
				//if laser is toggled on
				if (laserOn) {
					//burning objects and applying damage
					if (mGame[mHeight - 4][j].symb == badObject)
						lives--;
					else if (mGame[mHeight - 4][j].symb == goodObject)
						lives++;

					mGame[mHeight - 4][j].symb = ' ';
					mGame[mHeight - 4][j].bgCol= laserCol;
				}
				else {
					if (mGame[mHeight - 4][j].symb == ' ')
						mGame[mHeight - 4][j].bgCol = col_black;
				}
			}

			UpdateGraphics();

			if (lives <= 0)
				mIsRunning = false;

			mMutex.unlock();
		}
	}
}

void MinigameLaser::GameLogic() {
	int effHeight = mHeight - 2;
	int effWidth = mWidth - 2;
	DrawFrame();

	//drawing laser for first time
	if (laserOn) {
		mMutex.lock();
		for (int i = 0; i < effWidth; i++) {
			mGame[effHeight - 2][i].bgCol = laserCol;
		}
		mMutex.unlock();
	}


	while (mIsRunning && *mGameEngineIsRunning) {
		mMutex.lock();

		//moving objects down by one
		for (int i = effHeight - 1; i >= 0; i--) {
			for (int j = 0; j < effWidth; j++) {
				//removing last line and applying damage
				if (i == effHeight - 1 && mGame[i][j].symb != ' ') {
					//apply damage
					if (mGame[i][j].symb == goodObject) {
						lives-=2;
					}
					//removing
					mGame[i][j].symb = ' ';
					mGame[i][j].bgCol = col_black;
				}
				else {
					//transfering objects down by one line
					if (mGame[i][j].symb != ' ') {
						mGame[i + 1][j] = mGame[i][j];
						mGame[i][j].symb = ' ';
						mGame[i][j].bgCol = col_black;
						//applying laser color
						if (i == effHeight - 2 && laserOn) {
							mGame[i][j].bgCol = laserCol;
						}
					}
				}
			}
		}

		//handling laser burning
		if (laserOn) {
			for (int i = 0; i < effWidth; i++) {
				if (mGame[effHeight - 2][i].symb == badObject)
					lives--;
				mGame[effHeight - 2][i].symb = ' ';
				mGame[effHeight - 2][i].bgCol = laserCol;
			}
		}

		//generating objects
		if (mTicksPassed%obstacleFrequency == 0) {
			Pixel object;
			if (rand() % 2 == 0) {
				object.symb = badObject;
				object.bgCol = badCol;
			}
			else {
				object.symb = goodObject;
				object.bgCol = goodCol;
			}
			mGame[0][rand() % effWidth] = object;
		}

		//inserting lives info
		string text = "Lives ";
		if (lives > 0) {
			text += string(lives / 2, ':');
			text += string(lives % 2, '.');
		}
		InsertLine(mY, mX, text + "  ", col_noColor, col_black);

		if (lives <= 0)
			mIsRunning = false;

		UpdateGraphics();

		mMutex.unlock();

		if(mTicksPassed%obstacleFrequency==0)
			mTickTime *= mTickSpeed;

		Sleep(max(mTickTime, minTickTime));
		mTicksPassed++;
	}
}

void MinigameLaser::Run(){
	//delay game start
	Starter();
	//display controls
	string goal = "Goal:   you control laser.  Burn red, don't burn green.";
	string controls = "Controls: space";

	DrawGuide(goal, controls, 5);

	thread inputWorker(&MinigameLaser::GameControl, this);
	thread gameWorker(&MinigameLaser::GameLogic, this);

	inputWorker.join();
	gameWorker.join();

	GameOverAnimation();
	*mGameEngineIsRunning = false;
}

MinigameLaser::MinigameLaser(
	int y, int x, int height, int width,
	int frameColor, double tickTime, double tickSpeed,
	bool *gameEngineIsRunning)
	: MinigameEngine(y, x, height, width, frameColor, tickTime, tickSpeed, gameEngineIsRunning)
{
	minTickTime = 100;
	mStartTimer = 40;
}

MinigameLaser::MinigameLaser(){
}
MinigameLaser::~MinigameLaser(){
}
