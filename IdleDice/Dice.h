#pragma once
#include "pch.h"
#include "GraphicsLib.h"

using namespace std;

class Dice
{
private: 

	static double *mAnimationTime;
	static int normalDices, boosterDices;
	/*
		Types:
			1. Normal
			2. Booster
	*/
	string mType="Normal";
	char mUpgradeKey;
	int mY=0, mX=0;

	int mAscencion=1;
	int mLevel=1;
	int mRolledValue=1;
	double mMultiplier=1.0f;

	int boosterMaxRollValue = 2;

	double normalBuyCoef = 1.08f;
	double boosterBuyCoef = 1.08f;

	vector<vector<vector<Pixel>>> mNormalGraphics, mBoosterGraphics;

	void ChangeThisCoords(int, int);
	string FormatNumber(double);

public:
	Dice(string, int, int, double, double*, char);
	Dice();

	static double RollAllDices(vector<Dice> *);


	void AnimateThisDice();
	void InitializeGraphics();

};
