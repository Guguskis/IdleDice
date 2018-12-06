#pragma once
#include "pch.h"
#include "GraphicsLib.h"
#include "InputLib.h"
#include "Dice.h"

class Core:
	private GraphicsLib,
	private InputLib,
	private Logging
{
private:
	int mHeight = 30, mWidth = 90;
	int mFPS = 60;

	double mAnimationTime = 1200.f;
	bool mGameIsRunning = true;

private:

public:
	void HandleInput();
	void Test();
	void Demo();
	void Run();
	Core();
};

