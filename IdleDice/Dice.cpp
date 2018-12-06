#include "pch.h"
#include "Dice.h"

double* Dice::mAnimationTime = new double(1200.f);

/***********************STATIC METHODS****************************/

double Dice::RollAllDices(vector<Dice> *dices) {
	double totalSum = 0;

	//rolling dices
	for (int i = 0; i < dices->size(); i++) {
		dices->at(i).RollThisDice();
		totalSum += dices->at(i).getThisMoney();
	}

	//animating dices
	vector<thread> dicesWorker;
	for (int i = 0; i < dices->size(); i++) {
		dicesWorker.push_back(thread(&Dice::AnimateThisDice, dices->at(i)));
	}

	//waiting until all animation finishes
	for (int i = 0; i < dices->size(); i++) {
		dicesWorker[i].join();
	}

	return totalSum;
}



/***********************NON-STATIC METHODS****************************/

string Dice::FormatNumber(double value) {
	vector<string> postfixes(16);
	postfixes[0] = "";
	postfixes[1] = "K";
	postfixes[2] = "M";
	postfixes[3] = "B";
	postfixes[4] = "t";
	postfixes[5] = "q";
	postfixes[6] = "Q";
	postfixes[7] = "s";
	postfixes[8] = "S";
	postfixes[9] = "o";
	postfixes[10] = "n";
	postfixes[11] = "d";
	postfixes[12] = "U";
	postfixes[13] = "D";
	postfixes[14] = "T";
	postfixes[15] = "Qt";

	int powerCoef = 0;

	while (value >= 1000.f) {
		powerCoef++;
		value /= 1000.f;
	}
	//value = (int)(10*value)/10;
	string number = to_string(value);
	number = number.substr(0, 4);
	

	//removing unnecessary '.' symbol at the end
	if (number.back() == '.') {
		number.pop_back();
	}
	//removing unnecessary fraction at the end for numbers < 1000 
	else if (powerCoef == 0 && number.length() >= 3) {
		for (int i = 0; i < number.length(); i++) {
			if (number[i] == '.') {
				while (i != number.length()) {
					number.pop_back();
				}
			}
		}
	}

	if(powerCoef<postfixes.size())
		number += postfixes[powerCoef];
	else number = "TooBig";

	//normalizing number length
	for (int i = 0; number.length() < 5; i++) {
		if (i % 2 == 0) number.insert(number.begin(), ' ');
		else number.push_back(' ');
	}
	while (number.length() < 5) number += " ";
	return number;
}

void Dice::ChangeThisCoords(int y, int x) {
	mY = y;
	mX = x;
}
void Dice::AnimateThisDice() {
	//offset values for rolled value above the dice
	int offsetY = -2;
	int offsetX = mNormalGraphics[mRolledValue][0].size()/2;

	//initializing time variables
	auto startTime = chrono::steady_clock::now();
	chrono::milliseconds elapsedTime;

	//this variable is used to prevent from generating same random value twice in a row
	int lastRolledValue = rand() % mNormalGraphics.size();
	
	//hidding rolled value
	GraphicsLib::InsertLine(mY + offsetY, mX + offsetX - 2, "     ", col_noColor, col_yellow);


	do {
		//preventing same value from being generated
		int rolledValue;
		do {
			rolledValue = rand() % mNormalGraphics.size();
		} while (rolledValue == lastRolledValue);
		lastRolledValue = rolledValue;

		//inserting animation frame
		GraphicsLib::InsertArray(mY, mX, mNormalGraphics[lastRolledValue]);

		Sleep(90);

		elapsedTime = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - startTime);
		//inserting rolled value
		if (elapsedTime.count() > *mAnimationTime / 5*4) {
			GraphicsLib::InsertLine(mY +offsetY, mX + offsetX-2, FormatNumber(getThisMoney()), col_noColor, col_yellow);
		}
	} while (elapsedTime.count() < *mAnimationTime);
	
	//inserting true value frame
	GraphicsLib::InsertArray(mY, mX, mNormalGraphics[mRolledValue - 1]);
}
void Dice::RollThisDice() {
	mRolledValue = rand() % 5 + 1;
}
double Dice::getThisMoney() {
	return round(mMultiplier*mRolledValue);
}

Dice::Dice(string type, int y, int x, double multiplier, double *animationTime, char upgradeKey) {
	mType = type;
	mY = y;
	mX = x;
	mMultiplier = multiplier;
	mAnimationTime = animationTime;
	mUpgradeKey = upgradeKey;

	InitializeGraphics();
}
Dice::Dice() {
	InitializeGraphics();
}
void Dice::InitializeGraphics(){
	/*TODO
		add booster dice graphics
	*/
	int normalDiceColor = col_pink;

	vector<vector<Pixel>> temp(7, vector<Pixel>(11));

	if (mType == "Normal") {
		//iterating all 6 normal dices
		for (int k = 0; k < 6; k++) {
			//creating outline
			for (int i = 0; i < 7; i++) {
				for (int j = 0; j < 11; j++) {

					if (i == 0 && j == 0) temp[i][j].symb = '\xc9';
					else if (i == 0 && j == 10) temp[i][j].symb = '\xbb';
					else if (i == 6 && j == 0) temp[i][j].symb = '\xc8';
					else if (i == 6 && j == 10) temp[i][j].symb = '\xbc';
					else if (i == 0 || i == 6) temp[i][j].symb = '\xcd';
					else if (j == 0 || j == 10) temp[i][j].symb = '\xba';
					else temp[i][j].symb = ' ';
					temp[i][j].bgCol = col_black;
					temp[i][j].fgCol = normalDiceColor;
				}
			}
			//filling in dots
			if (k == 0) {
				temp[3][5].symb = '\xfe';
			}
			else if (k == 1) {
				temp[1][1].symb = '\xfe';
				temp[5][9].symb = '\xfe';
			}
			else if (k == 2) {
				temp[1][1].symb = '\xfe';
				temp[3][5].symb = '\xfe';
				temp[5][9].symb = '\xfe';
			}
			else if (k == 3) {
				temp[1][1].symb = '\xfe';
				temp[1][9].symb = '\xfe';
				temp[5][1].symb = '\xfe';
				temp[5][9].symb = '\xfe';
			}
			else if (k == 4) {
				temp[1][1].symb = '\xfe';
				temp[1][9].symb = '\xfe';
				temp[5][1].symb = '\xfe';
				temp[5][9].symb = '\xfe';
				temp[3][5].symb = '\xfe';
			}
			else if (k == 5) {
				temp[1][1].symb = '\xfe';
				temp[1][9].symb = '\xfe';
				temp[3][1].symb = '\xfe';
				temp[3][9].symb = '\xfe';
				temp[5][1].symb = '\xfe';
				temp[5][9].symb = '\xfe';
			}

			mNormalGraphics.push_back(temp);
		}


	}

}
