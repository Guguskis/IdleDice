#pragma once
#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

#include "DataStructures.h"

class GraphicsLib
{
private:
	/*
		About mCurrFrame and mLastFrame:
			in the first index is stored layer, f.e.:
				0 - main game screen, 
				1 - shop,
				2 - settings,
				...
			this way I can display more information without worrying when should I draw what
	*/
	static vector<vector<vector<Pixel>>> *mCurrFrame, *mLastFrame;
	static int* mCurrLayer;
	static int mLastLayer;
	static void DrawPixel(int, int, char, int, int);
	static bool NeedUpdatePixel(int, int, char, int, int);

public:
	static void SetData(int, int, int, int*);
	static void DrawScreen();

	static void Test();
};

