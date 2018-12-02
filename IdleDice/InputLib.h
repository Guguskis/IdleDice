#pragma once
#include "pch.h"
using namespace std;

class InputLib
{
private:
	static string mPressedKey;
public:
	void GetInput();
	static bool KeyPressed(string);
	void Test();
	void TestKeys();
};

