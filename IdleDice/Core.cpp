#include "pch.h"
#include "Core.h"


void Core::HandleGraphics(){
	//this method handles graphics.
	//TODO: should also execute calculations

	while (mGameIsRunning) {
		GraphicsLib::DrawScreen();
	}
}
void Core::HandleInput(){
	InputLib input;
	
	while (mGameIsRunning) {
		input.GetInput();

		if (input.KeyPressed("Esc")) mGameIsRunning=false;
		else if (input.KeyPressed("0")) InsertTextBox(0, 0, string(25, ' '), 1, 25, col_red, col_red);
		else if (input.KeyPressed("1")) InsertTextBox(1, 0, string(25, ' '), 1, 25, col_red, col_red);
		else if (input.KeyPressed("2")) InsertTextBox(2, 0, string(25, ' '), 1, 25, col_red, col_red);
		else if (input.KeyPressed("3")) InsertTextBox(3, 0, string(25, ' '), 1, 25, col_red, col_red);
		else if (input.KeyPressed("4")) InsertTextBox(4, 0, string(25, ' '), 1, 25, col_red, col_red);

	}
}

void Core::Test() {
	//This method contains children class Test methods to make it easier to debug

	//InputLib input;
	//input.TestKeys();

	Core::GameLoop();


	int money = 0;
	auto start = chrono::system_clock::now();

	vector<vector<Pixel>> arr(mHeight, vector<Pixel>(mWidth));
	int y = 0, x = 0;
	while (mGameIsRunning) {
		/*auto end = chrono::system_clock::now();
		std::chrono::duration<double> elapsed = end - start;
		money++;
		GraphicsLib::InsertTextBox(10, 0, to_string(elapsed.count()), 1, 10, col_black, col_yellow);
		GraphicsLib::InsertTextBox(11, 0, to_string(money), 1, 10, col_black, col_yellow);
*/
		for (int i = 0; i < mHeight; i++) {
			for (int j = 0; j < mWidth; j++) {
				if (i == y && j == x) arr[i][j].bgCol = col_yellow;
				else if (i == y) arr[i][j].bgCol = col_red;
				else if (j == x) arr[i][j].bgCol = col_blue;
				else arr[i][j].bgCol = col_black;
			}
		}
		InsertArray(0, 0, &arr);
		y = (y + 1) % mHeight;
		x = (x + 1) % mWidth;
		Sleep(100);
	}

}

void Core::GameLoop(){
	//This method is used to run the game. It runs graphics and input on seperate threads
	thread graphicsWorker(&Core::HandleGraphics, this);
	thread inputWorker(&Core::HandleInput, this);
	graphicsWorker.detach();
	inputWorker.detach();
}


Core::Core(){
	GraphicsLib::SetData(mHeight, mWidth);
}