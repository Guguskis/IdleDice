#pragma once
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <string>
#include <vector>

using namespace std;

class InputLib
{
private:
	string mPressedKey;
protected:
	void GetInput();
	bool KeyPressed(string);
public:
	void Test();
	void TestKeys();
};

