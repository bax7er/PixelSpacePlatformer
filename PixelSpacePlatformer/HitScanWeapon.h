#pragma once
#ifndef PROJECTILEWEAPON_H
#define PROJECTILEWEAPON_H
#include "Weapon.h"

using namespace std;
class HitscanWeapon :public Weapon {
private:
	
public:
	bool firing;
	float range;
	GLuint beamTexture;
	GLuint impactTexture;
	Point endPoint;
	Point getMaxPoint();
	Point getCollisionPoint(Box target);
	Point getCollisionPoint(Box target,Point max);
	bool checkHit(Box target,Point max);
	bool checkHitUpdate(Box target);
	void weapDraw(Point &mount);
	HitscanWeapon(float spawnXpos, float spawnYpos, float xSize, float ySize, Projectile &ammo);
	HitscanWeapon();
	void attack();
};
#endif