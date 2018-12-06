#include "pch.h"
#include "InputLib.h"

string InputLib::mPressedKey;
mutex InputLib::mMutex;
bool InputLib::mKeyRead = false;

void InputLib::TestKeys() {
	while (1) {
		int firstVal = _getch();
		int secondVal = _getch();
		cout << "First value: " << firstVal << "\nSecond value: " << secondVal << "\n\n";
	}
}
void InputLib::Test() {
	while (1) {
		GetInput();
		cout << mPressedKey << endl;
	}
}
bool InputLib::KeyPressed(string key) {
	lock_guard<mutex> lock(mMutex);
	if (mPressedKey == key && !mKeyRead) {
		mKeyRead = true; 
		return true;
	}
	else return false;
}
void InputLib::GetInput() {
	enum KeyCodes {
		//Function keys
		KEY_FUNCTION = 0,
		KEY_F1 = 59,
		KEY_F5 = 63,
		KEY_F6 = 64,
		KEY_F9 = 67,
		KEY_F10 = 68,
		//Arrow keys
		KEY_ARROW = 224,
		KEY_UP = 72,
		KEY_LEFT = 75,
		KEY_RIGHT = 77,
		KEY_DOWN = 80,
		//Command keys
		KEY_ENTER = 13,
		KEY_ESC = 27,
		KEY_SPACE = 32,
	};

	int firstValue = _getch();
	int secondValue = _getch();

	//locking mutex AFTER reading keyboard input
	lock_guard<mutex> lock(mMutex);
	/*
		Q: Why I sometimes call this method recursively?
		A: To iterate this method until I get specified input without continuing any
		outside loops. For example, if I get unspecified input, then I won't change
		mPressedKey value, but will exit this method. Thus, the last command will be
		executed again in the next program loop. This is unwanted behaviour.
	*/

	int keySuccesfullyRead = true;
	//Reading function keys
	if (firstValue == KEY_FUNCTION) {

		switch (secondValue) {
		case KEY_F1: mPressedKey = "F1"; break;
		case KEY_F5: mPressedKey = "F5"; break;
		case KEY_F6: mPressedKey = "F6"; break;
		case KEY_F9: mPressedKey = "F9"; break;
		case KEY_F10: mPressedKey = "F10"; break;
		default:keySuccesfullyRead = false;
		}

	}
	//Reading arrow keys
	else if (firstValue == KEY_ARROW) {

		switch (secondValue) {
		case KEY_UP: mPressedKey = "Up"; break;
		case KEY_LEFT: mPressedKey = "Left"; break;
		case KEY_RIGHT: mPressedKey = "Right"; break;
		case KEY_DOWN: mPressedKey = "Down"; break;
		default:keySuccesfullyRead = false;
		}

	}
	//Reading command keys
	else if (firstValue == KEY_ENTER) { mPressedKey = "Enter"; }
	else if (firstValue == KEY_ESC) { mPressedKey = "Esc"; }
	else if (firstValue == KEY_SPACE) { mPressedKey = "Space"; }
	//Reading normal keys
	else if (firstValue == '0') { mPressedKey = "0"; }
	else if (firstValue == '1') { mPressedKey = "1"; }
	else if (firstValue == '2') { mPressedKey = "2"; }
	else if (firstValue == '3') { mPressedKey = "3"; }
	else if (firstValue == '4') { mPressedKey = "4"; }
	else if (firstValue == '5') { mPressedKey = "5"; }
	else if (firstValue == '6') { mPressedKey = "6"; }
	else if (firstValue == '7') { mPressedKey = "7"; }
	else if (firstValue == '8') { mPressedKey = "8"; }
	else if (firstValue == '9') { mPressedKey = "9"; }
	else keySuccesfullyRead = false;

	if (keySuccesfullyRead)
		mKeyRead = false;
}
