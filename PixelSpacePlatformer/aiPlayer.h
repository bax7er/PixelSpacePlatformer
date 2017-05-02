#ifndef AIPLAYER_H
#define AIPLAYER_H
#define AIMODE_STATIC 0
#define AIMODE_WALK 1
#define AIMODE_FLY 2
#include "Player.h"
#include <vector>
#include "Weapon.h";
#include "Image_Loading/nvImage.h"
#include <iostream>

using namespace std;
class AiPlayer : public GameObject {
private:
	
	int texturePos = 0;
	bool faceLeft = true;
	double lastUpdate = 0;
	
	float animationFrameRate = 0.2;
	
public:
	bool canShoot = true;
	int maxHP = 1000;
	int spritesOnSheet = 11;
	int aiMode = AIMODE_WALK;
	int pointsForKill = 100;
	int pointsForCritKill = 200;
	Weapon * gun;
	bool hasCritical = true;
	Box criticalRegion;
	double distanceMovedX = 0;
	bool alive = true;
	int healthPoints = 1000;
	static double clock;
	bool onGround = true;
	Point weaponMount = Point(0, 0);
	Point react(float fOffest,Player player);
	void drawAiPlayer();
	void animationUpdate(bool reversed);
	void moveToDefault();
	void nextFrame() { texturePos++; texturePos %= spritesOnSheet; };
	void move(Point translation);
	AiPlayer();
	AiPlayer(float spawnXpos, float spawnYpos, float xSize, float ySize);
	bool checkCollision(GameObject &collisionObject, float xShift, float yShift);
	bool checkOnTop(GameObject &player);
};
#endif