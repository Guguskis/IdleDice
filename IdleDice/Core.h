#pragma once
#include "pch.h"
#include "GraphicsLib.h"
#include "InputLib.h"
class Core:
	private GraphicsLib,
	private InputLib,
	private Logging
{
private:
	int mHeight = 10, mWidth = 10;
	int mFPS = 30;

	bool mGameIsRunning = true;




private:

public:
	void HandleGraphics();
	void HandleInput();
	void Test();
	void GameLoop();
	Core();
};

