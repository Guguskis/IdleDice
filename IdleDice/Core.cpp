#include "pch.h"
#include "Core.h"



void Core::HandleInput() {
	InputLib input;
	/*
	while (mGameIsRunning) {
		input.GetInput();

		if (input.KeyPressed("Esc")) mGameIsRunning=false;
		else if (input.KeyPressed("0")) InsertTextBox(0, 0, string(25, ' '), 1, 25, col_red, col_red);
		else if (input.KeyPressed("1")) InsertTextBox(1, 0, string(25, ' '), 1, 25, col_red, col_red);
		else if (input.KeyPressed("2")) InsertTextBox(2, 0, string(25, ' '), 1, 25, col_red, col_red);
		else if (input.KeyPressed("3")) InsertTextBox(3, 0, string(25, ' '), 1, 25, col_red, col_red);
		else if (input.KeyPressed("4")) InsertTextBox(4, 0, string(25, ' '), 1, 25, col_red, col_red);

	}*/
}

void Core::Test() {
	//This method contains children class Test methods to make it easier to debug

	
}

void Core::Demo(){
	//demonstration of how to use graphics and input libraries and what they can do
	//thread inputWorker(&Core::HandleInput, this);
	
	thread graphicsWorker(&GraphicsLib::HandleGraphics, &mFPS, &mGameIsRunning);
	InputLib input;
	int money = 0;


	vector<vector<Pixel>> arr(mHeight, vector<Pixel>(mWidth));
	int y = 0, x = 0;

	auto spawnSymbols = [](vector<vector<Pixel>> *arr, int height, int width, bool *gameIsRunning) {
		while(*gameIsRunning){
			int y = rand() % (height-2)+1;
			int x = rand() % (width-2)+1;
			GraphicsLib::InsertLine(y, 1 , string(width-2, 'a' + rand() % ('z' - 'a')), col_noColor, rand()%16);
			Sleep(60);
		}
	};

	thread spawnSymbolWorker(spawnSymbols, &arr, mHeight, mWidth, &mGameIsRunning);


	while (mGameIsRunning) {

		for (int i = 0; i < mHeight; i++) {
			for (int j = 0; j < mWidth; j++) {
				if (i == y && j == x && (j % 4 == 0 && i % 4 == 0)) arr[i][j].bgCol = col_red;
				else if (i == y && j == x && ((j % 4 == 0 && i % 4 != 0) || (j % 4 != 0 && i % 4 == 0))) arr[i][j].bgCol = col_yellow;
				else if (i == y && j == x) arr[i][j].bgCol = col_green;
				else if (i == y && j % 4 == 0) arr[i][j].bgCol = col_grey;
				else if (j == x && i % 4 == 0) arr[i][j].bgCol = col_grey;
				else if (i == 0 || i == mHeight - 1 || j == 0 || j == mWidth - 1) arr[i][j].bgCol = col_cyan;
				else arr[i][j].bgCol = col_black;
				arr[i][j].fgCol = col_noColor;
			}
		}

		InsertArray(0, 0, arr);

		input.GetInput();

		if (InputLib::KeyPressed("Left"))	x = (x - 1) % mWidth;
		else if (InputLib::KeyPressed("Right"))	x = (x + 1) % mWidth;
		else if (InputLib::KeyPressed("Down"))	y = (y + 1) % mHeight;
		else if (InputLib::KeyPressed("Up"))	y = (y - 1) % mHeight;
		else if (InputLib::KeyPressed("Esc")) mGameIsRunning = false;
		if (y < 0) y += mHeight;
		if (x < 0) x += mWidth;

	}

	spawnSymbolWorker.join();
	graphicsWorker.join();
	//inputWorker.join();
}




Core::Core() {
	GraphicsLib::SetData(mHeight, mWidth);
	srand(time(NULL));
}