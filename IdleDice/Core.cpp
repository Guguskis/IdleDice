#include "pch.h"
#include "Core.h"


/*********************WORKING ON*********************/

void Core::RunGameCore() {
	auto runInput = [](bool* mGameIsRunning) {
		while (*mGameIsRunning) {
			GetInput();
			if (KeyPressed("Esc")) *mGameIsRunning = false;
		}
	};
	thread workerInput(runInput, &mGameIsRunning);

	MinigameCars cars(0, 0, 20, 10, col_green, 400.f, 0.98f, &mGameIsRunning);
	MinigamePianoTiles pianoTiles(0, 10, 6, 30, col_cyan, 400.f, 0.97f, &mGameIsRunning);
	MinigameLaser laser(6, 10, 14, 30, col_yellow, 400.f, 0.98f, &mGameIsRunning);

	thread carsWorker(&MinigameCars::Run, &cars);
	thread pianoTilesWorker(&MinigamePianoTiles::Run, &pianoTiles);
	thread laserWorker(&MinigameLaser::Run, &laser);

	carsWorker.join();
	pianoTilesWorker.join();
	laserWorker.join();
	workerInput.join();

}

void Core::StartMenu(){

}
/*********************FINISHED*********************/
void Core::Test(){
	auto runInput = [](bool* mGameIsRunning) {
		while (*mGameIsRunning) {
			GetInput();
			if (KeyPressed("Esc")) *mGameIsRunning = false;
		}
	};
	thread workerInput(runInput, &mGameIsRunning);
	/*********************************/
	//InsertScore(50);

	
	MinigameCars cars(0, 0, 20, 10, col_green, 600.f, 0.98f, &mGameIsRunning);
	thread carsWorker(&MinigameCars::Run, &cars);
	carsWorker.join();
	

	



	/*********************************/
	workerInput.join();
}



void Core::InsertScore(int score){
	vector<string> names = { "Fred", "Paul", "Stan Lee", "Gaben", "Tony", "Princess", "10?", "DISQUALIFIED", "Jake", "Esmeralda", "UNO", "To Isengard", "Nope", "k." };
	
	string name = names[rand()%names.size()];

	string gzMsg = "Congratulations, " + name + " your score is " + to_string(score);

	int boxHeight = 6;
	int boxWidth = gzMsg.length()+4;
	int boxY = (mHeight - boxHeight) / 2;
	int boxX = (mWidth - boxWidth) / 2;


	int originCol = col_blue;
	int shiningCol = col_darkBlue;
	//Displaying message
	InsertTextBox(boxY, boxX, "", boxHeight, boxWidth, col_red, col_red);
	InsertTextBox(boxY+1, boxX+1, "", boxHeight-2, boxWidth-2, col_black, col_black);

	InsertTextBox(boxY+boxHeight/2-1, boxX + 2, gzMsg, boxHeight-2, gzMsg.length(), col_noColor, originCol);

	//Shining text
	bool isRunning = true;
	auto textShine = [boxX, boxY, boxHeight, boxWidth, &isRunning, gzMsg, originCol, shiningCol]() {
		while (isRunning) {
			for (int i = 0; i < gzMsg.length()+1; i++) {
				if(i<gzMsg.length())
					InsertPixel(boxY + boxHeight / 2 - 1, boxX + 2 + i, symb_noSymbol, col_noColor, shiningCol);
				if (i >= 1)
					InsertPixel(boxY + boxHeight / 2 - 1, boxX + 2 + i-1, symb_noSymbol, col_noColor, originCol);
				
				if(isRunning)
					Sleep(40);
			}

			//waiting 1400ms, splitted in chuncks for faster respond
			auto start = chrono::steady_clock::now();
			auto end = chrono::steady_clock::now();
			auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);
			while (elapsed.count() < 1400) {
				Sleep(40);
				end = chrono::steady_clock::now();
				elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);
			}
		}

	};

	thread shiningWorker(textShine);

	while (isRunning) {
		if (KeyPressed("Space"))
			isRunning = false;
		if (isRunning)
			Sleep(40);

	}


	shiningWorker.join();


	//Inserting score
	mScores.push_back(make_pair(name, score));
	auto sortBySecond = [](pair<string, int> &a, pair<string, int> &b) {
		return (a.second > b.second);
	};
	sort(mScores.begin(), mScores.end(), sortBySecond);

}
void Core::WriteScore(){
	ofstream os("Scores.txt");
	
	for (int i = 0; i < mScores.size(); i++) {
		os << mScores[i].first << " " << mScores[i].second << endl;
	}

	os.close();

}
void Core::LoadScore(){
	ifstream is("Scores.txt");

	if (!is.good()) return;

	mScores.clear();
	while (!is.eof()) {
		string name;
		int score;
		is >> name >> score;
		mScores.push_back(make_pair(name, score));
	}

	auto sortBySecond = [](pair<string, int> &a, pair<string, int> &b) {
		return (a.second > b.second); 
	};
	sort(mScores.begin(), mScores.end(), sortBySecond);


	is.close();
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
