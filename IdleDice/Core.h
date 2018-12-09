#pragma once
#include "pch.h"
#include "GraphicsLib.h"
#include "InputLib.h"

#include "MinigameCars.h"
#include "MinigamePianoTiles.h"
#include "MinigameLaser.h"

class Core:
	private GraphicsLib,
	private InputLib,
	private Logging
{
private:
	int mHeight = 30, mWidth = 60;
	int mFPS = 60;


	bool mGameIsRunning = true;


private:
	void SetConsole();
	void GameOverAnimation(int, int, int, int, int, int);
	void Minigame3(int, int, bool*);


public:
	void Test();
	void Demo();
	void Run();
	Core();
};

