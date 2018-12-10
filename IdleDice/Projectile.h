#pragma once
#include "pch.h"

using namespace std;

class Projectile
{



private:
	static int projectileCount;
	string type;

	int maxSpeed = 300;
	int minSpeed = 150;
	int speed = 0;

	int xPoz=0;
	int yPoz=0;

	int xVel = 0;
	int yVel = 0;

public:
	Projectile();
	~Projectile();
};

