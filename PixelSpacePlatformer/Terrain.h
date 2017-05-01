
#ifndef TERRAIN_H
#define TERRAIN_H
//#include "Player.h"
//#include "AiPlayer.h"
#include "Effect.h"
class Terrain : public GameObject{
private:
	bool ground;
public:
	bool isDestroyable = false;
	bool isDestroyed = false;
	Effect destroyedEffect;
	inline bool isGround() { return ground; };
	inline void setGround(bool state) { ground = state; };
	Terrain();
	Terrain(float spawnXpos, float spawnYpos, float xSize, float ySize);
	//bool checkCollision(GameObject &collisionObject);
	//bool checkCollision(GameObject &collisionObject, float xShift, float yShift);
	//bool checkCollision(Player &collisionObject, Point &translation);
	//bool checkCollision(AiPlayer &collisionObject, Point &translation);
	bool checkOnGround(Box &box);
	//bool checkOnGround(AiPlayer &player);
	bool checkOnGround(GameObject &player);
};
#endif