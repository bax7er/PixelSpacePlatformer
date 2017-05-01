#ifndef AIPLAYER_H
#define AIPLAYER_H
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
	int spritesOnSheet = 11;
	float animationFrameRate = 0.2;
	int maxHP = 100000;
public:
	double distanceMovedX = 0;
	bool alive = true;
	int healthPoints = 100000;
	static double clock;
	bool onGround = true;
	Point react(float fOffest,Player player);
	void drawAiPlayer();
	void animationUpdate(bool reversed);
	void moveToDefault();
	void nextFrame() { texturePos++; texturePos %= spritesOnSheet; };
	void move(Point translation);
	AiPlayer();
	AiPlayer(float spawnXpos, float spawnYpos, float xSize, float ySize);
	bool checkCollision(GameObject &collisionObject, float xShift, float yShift);
	bool checkCollision(Player &collisionObject, Point &translation);
	bool checkOnTop(GameObject &player);
};
#endif