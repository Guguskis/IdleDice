#pragma once
#include "pch.h"
#include "DataStructures.h"
#include "GraphicsLib.h"
#include "InputLib.h"

using namespace std;
class MinigameEngine:
	protected GraphicsLib,
	protected InputLib
{

protected: 
	mutex mMutex;
	bool mIsRunning = false;
	bool *mGameEngineIsRunning;

	int mY = 0, mX = 0;
	int mHeight = 3, mWidth = 3;

	int mFrameColor = col_grey;
	vector<vector<Pixel>> mGame;
	
	double mTickTime = 600.f;
	double minTickTime = 50.f;
	double mTickSpeed = 0.97f;
	int mTicksPassed = 0;

	int mStartTimer=5;

protected:
	void DrawFrame();
	void UpdateGraphics();
	void DrawGuide(string, string, int);
	void GameOverAnimation();
	void Starter();

public:
	void MinigameTest();
	MinigameEngine(int, int, int, int, int, double, double, bool *);
	MinigameEngine();
	~MinigameEngine();
};

