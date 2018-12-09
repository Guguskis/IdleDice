#include "pch.h"
#include "MinigameCars.h"









void MinigameCars::GameControl() {
#define car(x) mGame[mHeight-3][x]
	Pixel empty;
	empty.symb = ' ';
	empty.bgCol = col_black;
	empty.fgCol = col_black;


	while (mIsRunning && *mGameEngineIsRunning) {
		if (KeyPressed("Left")) {
			lock_guard<mutex> lock(mMutex);
			if (carX > 0) {
				//check for collision
				if (car(carX-1).symb == 'H') {
					mIsRunning = false;
					car(carX - 1).symb = '?';
					car(carX - 1).bgCol = col_darkYellow;
					car(carX - 1).fgCol = col_yellow;
				}
				else {
					car(carX - 1).symb = '^';
					car(carX - 1).bgCol = col_darkGreen;
					car(carX - 1).fgCol = col_green;
				}

				car(carX) = empty;

				carX--;
				UpdateGraphics();
			}

		}
		else if (KeyPressed("Right")) {
			lock_guard<mutex> lock(mMutex);
			if (carX < mWidth - 3) {
				//check for collision
				if (car(carX + 1).symb == 'H') {
					mIsRunning = false;
					car(carX + 1).symb = '?';
					car(carX + 1).bgCol = col_darkYellow;
					car(carX + 1).fgCol = col_yellow;
				}
				else {
					car(carX + 1).symb = '^';
					car(carX + 1).bgCol = col_darkGreen;
					car(carX + 1).fgCol = col_green;
				}

				car(carX) = empty;

				carX++;
				UpdateGraphics();
			}

		}
		Sleep(1);
	}
#undef car(x)
}

void MinigameCars::GameLogic() {
	//initializing effective dimensions
	int effHeight = mHeight - 2;
	int effWidth = mWidth - 2;
	DrawFrame();

	while (mIsRunning && *mGameEngineIsRunning) {
		mMutex.lock();

		for (int i = effHeight - 1; i >= 0; i--) {
			for (int j = 0; j < effWidth; j++) {
				if (i == effHeight - 1 && j == carX) {
				//drawing car
					Pixel car;
					car.symb = '^';
					car.bgCol = col_darkGreen;
					car.fgCol = col_green;
					mGame[i][j] = car;
				}
				else if (i == effHeight - 1) {
				//removing last line of obstacles
					Pixel car;
					car.symb = ' ';
					car.bgCol = col_black;
					car.fgCol = col_black;
					mGame[i][j] = car;
				}
				else {
				//transfering obstacles down by one line
					if (mGame[i][j].symb == 'H') {
						mGame[i + 1][j] = mGame[i][j];
						mGame[i][j].symb = ' ';
						mGame[i][j].bgCol = col_black;
					}
				}
			}
		}
		//generating obstacles
		Pixel obstacle;
		obstacle.symb = 'H';
		obstacle.bgCol = col_red;
		obstacle.fgCol = col_darkRed;

		if (mTicksPassed%obstacleFrequency == 0) {
		//constistent obstacle
			int x = rand() % effWidth;
			mGame[0][x] = obstacle;
			mTickTime *= mTickSpeed;
		}
		if (rand() % 100 <= extraObstacleChance) {
		//extra obstacle
			int x = rand() % effWidth;
			mGame[0][x] = obstacle;
		}

		//checking for collision
		if (mGame[effHeight - 1][carX].symb == 'H') {
			mIsRunning = false;
			mGame[effHeight - 1][carX].fgCol = col_yellow;
			mGame[effHeight - 1][carX].bgCol = col_darkYellow;
		}

		InsertArray(mY + 1, mX + 1, mGame);
		mMutex.unlock();

		Sleep(max(mTickTime, minTickTime));
		mTicksPassed++;
	}
}

void MinigameCars::Test() {
	
}

void MinigameCars::Run() {
	thread inputWorker(&MinigameCars::GameControl, this);
	thread gameWorker(&MinigameCars::GameLogic, this);

	inputWorker.join();
	gameWorker.join();

	*mGameEngineIsRunning = false;
}










MinigameCars::MinigameCars(
	int y, int x, int height, int width,
	int frameColor, double tickTime, double tickSpeed,
	bool *gameEngineIsRunning)
	: MinigameEngine(y, x, height, width, frameColor, tickTime, tickSpeed, gameEngineIsRunning)
{
	carX = (width - 2) / 2;
}
MinigameCars::MinigameCars(){
}
MinigameCars::~MinigameCars(){
}
