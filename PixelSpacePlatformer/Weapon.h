#ifndef WEAPON_H
#define WEAPON_H
#include "Projectile.h"
#include <SDL_mixer.h>
#include <vector>
#include <iostream>
using namespace std;
class Weapon : public GameObject {
private:
	Point bindPoint = Point(0,0);
	//Point projectileSpawn = Point(0, 0);
	int maxProjectiles = 10000;
	int currentProjectiles = 0;
	Projectile sample;
	vector<Projectile> clip;
	bool mirror_texture = false;
	float initSpawnX, initSpawnY;
	float initSpawnerX=0, initSpawnerY =0;
	double currentAngle;
public:
	Point projectileSpawn = Point(0, 0);
	float weaponDamage;
	Mix_Chunk *explosion = NULL;
	vector<Effect> effects;
	float clock;
	float lastUpdate;
	Weapon(float spawnXpos, float spawnYpos, float xSize, float ySize,Projectile &ammo);
	Weapon();
	inline void mirror(bool value) {
		if (mirror_texture != value) {
			mirror_texture = value;
			initSpawnerY = ((initSpawnY + (basicBox.getYSize()/2) ) - initSpawnerY) + (initSpawnY - (basicBox.getYSize() / 2)); // Corrects fire position
		}
	}
	void setBinding(float x, float y);
	void setBinding();
	void setSpawner(float x, float y);
	void setSpawner();
	void rebind(Point &mount);
	void weapDraw(Point &mount);
	void rotateWeapon(float angle);
	void resetPos();
	Point attack();
	void updateProjectiles(Point &tranformation, double fOffset);
	bool checkProjectileCollision(Box &collision, vector <Effect> &effectList);
	void effectUpdate(int counter);
	void aim(double angle);
};
#endif