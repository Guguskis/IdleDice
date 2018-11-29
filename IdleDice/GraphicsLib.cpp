#include "GraphicsLib.h"

//static members' definition
vector<vector<vector<Pixel>>>* GraphicsLib::mCurrFrame;
vector<vector<vector<Pixel>>>* GraphicsLib::mLastFrame;
int GraphicsLib::mLastLayer;
int *GraphicsLib::mCurrLayer;

/*********************WORKING ON*********************/

void GraphicsLib::Test() {
	bool drawBlack = false;
	while (1) {
		for (int i = 0; i < GraphicsLib::mCurrFrame->at(*GraphicsLib::mCurrLayer).size(); i++) {
			for (int j = 0; j < GraphicsLib::mCurrFrame->at(*GraphicsLib::mCurrLayer)[i].size(); j++) {
				if (drawBlack) GraphicsLib::mLastFrame->at(*GraphicsLib::mCurrLayer)[i][j].bgCol = col_black;
				else GraphicsLib::mLastFrame->at(*GraphicsLib::mCurrLayer)[i][j].bgCol = col_white;
			}
		}
		DrawScreen();
		drawBlack = !drawBlack;
	}
}


void GraphicsLib::DrawScreen(){
#define currPixel GraphicsLib::mCurrFrame->at(*GraphicsLib::mCurrLayer)[i][j]
#define zeroLayerPixel GraphicsLib::mCurrFrame->at(0)[i][j]

	//Might want to have fixed current frame value

	for (int i = 0; i < GraphicsLib::mCurrFrame->at(*GraphicsLib::mCurrLayer).size(); i++) {
		for (int j = 0; j < GraphicsLib::mCurrFrame->at(*GraphicsLib::mCurrLayer)[i].size(); j++) {
			//will need to check whether this works
			//It should update pixel if it's visible in that layer, else update with layer zero pixel
			if(currPixel.visible) DrawPixel(i, j, currPixel.symb, currPixel.bgCol, currPixel.fgCol);
			else DrawPixel(i, j, zeroLayerPixel.symb, zeroLayerPixel.bgCol, zeroLayerPixel.fgCol);
		}
	}
	
	//saving current values
	swap(mCurrFrame, mLastFrame);
	GraphicsLib::mLastLayer = *GraphicsLib::mCurrLayer;
}
/*********************FINISHED*********************/
void GraphicsLib::DrawPixel(int y, int x, char symb, int bgCol, int fgCol){
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
bool GraphicsLib::NeedUpdatePixel(int y, int x, char symb, int bgCol, int fgCol){
#define lastPixel GraphicsLib::mLastFrame->at(GraphicsLib::mLastLayer)[y][x]
	//if curr and last pixels are spaces and bgCol matches
	if (symb == ' ' && lastPixel.symb == ' ' && bgCol == lastPixel.bgCol) false;
	//if curr and last pixels are not spaces, then exit if their symb, bgCol and fgCol match
	else if (symb == lastPixel.symb && bgCol == lastPixel.bgCol && fgCol == lastPixel.fgCol) false;
	else return true;
}
void GraphicsLib::SetData(int height, int width, int layerCount, int* currentLayer){
	//creating line container
	vector<Pixel> line(width);
	//creating grid container
	vector<vector<Pixel>> screen(height, line);
	//assigning pointers to adresses
	GraphicsLib::mCurrFrame = new vector<vector<vector<Pixel>>>(layerCount, screen);
	GraphicsLib::mLastFrame = new vector<vector<vector<Pixel>>>(layerCount, screen);

	GraphicsLib::mCurrLayer = currentLayer;
}
