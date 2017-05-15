#ifndef PROJECTILE_H
#define PROJECTILE_H
#include "GameObject.h"
#include "Effect.h"
class Projectile : public GameObject {
private:
	Point heading = Point(0,1);
	double speedMultiplier = 0.015;
	double distantToLive = 6;
	double maxDistance = 6;
public:
	GLuint effectTexture;
	bool active = false;
	Projectile(float spawnXpos, float spawnYpos, float xSize, float ySize, float angle);
	Projectile(float spawnXpos, float spawnYpos, float angle, Projectile& old);
	Projectile();
	void generateHeading(float angle);
	void updatePos(double foffset);
	Effect collisionAction();
};
#endif