#include "pch.h"
#include "GraphicsLib.h"

//static members' definition
vector<Command> GraphicsLib::mCommand;
vector<vector<Pixel>>* GraphicsLib::mCurrFrame = new vector<vector<Pixel>>(1);
vector<vector<Pixel>>* GraphicsLib::mLastFrame = new vector<vector<Pixel>>(1);
int GraphicsLib::mHeight = 0;
int GraphicsLib::mWidth = 0;
bool GraphicsLib::mCommandInUse = false;
bool GraphicsLib::mFrameRateOn = true;
mutex GraphicsLib::mMutexCommand;
int GraphicsLib::mYoffset = 10;
int GraphicsLib::mXoffset = 15;

/*********************WORKING ON*********************/



void GraphicsLib::Test() {

}



/*********************FINISHED*********************/

void GraphicsLib::HandleGraphics(int *fps, bool *gameIsRunning){

	auto frameStart = chrono::steady_clock::now();
	auto frameEnd = chrono::steady_clock::now();
	auto counterStart = chrono::steady_clock::now();



	while (*gameIsRunning) {
		//applying given commands
		if (mCommand.size() > 0) {
			HandleCommands();
		}

		/*controlling framerate*/
		frameEnd = chrono::steady_clock::now();
		auto frameElapsedTime = chrono::duration_cast<chrono::milliseconds>(frameEnd - frameStart);

		//draw frame if enough time has passed since last frame was drawn
		if (frameElapsedTime.count() >= (1000.f / *fps)) {

			//drawing FPS counter on top-right screen
			if (mFrameRateOn) {
				//capturing time periods
				auto counterEnd = chrono::steady_clock::now();
				auto counterElapsedTime = chrono::duration_cast<chrono::milliseconds>(counterEnd - counterStart);

				//redraw fps counter if appropriate time period has passed since last execution
				if (counterElapsedTime.count() >= 200.f) {
					//storing frame rate as string and working out maximum framerate digit count
					string frameRate = to_string((int)round(1000.f / frameElapsedTime.count()));
					int numberLength = log10(*fps) + 1;

					//equalizing frame rate length
					while (frameRate.length() < numberLength) frameRate += " ";
					HandleLine(0, mWidth - numberLength, frameRate, col_noColor, col_red);

					//restart timer
					counterStart = chrono::steady_clock::now();
				}
			}

			//drawing frame
			DrawScreen();
			frameStart = chrono::steady_clock::now();
		}
	}

}
void GraphicsLib::DrawScreen() {
	//display whole screen

	for (int i = 0; i < mHeight; i++) {
		for (int j = 0; j < mWidth; j++) {
			if (NeedUpdatePixel(&mCurrFrame->at(i)[j], &mLastFrame->at(i)[j]))
				DrawPixel(i, j, &mCurrFrame->at(i)[j]);
		}
	}

	//saving current frame
	for (int i = 0; i < mHeight; i++) {
		for (int j = 0; j < mWidth; j++)
			mLastFrame->at(i)[j] = mCurrFrame->at(i)[j];
	}

}

void GraphicsLib::InsertClearScreen(){
	Command comm;
	comm.type = "ClearScreen";
	
	lock_guard<mutex> lock(mMutexCommand);
	mCommand.push_back(comm);
}
void GraphicsLib::InsertArray(int y, int x, vector<vector<Pixel>> arr) {
	//parse InsertArray command to command vector
	Command comm;
	comm.type = "Array";
	comm.y = y;
	comm.x = x;
	comm.arr = arr;

	lock_guard<mutex> lock(mMutexCommand);
	mCommand.push_back(comm);
}
void GraphicsLib::InsertTextBox(int y, int x, string text, int height, int width, int bgCol, int fgCol) {
	//parse InsertTextBox command to command vector
	Command comm;
	comm.text = "TextBox";
	comm.y = y;
	comm.x = x;
	comm.text = text;
	comm.height = height;
	comm.width = width;
	comm.bgCol = bgCol;
	comm.fgCol = fgCol;
	
	lock_guard<mutex> lock(mMutexCommand);
	mCommand.push_back(comm);
}
void GraphicsLib::InsertLine(int y, int x, string text, int bgCol, int fgCol) {
	//parse InsertLine command to command vector
	Command comm;
	comm.type = "Line";
	comm.y = y;
	comm.x = x;
	comm.text = text;
	comm.bgCol = bgCol;
	comm.fgCol = fgCol;
	
	lock_guard<mutex> lock(mMutexCommand);
	mCommand.push_back(comm);
}
void GraphicsLib::InsertPixel(int y, int x, char symb, int bgCol, int fgCol){
	//parse InsertPixel command to command vector
	Command comm;
	comm.type = "Pixel";
	comm.y = y;
	comm.x = x;
	comm.text = symb;
	comm.bgCol = bgCol;
	comm.fgCol = fgCol;

	lock_guard<mutex> lock(mMutexCommand);
	mCommand.push_back(comm);
}

void GraphicsLib::HandleCommands() {
	//execute pending commands
#define c mCommand[i]
	lock_guard<mutex> lock(mMutexCommand);

	for (int i = 0; i < mCommand.size(); i++) {
		if (c.type == "Line") HandleLine(c.y, c.x, c.text, c.bgCol, c.fgCol);
		else if (c.type == "TextBox") HandleTextBox(c.y, c.x, c.text, c.height, c.width, c.bgCol, c.fgCol);
		else if (c.type == "Array") HandleArray(c.y, c.x, c.arr);
		else if (c.type == "Pixel") HandlePixel(c.y, c.x, c.text[0], c.bgCol, c.fgCol);
		else if (c.type == "ClearScreen") HandleClearScreen();
	}
	mCommand.clear();
#undef c
}
void GraphicsLib::HandleClearScreen() {
	//display black screen
	for (int i = 0; i < mHeight; i++) {
		for (int j = 0; j < mWidth; j++) {
			Pixel blackPixel;
			blackPixel.symb = ' ';
			blackPixel.bgCol = col_black;
			if (NeedUpdatePixel(&mCurrFrame->at(i)[j], &blackPixel));
				HandlePixel(i, j, ' ', col_black, col_black);
		}
	}
}
void GraphicsLib::HandleArray(int y, int x, const vector<vector<Pixel>> arr) {
	//display given array on screen
	for (int i = 0; i < arr.size(); i++) {
		for (int j = 0; j < arr.at(i).size(); j++) {
			HandlePixel(i+y, j+x, arr.at(i)[j].symb, arr[i][j].bgCol, arr[i][j].fgCol);
		}
	}
}
void GraphicsLib::HandleTextBox(int y, int x, string text, int height, int width, int bgCol, int fgCol) {
	//Split given text into array of lines and display it
	stringstream ss(text);
	string word;
	vector<string> textBox(1);

	//protection
	if (width <= 0 || height <= 0) return;

#define lastLine textBox[textBox.size()-1]
	//adding text
	for (int i = 0; i < text.length(); i++) {
		if (lastLine.size() >= width) textBox.push_back(string());
		lastLine += text[i];
	}

	//making lines to be equal size
	for (int i = 0; i < height; i++) {
		if (textBox.size() < i) textBox.push_back(string());
		textBox[i] += string(width - textBox[i].size(), ' ');
	}

	//inserting text box
	for (int i = 0; i < height; i++) {
		HandleLine(y + i, x, textBox[i], bgCol, fgCol);
	}

#undef lastLine
}
void GraphicsLib::HandleLine(int y, int x, string text, int bgCol, int fgCol) {
	//display text line on screen
	for (int i = 0; i < text.length(); i++) {
		HandlePixel(y, x + i, text[i], bgCol, fgCol);
	}
}
void GraphicsLib::HandlePixel(int y, int x, char symb, int bgCol, int fgCol) {
	//change the values of pixel at given coordinates
#define pixel1 mCurrFrame->at(y)[x]
	//exit if coordinates are out of bounds
	if (y < 0 || y >= mHeight || x < 0 || x >= mWidth) return;

	if (symb != symb_noSymbol)  pixel1.symb = symb;
	if (bgCol != col_noColor)	pixel1.bgCol = bgCol;
	if (fgCol != col_noColor)	pixel1.fgCol = fgCol;

#undef pixel
}

void GraphicsLib::DrawPixel(int y, int x, Pixel * pixel) {
	//displays given pixel on screen

	//Goto 
	COORD coord;
	coord.Y = y+mYoffset;
	coord.X = x+mXoffset;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

	//Set color
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), pixel->bgCol * 16 + pixel->fgCol);

	//Output symbol
	cout << pixel->symb;

	//Change color back to black
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
}
bool GraphicsLib::NeedUpdatePixel(const Pixel * cpx, const Pixel * lpx) {
	//checks if given pixels are different enough to redraw them
	/*
		cpx - current pixel
		lpx - last pixel
	*/

	if (cpx->symb == ' ' && lpx->symb == ' ') {
		if (cpx->bgCol == lpx->bgCol) return false;
		else return true;
	}
	else if (cpx->symb == lpx->symb && cpx->bgCol == lpx->bgCol && cpx->fgCol == lpx->fgCol) return false;
	else return true;

}

void GraphicsLib::SetData(int height, int width) {
	//creating pixel container with custom values
	Pixel pixel;
	pixel.symb = ' ';
	pixel.bgCol = col_black;
	pixel.fgCol = col_black;
	//creating line container
	vector<Pixel> line(width, pixel);
	//creating grid container
	vector<vector<Pixel>> screen(height, line);
	//setting data
	delete GraphicsLib::mCurrFrame;
	delete GraphicsLib::mLastFrame;
	
	mCurrFrame = new vector<vector<Pixel>>(screen);
	mLastFrame = new vector<vector<Pixel>>(screen);

	mHeight = height;
	mWidth = width;

}
