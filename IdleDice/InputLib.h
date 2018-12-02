#pragma once
#include "pch.h"
using namespace std;

class InputLib
{
private:
	string mPressedKey;
protected:
public:
	void GetInput();
	bool KeyPressed(string);
	void Test();
	void TestKeys();
};

