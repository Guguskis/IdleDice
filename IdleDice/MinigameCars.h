#pragma once
#include "pch.h"
#include "MinigameEngine.h"


class MinigameCars:
	private MinigameEngine
{
private:
	int carX=0;
	int obstacleFrequency = 6;
	int extraObstacleChance = 15;

	void GameControl();
	void GameLogic();

public:

	void Run();
	MinigameCars(int, int, int, int, int, double, double, bool *);
	MinigameCars();
	~MinigameCars();
};

