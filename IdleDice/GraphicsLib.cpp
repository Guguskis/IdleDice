#include "pch.h"
#include "GraphicsLib.h"

//static members' definition
vector<Command> GraphicsLib::mCommand;
vector<vector<Pixel>>* GraphicsLib::mCurrFrame = new vector<vector<Pixel>>(1);
vector<vector<Pixel>>* GraphicsLib::mLastFrame = new vector<vector<Pixel>>(1);
int GraphicsLib::mHeight = 0;
int GraphicsLib::mWidth = 0;
bool GraphicsLib::mCommandInUse = false;
/*********************WORKING ON*********************/




void GraphicsLib::Test() {

}



/*********************FINISHED*********************/

void GraphicsLib::DrawScreen() {
	//display whole screen
	if (mCommand.size() == 0) return;

	HandleCommands();

	for (int i = 0; i < mHeight; i++) {
		for (int j = 0; j < mWidth; j++) {
			DrawPixel(i, j, &mCurrFrame->at(i)[j]);
		}
	}

	//saving current frame
	for (int i = 0; i < mHeight; i++) {
		for (int j = 0; j < mWidth; j++)
			mLastFrame->at(i)[j] = mCurrFrame->at(i)[j];
	}

}
void GraphicsLib::ClearScreen() {
	//display black screen
	for (int i = 0; i < mHeight; i++) {
		for (int j = 0; j < mWidth; j++) {
			InsertPixel(i, j, ' ', col_black, col_black);
		}
	}
}

void GraphicsLib::InsertArray(int y, int x, vector<vector<Pixel>> arr){
	//parse InsertArray command to command vector
	Command comm;
	comm.type = "Array";
	comm.y = y;
	comm.x = x;
	comm.arr = arr;
	while (mCommandInUse) Sleep(1);
	mCommand.push_back(comm);
}
void GraphicsLib::InsertTextBox(int y, int x, string text, int height, int width, int bgCol, int fgCol){
	//parse InsertTextBox command to command vector
	Command comm;
	comm.text = "TextBox";
	comm.y = y;
	comm.x = x;
	comm.text = text;
	comm.height= height;
	comm.width= width;
	comm.bgCol= bgCol;
	comm.fgCol= fgCol;
	while (mCommandInUse) Sleep(1);
	mCommand.push_back(comm);
}
void GraphicsLib::InsertLine(int y, int x, string text, int bgCol, int fgCol){
	//parse InsertLine command to command vector
	Command comm;
	comm.type = "Line";
	comm.y = y;
	comm.x = x;
	comm.text = text;
	comm.bgCol = bgCol;
	comm.fgCol = fgCol;
	//wait while commands are being executed
	while (mCommandInUse) Sleep(1);
	mCommand.push_back(comm);
}

void GraphicsLib::HandleCommands() {
	//execute pending commands
#define c mCommand[i]
	mCommandInUse = true;
	for(int i=0; i<mCommand.size(); i++) {
		if (c.type == "Line") HandleLine(c.y, c.x, c.text, c.bgCol, c.fgCol);
		else if (c.type == "TextBox") HandleTextBox(c.y, c.y, c.text, c.height, c.width, c.bgCol, c.fgCol);
		else if (c.type == "Array") HandleArray(c.y, c.x, c.arr);
	}
	mCommand.clear();
	mCommandInUse = false;
}
void GraphicsLib::HandleArray(int y, int x, const vector<vector<Pixel>> arr) {
	//display given array on screen
	for (int i = y; i < y + arr.size(); i++) {
		for (int j = x; j < x + arr.at(0).size(); j++) {
			InsertPixel(i, j, arr.at(i)[j].symb, arr.at(i)[j].bgCol, arr.at(i)[j].fgCol);
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
		InsertPixel(y, x + i, text[i], bgCol, fgCol);
	}
}

void GraphicsLib::DrawPixel(int y, int x, Pixel * pixel) {
	//displays given pixel on screen
	//exit, if pixel doesn't need update
	if (!pixel->update) return;

	//Goto 
	COORD coord;
	coord.Y = y;
	coord.X = x;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

	//Set color
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), pixel->bgCol * 16 + pixel->fgCol);

	//Output symbol
	cout << pixel->symb;

	//Change color back to black
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
	pixel->update = false;
}
void GraphicsLib::InsertPixel(int y, int x, char symb, int bgCol, int fgCol) {
	//change the values of pixel at given coordinates
#define pixel1 mCurrFrame->at(y)[x]
	//exit if coordinates are out of bounds
	if (y < 0 || y >= mHeight || x < 0 || x >= mWidth) return;

	Pixel pixel2;
	pixel2.symb = symb;
	pixel2.bgCol = bgCol;
	pixel2.fgCol = fgCol;

	if (NeedUpdatePixel(&pixel2, &pixel1)) {
		if (y == 0 && x == 0) {
			Logging::Log(y, x);
		}
		pixel1.update = true;
		if(symb!=-1) pixel1.symb = symb;
		if(bgCol!=col_noColor)	pixel1.bgCol = bgCol;
		if(fgCol!=col_noColor)	pixel1.fgCol = fgCol;
	}
#undef pixel
}
bool GraphicsLib::NeedUpdatePixel(const Pixel * cpx, const Pixel * lpx) {
	/*
		cpx - current pixel
		lpx - last pixel
	*/
	//check if two pixels are different. If yes, return true, else return false

	//if (cpx->symb == ' ') {
	//	if (cpx->symb != lpx->symb || cpx->bgCol != lpx->bgCol) return true;
	//}
	//else if (cpx->symb != lpx->symb || cpx->bgCol != lpx->bgCol || cpx->fgCol != lpx->fgCol) return true;
	if (cpx->symb == lpx->symb && cpx->bgCol == lpx->bgCol && cpx->fgCol == lpx->fgCol) return false;
	else if (cpx->bgCol == col_noColor && lpx->fgCol == col_noColor) {
		if (cpx->symb != lpx->symb) return true;
		else return false;
	}
	else if (cpx->bgCol == col_noColor) {
		if (cpx->symb == ' ' && lpx->symb == ' ') return false;
		else if (cpx->symb != lpx->symb || cpx->fgCol != lpx->fgCol) return true;
		else return false;
	}
	else if (cpx->fgCol == col_noColor) {
		if (cpx->symb == ' ' && lpx->symb == ' ' && cpx->bgCol==lpx->bgCol) return false;
		else if (cpx->symb != lpx->symb || cpx->bgCol != lpx->bgCol) return true;
		else return false;
	}
	else {
		if (cpx->symb == ' ' && lpx->symb == ' ' && cpx->bgCol == cpx->fgCol) return false;
		else return true;
	}
/*
	if (cpx->symb != lpx->symb || cpx->bgCol != lpx->bgCol || cpx->fgCol != lpx->fgCol) return true;
	else return false;
*/
}

void GraphicsLib::SetData(int height, int width) {
	//creating line container
	vector<Pixel> line(width);
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