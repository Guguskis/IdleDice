#include "pch.h"
#include "MinigamePianoTiles.h"






void MinigamePianoTiles::GameLogic() {
	//initializing effective dimensions
	int effHeight = mHeight - 2;
	int effWidth = mWidth - 2;
	DrawFrame();

	//drawing green lines and changing their foreground color
	for (int i = 0; i < effHeight; i++) {
		for (int j = 0; j < greenLines; j++) {
			mGame[i][j + 1].bgCol = col_green;
			mGame[i][0].fgCol = col_pink;
			mGame[i][1].fgCol = col_red;
			mGame[i][2].fgCol = col_yellow;
		}
	}


	while (mIsRunning && *mGameEngineIsRunning) {
		mMutex.lock();
		for (int i = 0; i < effHeight; i++) {
			for (int j = 0; j < effWidth; j++) {

				//removing first column of notes
				if (j == 0) {
					if (mGame[i][j].symb != ' ') {
						mGame[i][j].symb = ' ';
						lives -= 2;
					}
				}
				//shifting everything to the left
				else {
					if (mGame[i][j].symb != ' ') {
						mGame[i][j - 1].symb = mGame[i][j].symb;
						mGame[i][j].symb = ' ';
					}
				}
			}
		}

		//generating notes
		if (mTicksPassed%noteFrequency == 0) {
			int y = rand() % effHeight;
			mGame[y][effWidth - 1].symb = '1' + y;
			mGame[y][effWidth - 1].fgCol = col_white;

			mTickTime *= mTickSpeed;
		}

		//drawing life counter
		string text = "Lives ";
		InsertLine(mY, mX, text, col_noColor, col_grey);

		for (int i = 1; i <= maxLives; i+=2) {
			char tile;
			if (i == lives && lives % 2 == 1) tile = '\\';
			else if (i < lives) tile = 'V';
			else tile = ' ';

			InsertPixel(mY, mX + i/2 + text.length(), tile, col_noColor, col_darkRed);

		}

		if (lives <= 0)
			mIsRunning = false;



		UpdateGraphics();
		mMutex.unlock();

		Sleep(max(mTickTime, minTickTime));
		mTicksPassed++;
	}

}
void MinigamePianoTiles::GameControl() {
	while (mIsRunning && *mGameEngineIsRunning) {
		int keyPressed = -1;
		bool noteHit = false;
		int noteX = 0, noteY = 0;

		//checkign all pressed keys
		for (int i = 0; i < mHeight - 2; i++) {
			string key = "1";
			key[0] += i;

			//if key was pressed, record note coordinates
			if (KeyPressed(key)) {
				keyPressed = i;

				mMutex.lock();
				for (int j = 1; j <= greenLines && !noteHit; j++) {
					if (mGame[i][j].symb != ' ') {
						mGame[i][j].symb = ' ';
						noteHit = true;
						noteY = i;
						noteX = j;
					}
				}
				mMutex.unlock();
			}
		}

		//handle input
		if (keyPressed != -1) {
			mMutex.lock();
			if (!noteHit) {
				lives--;
				InsertLine(mY + 1 + keyPressed, mX + 2, string(greenLines, ' '), col_red, col_noColor);
			}
			else {
				InsertArray(mY + 1, mX + 1, mGame);
				InsertPixel(mY + noteY + 1, mX + noteX + 1, ' ', col_darkGreen, col_noColor);
			}
			mMutex.unlock();
		}

		Sleep(1);
	}
}
void MinigamePianoTiles::Run() {
	thread inputWorker(&MinigamePianoTiles::GameControl, this);
	thread gameWorker(&MinigamePianoTiles::GameLogic, this);

	inputWorker.join();
	gameWorker.join();

	*mGameEngineIsRunning = false;
}

MinigamePianoTiles::MinigamePianoTiles(
	int y, int x, int height, int width,
	int frameColor, double tickTime, double tickSpeed,
	bool *gameEngineIsRunning)
	: MinigameEngine(y, x, height, width, frameColor, tickTime, tickSpeed, gameEngineIsRunning)
{
}
MinigamePianoTiles::MinigamePianoTiles() {

}
MinigamePianoTiles::~MinigamePianoTiles() {
}



