#pragma once
#include "pch.h"
#include "MinigameEngine.h"


class MinigameLaser :
	private MinigameEngine
{
private:
	int lives = 8;
	
	bool laserOn = true;
	int laserCol = col_red;
	
	char goodObject = 'Y'; //must burn
	char badObject = 'X';  //must fall
	int goodCol = col_red;
	int badCol = col_green;

	int obstacleFrequency = 7;



	void GameControl();
	void GameLogic();
public:
	void Run();

	MinigameLaser(int, int, int, int, int, double, double, bool *);
	MinigameLaser();
	~MinigameLaser();
};

