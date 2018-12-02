#include "pch.h"
#include "Core.h"


void Core::HandleGraphics() {
	//this method handles graphics.
	//TODO: should also execute calculations


	auto start = chrono::system_clock::now();
	
	auto startFpsCounter = chrono::system_clock::now();
	auto startFrame = chrono::system_clock::now();
	int framesDrawn = 0;

	while (mGameIsRunning) {
		startFrame = chrono::system_clock::now();

		GraphicsLib::DrawScreen();
		
		auto elapsedFrame = chrono::system_clock::now() - startFrame;

		while (elapsedFrame.count() < 1.0f / mFPS) {
			Sleep(1);
			elapsedFrame = chrono::system_clock::now() - startFrame;
		}

	}
}
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

	//InputLib input;
	//input.TestKeys();

	Core::GameLoop();

	thread graphicsWorker(&Core::HandleGraphics, this);
	thread inputWorker(&Core::HandleInput, this);

	int money = 0;
	auto start = chrono::system_clock::now();

	vector<vector<Pixel>> arr(mHeight, vector<Pixel>(mWidth));
	int y = 0, x = 0;
	InputLib input;


	while (mGameIsRunning) {

		for (int i = 0; i < mHeight; i++) {
			for (int j = 0; j < mWidth; j++) {
				if (i == y && j == x) arr[i][j].bgCol = col_green;
				else if (i == y) arr[i][j].bgCol = col_red;
				else if (j == x) arr[i][j].bgCol = col_red;
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

	graphicsWorker.join();
	inputWorker.join();
}

void Core::GameLoop() {
	//This method is used to run the game. It runs graphics and input on seperate threads

	/*thread graphicsWorker(&Core::HandleGraphics, this);
	thread inputWorker(&Core::HandleInput, this);
	graphicsWorker.detach();
	inputWorker.detach();
*/
}


Core::Core() {
	GraphicsLib::SetData(mHeight, mWidth);
}