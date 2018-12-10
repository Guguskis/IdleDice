#include "pch.h"
#include "Projectile.h"

int Projectile::projectileCount = 0;

Projectile::Projectile(){
	projectileCount++;
	speed = rand() % abs(maxSpeed - minSpeed) + minSpeed;
}


Projectile::~Projectile(){
	projectileCount--;
}
