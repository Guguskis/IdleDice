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
	int mHeight = 29, mWidth = 69;
	int mFPS = 60;

	bool mGameIsRunning = true;




private:

public:
	void HandleInput();
	void Test();
	void Demo();
	Core();
};

