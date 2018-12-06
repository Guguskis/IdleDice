#include "pch.h"
#include "Core.h"


void Core::HandleGraphics() {
	//this method handles graphics.
	//TODO: should also execute calculations


	auto startFpsCounter = chrono::steady_clock::now();
	auto startFrame = chrono::steady_clock::now();
	int framesDrawn = 0;

	while (mGameIsRunning) {
		//capture frame start time
		startFrame = chrono::steady_clock::now();
		//do the work
		GraphicsLib::DrawScreen();
		//measure end time
		auto endFrame = chrono::steady_clock::now();

		//control maximum frame rate
		while (chrono::duration_cast<chrono::milliseconds>(endFrame-startFrame).count() < 1000.f / mFPS) {
			Sleep(1);
			endFrame = chrono::steady_clock::now();
		}

		//display fps every half second
		auto endFpsCounter = chrono::steady_clock::now();
		if (chrono::duration_cast<chrono::milliseconds>(endFpsCounter - startFpsCounter).count() > 500.f) {
			//save momentary frames per second value as string: 1000 milliseconds / draw time of last frame
			string displayFPS = to_string((int)round(1000.f / chrono::duration_cast<chrono::milliseconds>(endFrame - startFrame).count()));
			//even the length
			while (displayFPS.length() < 4) displayFPS += " ";
			InsertLine(0, mWidth-4, displayFPS, col_noColor, col_green);
			//restart timer
			startFpsCounter = chrono::steady_clock::now();
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
				else if (i == y && j % 4 == 0) arr[i][j].bgCol = col_red;
				else if (j == x && i % 4 == 0) arr[i][j].bgCol = col_red;
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




Core::Core() {
	GraphicsLib::SetData(mHeight, mWidth);
}