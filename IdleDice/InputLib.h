#pragma once
#include "pch.h"
using namespace std;

class InputLib
{
private:
	static string mPressedKey;
	static mutex mMutex;
	static bool mKeyRead;
public:
	static void GetInput();
	static bool KeyPressed(string);
	void Test();
	void TestKeys();
};

