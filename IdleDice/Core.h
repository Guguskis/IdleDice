#pragma once
#include "pch.h"
#include "GraphicsLib.h"

class Core
{
private:
	int mHeight = 40, mWidth = 60, mLayers = 2;
	int mCurrLayer=0;
	bool mGameIsRunning = true;




private:

public:
	void HandleGraphics();
	void Test();
	void GameLoop();
	Core();
};

