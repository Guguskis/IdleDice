#pragma once
#include "pch.h"
#include "MinigameEngine.h"


class MinigamePianoTiles :
	private MinigameEngine
{
private:
	int noteFrequency = 8;
	int lives = 9;
	int maxLives = lives;
	int greenLines = 3;

	void GameControl();
	void GameLogic();

public:
	void Run();
	MinigamePianoTiles(int, int, int, int, int, double, double, bool *);
	MinigamePianoTiles();
	~MinigamePianoTiles();
};

