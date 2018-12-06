#pragma once
#include "pch.h"
#include "DataStructures.h"
#include "Logging.h"

class GraphicsLib
{
private:

	static vector<Command> mCommand;
	static vector<vector<Pixel>> *mCurrFrame, *mLastFrame;
	static int mHeight, mWidth;
	static bool mCommandInUse;
	static bool mFrameRateOn;
	static mutex mMutexCommand;
	static int mYoffset, mXoffset;



	static void HandleCommands();
	static void HandleClearScreen();
	static void HandleArray(int, int, const vector<vector<Pixel>>);
	static void HandleTextBox(int, int, string, int, int, int, int);
	static void HandleLine(int, int, string, int, int);
	static void HandlePixel(int, int, char, int, int);

	static void DrawPixel(int, int, Pixel *);
	static bool NeedUpdatePixel(const Pixel *, const Pixel *);

public:
	static void InsertClearScreen();
	static void InsertArray(int, int, vector<vector<Pixel>>);
	static void InsertTextBox(int, int, string, int, int, int, int);
	static void InsertLine(int, int, string, int, int);
	static void InsertPixel(int, int, char, int, int);

	static void Test();
	static void HandleGraphics(int*, bool*);
	static void DrawScreen();

	static void SetData(int, int);
};

