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
	thread workerMinigame1(&Core::Minigame1, this, 0, 0, &mGameIsRunning);
	workerInput.join();
	workerMinigame1.join();
}

/*********************FINISHED*********************/
void Core::Test(){
	//This method contains children class Test methods to make it easier to debug

	vector<Dice> dices;

	for (int i = 0; i < 5; i++) {
		Dice dice("Normal", 5, 5 + 15 * i, 1.f, &mAnimationTime, '1' + i);
		dices.push_back(dice);
	}
	while (mGameIsRunning) {
		InputLib::GetInput();
		if (InputLib::KeyPressed("Esc")) mGameIsRunning = false;
		Dice::RollAllDices(&dices);
	}
}



void Core::Minigame1(int y, int x, bool *gameIsRunning) {
	mutex minigameMutex;
	bool minigameIsRunning = true;

	int height = 12, width = 6;
	int carX=2;

	//Draw frame around minigame window
	for (int i = 0; i < height+2; i++) {
		for (int j = 0; j < width+2; j++) {
			if (i == 0 || i == height + 1 || j == 0 || j == width + 1) {
				InsertPixel(i, j, ' ', col_cyan, col_noColor);
			}
		}
	}

	//Main minigame logic
	auto gameLoop = [](mutex *minigameMutex, vector<vector<Pixel>> *game, bool *minigameIsRunning, int *y, int *x, int *carX) {
		double tickTime = 600.f;
		double gameSpeed = 0.99f;
		int obstacle1Chance = 30;
		int obstacle2Chance = 5;
		while (*minigameIsRunning) {
			minigameMutex->lock();
			//minigame logic
			for (int i = game->size() - 1; i >= 0; i--) {
				for (int j = 0; j <game->at(i).size(); j++) {

					if (i == game->size()-1 && j == *carX) {
					//drawing car
						Pixel car;
						car.symb = '^';
						car.bgCol = col_darkGreen;
						car.fgCol = col_green;
						game->at(i)[j] = car;
					}
					else if (i == game->size() - 1) {
					//removing last line of obstacles
						Pixel empty;
						empty.symb = ' ';
						empty.bgCol = col_black;
						game->at(i)[j] = empty;
					}
					else {
					//transfering obstacles down by one line
						if (game->at(i)[j].symb == 'H') {
							game->at(i+1)[j] = game->at(i)[j];
							game->at(i)[j].symb = ' ';
							game->at(i)[j].bgCol = col_black;

						}
					}
				}
			}

			//checking for collision
			if (game->at(game->size() - 1)[*carX].symb == 'H') 
				*minigameIsRunning = false;

			//generating obstacles
			bool addObstacle1 = false;
			bool addObstacle2 = false;
			if (rand() % 100 <= obstacle1Chance) addObstacle1 = true;
			if (rand() % 100 <= obstacle2Chance) addObstacle2 = true;

			Pixel obstacle;
			obstacle.symb = 'H';
			obstacle.bgCol = col_red;
			obstacle.fgCol = col_darkRed;

			if (addObstacle1) game->at(0)[rand() % game->at(0).size()] = obstacle;
			if (addObstacle2) game->at(0)[rand() % game->at(0).size()] = obstacle;

			InsertArray(*y+1, *x+1, *game);


			minigameMutex->unlock();

			//controlling game speed
			Sleep(max((int)tickTime, 100));
			tickTime *= gameSpeed;
		}

	};

	vector<vector<Pixel>> game(height, vector<Pixel>(width));

	thread minigame(gameLoop, &minigameMutex, &game, &minigameIsRunning, &y, &x, &carX);

	while (minigameIsRunning) {
		if (KeyPressed("Left") && carX>0) {
			minigameMutex.lock();
			//check for collision
			if (game[game.size()-1][carX - 1].symb == 'H') minigameIsRunning = false;
			//move car
			else swap(game[game.size()-1][carX-1], game[game.size()-1][carX]);
			carX--;
			//apply update
			InsertArray(y+1, x+1, game);
			minigameMutex.unlock();

		}
		if (KeyPressed("Right") && carX < width - 1) {
			minigameMutex.lock();
			//check for collision
			if (game[game.size() - 1][carX + 1].symb == 'H') minigameIsRunning = false;
			//move car
			else swap(game[game.size() - 1][carX + 1], game[game.size() - 1][carX]);
			carX++;
			//apply update
			InsertArray(y+1, x+1, game);
			minigameMutex.unlock();

		}
		Sleep(1);
	}
	minigame.join();

	/*
		PLACE MINIGAME LOSE ANIMATION HERE
	*/
	mGameIsRunning = false;
}

void Core::Minigame2(int y, int x, bool *gameIsRunning){
	int height = 5, width = 10;
	

	while (*gameIsRunning) {
		if (KeyPressed("Right")) cout << "Right" << endl;
		else if (KeyPressed("Esc")) *gameIsRunning = false;
		Sleep(1);
	}
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
