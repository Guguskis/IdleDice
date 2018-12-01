#pragma once
#include "pch.h"
#include "DataStructures.h"
#include "Logging.h"

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
	static int mLastLayer, mHeight, mWidth;
	

	static void DrawPixel(int, int, char, int, int);
	static void InsertPixel(int, int, int, char, int, int);
	static bool NeedUpdatePixel(int, int, char, int, int);

public:
	static void InsertTextBox(int, int, int, string, int, int, int, int);
	static void SetData(int, int, int, int*);
	static void DrawScreen();

	static void Test();
};

