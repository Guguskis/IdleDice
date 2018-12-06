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
	int mHeight = 30, mWidth = 30;
	int mFPS = 60;

	bool mGameIsRunning = true;




private:

public:
	void HandleGraphics();
	void HandleInput();
	void Test();
	Core();
};

