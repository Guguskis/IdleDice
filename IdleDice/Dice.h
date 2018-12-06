#pragma once
#include "pch.h"
#include "GraphicsLib.h"

using namespace std;

class Dice
{
private: 

	static double *mAnimationTime;
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

	double buyCoef = 1.08f;

	vector<vector<vector<Pixel>>> mNormalGraphics, mBoosterGraphics;

	void ChangeThisCoords(int, int);
	string FormatNumber(double);

public:
	Dice(string, int, int, double, double*, char);
	Dice();

	static double RollAllDices(vector<Dice> *);

	double getThisMoney();
	void RollThisDice();
	void AnimateThisDice();
	void InitializeGraphics();

};
