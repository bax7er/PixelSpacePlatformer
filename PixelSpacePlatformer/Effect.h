#ifndef EFFECT_H
#define EFFECT_H
#define PLAYONCE 0
#define LOOP 1
#include "GameObject.h"
class Effect : public GameObject {
private:
	int frameAdvance = 1;

public:
	bool active;
	int tickRate=10;
	int ticksWaited = 0;
	int loopType = PLAYONCE;
	int currentFrame;
	float framesInSet;
	Effect();
	Effect(float spawnXpos, float spawnYpos, float xSize, float ySize, GLuint sprites);
	Effect(float spawnXpos, float spawnYpos, float xSize, float ySize, Effect &base);
	void DrawEffect(bool isStatic = false);
	void nextFrame();
	void addTick();
};
#endif
