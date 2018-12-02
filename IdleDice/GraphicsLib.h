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


	static void HandleLine(int, int, string, int, int);
	static void HandleTextBox(int, int, string, int, int, int, int);
	static void HandleArray(int, int, const vector<vector<Pixel>> *);

	static void HandleCommands();
	static void DrawPixel(int, int, Pixel *);
	static void InsertPixel(int, int, char, int, int);
	static bool NeedUpdatePixel(const Pixel *, const Pixel *);

public:
	static void InsertLine(int, int, string, int, int);
	static void InsertTextBox(int, int, string, int, int, int, int);
	static void InsertArray(int, int, vector<vector<Pixel>> *);

	static void Test();
	static void DrawScreen();
	static void ClearScreen();

	static void SetData(int, int);
};

