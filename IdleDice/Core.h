#pragma once
#include <thread>




#include "GraphicsLib.h"

class Core
{
private:
	int mHeight = 20, mWidth = 30, mLayers = 2;
	int mCurrLayer=0;
	bool mGameIsRunning = true;




private:

public:
	void HandleGraphics();
	void Test();
	void GameLoop();
	Core();
};

