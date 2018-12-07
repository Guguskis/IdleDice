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
	thread workerMinigame2(&Core::Minigame2, this, 0, 8, &mGameIsRunning);

	workerInput.join();
	workerMinigame1.join();
	workerMinigame2.join();
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
	vector<vector<Pixel>> game(height, vector<Pixel>(width));

	//Draw frame around minigame window
	for (int i = 0; i < height+2; i++) {
		for (int j = 0; j < width+2; j++) {
			if (i == 0 || i == height + 1 || j == 0 || j == width + 1) {
				InsertPixel(i, j, ' ', col_cyan, col_noColor);
			}
		}
	}

	//Main minigame logic
	auto gameLoop = [&gameIsRunning](mutex *minigameMutex, vector<vector<Pixel>> *game, bool *minigameIsRunning, int *y, int *x, int *carX) {
		double tickTime = 600.f;
		double gameSpeed = 0.99f;
		int obstacle1Chance = 25;
		int obstacle2Chance = 3;
		while (*minigameIsRunning && *gameIsRunning) {
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
			if (game->at(game->size() - 1)[*carX].symb == 'H') {
				*minigameIsRunning = false;
				game->at(game->size() - 1)[*carX].fgCol = col_yellow;
				game->at(game->size() - 1)[*carX].bgCol = col_darkYellow;
			}

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


	thread minigame(gameLoop, &minigameMutex, &game, &minigameIsRunning, &y, &x, &carX);

	//controlling car
	while (minigameIsRunning && *gameIsRunning) {
		if (KeyPressed("Left") && carX>0) {
			minigameMutex.lock();
			//check for collision
			if (game[game.size() - 1][carX - 1].symb == 'H') {
				minigameIsRunning = false;
				game[game.size() - 1][carX].fgCol = col_yellow;
				game[game.size() - 1][carX].bgCol = col_darkYellow;
			}
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
			if (game[game.size() - 1][carX + 1].symb == 'H') {
				minigameIsRunning = false;
				game[game.size() - 1][carX].fgCol = col_yellow;
				game[game.size() - 1][carX].bgCol = col_darkYellow;
			}
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
	mutex minigameMutex;
	bool minigameIsRunning = true;

	int height = 3, width = 25;
	int lives = 5;
	int greenLines = 3;

	Pixel pixel;
	pixel.fgCol = col_white;
	pixel.symb = ' ';

	vector<vector<Pixel>> game(height, vector<Pixel>(width, pixel));

	//Draw frame around minigame window
	for (int i = 0; i < height + 2; i++) {
		for (int j = 0; j < width + 2; j++) {
			if (i == 0 || j == 0 || i == height + 1 || j == width + 1) {
				InsertPixel(i+y, j+x, ' ', col_yellow, col_red);
			}
		}
	}

	
	auto gameLoop = [&gameIsRunning, &greenLines](mutex *minigameMutex, vector<vector<Pixel>> *game, bool *minigameIsRunning, int *y, int *x, int *lives) {
		double tickTime = 300.f;
		double gameSpeed = 0.99f;
		int noteChance = 4;
		int maxLives = *lives;

		//drawing green lines and changing foreground color
		for (int i = 0; i < game->size(); i++) {
			for (int j = 0; j < greenLines; j++) {
				game->at(i)[j+1].bgCol = col_green;
			}
			game->at(i)[0].fgCol = col_pink;
			game->at(i)[1].fgCol = col_red;
			game->at(i)[2].fgCol = col_yellow;
		}

		int ticksPassed = 0;
		while (*gameIsRunning && *minigameIsRunning) {
			minigameMutex->lock();
			//minigame logic
			for (int i = 0; i < game->size(); i++) {
				for (int j = 0; j < game->at(i).size(); j++) {

					//checking first column
					if (j == 0) {
						if (game->at(i)[0].symb !=' ') {
							game->at(i)[j].symb = ' ';
							*lives = *lives - 1;
						}
					}
					//shifting everything to the left
					else {
						if (game->at(i)[j].symb != ' ') {
							game->at(i)[j - 1].symb = game->at(i)[j].symb;
							game->at(i)[j].symb = ' ';
						}
					}
				}
			}
			
			//generating notes
			bool addNote = false;
			if (ticksPassed % noteChance == 0) addNote = true;

			if (addNote) {
				int noteY = rand() % game->size();
				game->at(noteY)[game->at(0).size() - 1].symb = '1' + noteY;
				game->at(noteY)[game->at(0).size() - 1].fgCol = col_white;
				
			}

			InsertArray(*y + 1, *x + 1, *game);

			//draw live counter
			string text = "Lives ";
			InsertLine(*y, *x, text, col_noColor, col_grey);

			for (int i = 1; i <= maxLives; i++) {
				if (i <= *lives) InsertPixel(*y, *x + i - 1 + text.length(), 'V', col_noColor, col_darkRed);
				else InsertPixel(*y, *x + i - 1+text.length(), ' ', col_noColor, col_red);
			}

			if (*lives <= 0) 
				*minigameIsRunning = false;

			minigameMutex->unlock();
			
			//controlling game speed
			Sleep(max((int)tickTime, 150));
			tickTime *= gameSpeed;
			ticksPassed++;
		}
	};
	
	
	thread minigame(gameLoop, &minigameMutex, &game, &minigameIsRunning, &y, &x, &lives);

	//controlling minigame
	while (minigameIsRunning && *gameIsRunning) {
		bool keyWasPressed = false;
		bool noteHit = false;
		int noteX=0, noteY=0;

		//checking all pressed keys
		for (int i = 0; i < height; i++) {
			string key = "0";
			key[0] += i+1;
			//if key was pressed, record note coordinates
			if (KeyPressed(key)) {
				keyWasPressed = true;

				minigameMutex.lock();
				for (int j = 1; j <=greenLines  && !noteHit; j++) {
					if (game[i][j].symb != ' ') {
						game[i][j].symb = ' ';
						noteHit = true;
						noteY = i;
						noteX = j;
					}
				}
				minigameMutex.unlock();
			}
		}

		//handle input
		if (keyWasPressed) {
			minigameMutex.lock();

			if (!noteHit) lives--;
			else {
				InsertArray(y + 1, x + 1, game);
				InsertPixel(y + noteY + 1, x + noteX + 1, ' ', col_darkGreen, col_grey);
			}
			minigameMutex.unlock();
		}
		Sleep(1);
	}

	minigame.join();

	/*Insert game over animation here*/


	mGameIsRunning = false;
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
