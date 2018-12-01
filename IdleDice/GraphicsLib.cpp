#include "pch.h"
#include "GraphicsLib.h"

//static members' definition
vector<vector<vector<Pixel>>>* GraphicsLib::mCurrFrame = new vector<vector<vector<Pixel>>>(1);
vector<vector<vector<Pixel>>>* GraphicsLib::mLastFrame = new vector<vector<vector<Pixel>>>(1);
int GraphicsLib::mLastLayer = 0;
int *GraphicsLib::mCurrLayer = new int(0);
int GraphicsLib::mHeight = 0;
int GraphicsLib::mWidth = 0;
/*********************WORKING ON*********************/

void GraphicsLib::Test() {

	int money = 0;
	auto start = chrono::system_clock::now();
	Logging log;
	

	while (money<200) {
		auto end = chrono::system_clock::now();
		std::chrono::duration<double> elapsed = end - start;
		GraphicsLib::InsertTextBox(5, 5, 0, "Time: " + to_string(elapsed.count()), 3, 20, col_black, col_white);
		GraphicsLib::InsertTextBox(10, 5, 0, "Money: " + to_string(money), 3, 20, col_black, col_white);
		DrawScreen();
		
		money++;
	}
}


void GraphicsLib::DrawScreen() {
#define currPixel GraphicsLib::mCurrFrame->at(*GraphicsLib::mCurrLayer)[i][j]
#define zeroLayerPixel GraphicsLib::mCurrFrame->at(0)[i][j]

	//Might want to have fixed current frame value

	for (int i = 0; i < GraphicsLib::mCurrFrame->at(*GraphicsLib::mCurrLayer).size(); i++) {
		for (int j = 0; j < GraphicsLib::mCurrFrame->at(*GraphicsLib::mCurrLayer)[i].size(); j++) {
			//will need to check whether this works
			//It should update pixel if it's visible in that layer, else update with layer zero pixel
			if (currPixel.visible) DrawPixel(i, j, currPixel.symb, currPixel.bgCol, currPixel.fgCol);
			else DrawPixel(i, j, zeroLayerPixel.symb, zeroLayerPixel.bgCol, zeroLayerPixel.fgCol);
		}
	}

	//saving current values
	for (int k = 0; k < mCurrFrame->size(); k++) {
		for (int i = 0; i < mCurrFrame->at(k).size(); i++) {
			for (int j = 0; j < mCurrFrame->at(k)[i].size(); j++) {
				mLastFrame->at(k)[i][j] = mCurrFrame->at(k)[i][j];
			}
		}
	}
	GraphicsLib::mLastLayer = *GraphicsLib::mCurrLayer;
}
/*********************FINISHED*********************/
void GraphicsLib::DrawPixel(int y, int x, char symb, int bgCol, int fgCol) {
#define lastPixel GraphicsLib::mLastFrame->at(GraphicsLib::mLastLayer)[y][x]
	if (!NeedUpdatePixel(y, x, symb, bgCol, fgCol)) return;
	//Goto 
	COORD coord;
	coord.Y = y;
	coord.X = x;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

	//Set color
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), bgCol * 16 + fgCol);

	//Output symbol
	cout << symb;

	//Change color back to black
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);

}
void GraphicsLib::InsertPixel(int y, int x, int layer, char symb, int bgCol, int fgCol) {
#define pixel mCurrFrame->at(layer)[y][x]
	//exit if coordinates or layer are out of bounds
	if (y < 0 || y >= mCurrFrame->at(0).size() || x < 0 || x >= mCurrFrame->at(0)[0].size() ||
		layer < 0 || layer >= mCurrFrame->size()) return;

	pixel.symb = symb;
	pixel.bgCol = bgCol;
	pixel.fgCol = fgCol;
	pixel.visible = true;
}
bool GraphicsLib::NeedUpdatePixel(int y, int x, char symb, int bgCol, int fgCol) {
#define lastPixel GraphicsLib::mLastFrame->at(GraphicsLib::mLastLayer)[y][x]
	//if curr and last pixels are spaces and bgCol matches
	if (symb == ' ' && lastPixel.symb == ' ' && bgCol == lastPixel.bgCol) false;
	//if curr and last pixels are not spaces, then exit if their symb, bgCol and fgCol match
	else if (symb == lastPixel.symb && bgCol == lastPixel.bgCol && fgCol == lastPixel.fgCol) false;
	else return true;
}
void GraphicsLib::InsertTextBox(int y, int x, int layer, string text, int height, int width, int bgCol, int fgCol) {
	//Split given text into array of lines of width length and insert into frame
	stringstream ss(text);
	string word;
	vector<string> textBox(1);

	//protection
	if (width == 0 || height == 0) return;

#define lastLine textBox[textBox.size()-1]
	while (ss >> word) {
		//if word is big
		if (word.length() >= width / 2) {
			for (int i = 0; i < word.length(); i++) {
				if (lastLine.size() >= width) textBox.push_back(string());
				lastLine += word[i];
			}
			lastLine += " ";
		}
		else {
			if (lastLine.size() + word.size() > width) textBox.push_back(string());
			lastLine += word+" ";
		}
	}

	textBox.resize(height);

	for (int i = 0; i < height; i++)
		textBox[i] += string(max(0, width - textBox[i].size()), ' ');

	Logging log;
	
	log.Log(textBox.size());

	for (int i = y; i < min(y + height, mHeight); i++) {

		for (int j = x; j < min(x + textBox[i-y].size(), mWidth); j++) {
				GraphicsLib::InsertPixel(i, j, layer, textBox[i-y][j-x], bgCol, fgCol);
		}
	}


}
void GraphicsLib::SetData(int height, int width, int layerCount, int* currentLayer) {
	//creating line container
	vector<Pixel> line(width);
	//creating grid container
	vector<vector<Pixel>> screen(height, line);
	//setting data
	delete GraphicsLib::mCurrFrame;
	delete GraphicsLib::mLastFrame;
	GraphicsLib::mCurrFrame = new vector<vector<vector<Pixel>>>(layerCount, screen);
	GraphicsLib::mLastFrame = new vector<vector<vector<Pixel>>>(layerCount, screen);

	GraphicsLib::mCurrLayer = currentLayer;

	GraphicsLib::mHeight = height;
	GraphicsLib::mWidth = width;

	//setting layer zero to visible
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			mCurrFrame->at(0)[i][j].visible = true;
			mLastFrame->at(0)[i][j].visible = true;
		}
	}
}
