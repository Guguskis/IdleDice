#include "pch.h"
#include "Core.h"


/*********************WORKING ON*********************/

void Core::Run() {
	auto runInput = [](bool* mGameIsRunning) {
		while (*mGameIsRunning) {
			GetInput();
			if (KeyPressed("Esc")) *mGameIsRunning = false;
		}
	};
	thread workerInput(runInput, &mGameIsRunning);

	MinigameCars cars(0, 0, 20, 10, col_green, 600.f, 0.98f, &mGameIsRunning);
	MinigamePianoTiles pianoTiles(0, 10, 6, 30, col_cyan, 600.f, 0.98f, &mGameIsRunning);
	MinigameLaser laser(6, 10, 14, 30, col_red, 600.f, 0.98f, &mGameIsRunning);

	thread carsWorker(&MinigameCars::Run, &cars);
	thread pianoTilesWorker(&MinigamePianoTiles::Run, &pianoTiles);
	thread laserWorker(&MinigameLaser::Run, &laser);

	carsWorker.join();
	pianoTilesWorker.join();
	laserWorker.join();
	workerInput.join();

}

/*********************FINISHED*********************/
void Core::Test(){
	
}
void Core::SetConsole() {
	//Hiding cursor
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO     cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.dwSize = 1;
	cursorInfo.bVisible = false; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
	
}
void Core::GameOverAnimation(int y, int x, int endY, int endX, int height, int width) {

	int radius = max(height, width);

	vector<vector<Pixel>> screen(height, vector<Pixel>(width, Pixel()));

	for (int k = 0; k < radius; k++) {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				//x^2+y^2=z^2
			}
		}
	}

	
	
	cin >> radius;
}
Core::Core() {
	//set up console
	SetConsole();
	//initialize random seed
	srand(time(NULL));
	//set graphics handler data
	GraphicsLib::SetData(mHeight, mWidth);
	//start graphics handler
	thread graphicsWorker(&GraphicsLib::HandleGraphics, &mFPS, &mGameIsRunning);
	graphicsWorker.detach();
}
void Core::Demo(){
	//demonstration of how to use graphics and input libraries and what they can do
	
	
	int money = 0;

	vector<vector<Pixel>> arr(mHeight, vector<Pixel>(mWidth));
	int y = 0, x = 0;

	auto spawnSymbols = [](vector<vector<Pixel>> *arr, int height, int width, bool *gameIsRunning) {
		int commandsExecuted = 0;
		while(*gameIsRunning){
			int y = rand() % (height-2)+1;
			int x = rand() % (width-2)+1;
			GraphicsLib::InsertLine(y, 1 , string(width-2, 'a' + rand() % ('z' - 'a')), col_noColor, rand()%16);
			commandsExecuted++;
			if (commandsExecuted % 100 == 0) GraphicsLib::InsertClearScreen();
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

		InputLib::GetInput();

		if (InputLib::KeyPressed("Left"))	x = (x - 1) % mWidth;
		else if (InputLib::KeyPressed("Right"))	x = (x + 1) % mWidth;
		else if (InputLib::KeyPressed("Down"))	y = (y + 1) % mHeight;
		else if (InputLib::KeyPressed("Up"))	y = (y - 1) % mHeight;
		else if (InputLib::KeyPressed("Esc")) mGameIsRunning = false;
		if (y < 0) y += mHeight;
		if (x < 0) x += mWidth;

	}

	spawnSymbolWorker.join();
}
