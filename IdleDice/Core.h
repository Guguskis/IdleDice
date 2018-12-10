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
	int mHeight = 30, mWidth = 65;
	int mFPS = 60;

	bool mGameIsRunning = true;

	vector<pair<string, int>> mScores;

private:
	void WriteScore();
	void LoadScore();
	void InsertScore(int);
	void SetConsole();
	void StartMenu();

public:
	void Test();
	void Demo();
	void RunGameCore();
	void RunGame();
	Core();
};

