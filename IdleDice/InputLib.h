#pragma once
#include "pch.h"
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

