#ifndef PLAYER_H
#define PLAYER_H
#include "GameObject.h"
//#include "Weapon.h";
#include <vector>
#include "Image_Loading/nvImage.h"
#include <iostream>

using namespace std;
class Player : public GameObject{
protected:
	//Weapon weapon;

	float maxJumpTime = 0.8;
	float currentJumptime=0;
	float currentFalltime = 0;
	bool onGround;
	float weaponSpin = 0;
	int texturePos;
	float maxJumpHeight;
	float jumpedHeight;
	float maxXspeed;
	float xSpeed, xAcc,ySpeed,yAcc;
	float currentAngle = 0;
	double lastUpdate = 0;
	float effectMoveY = 0;
	float effectMoveX = 0;
	float hitTimer = 0;
	bool hasShield = false;
public:
	GLuint shieldTexture;
	int points = 0;
	double distanceMovedX = 0;
	Point weaponMount = Point(0, 0);
	bool jumping;
	bool falling;
	int spritesOnSheet = 11;
	int hp = 1000;
	int maxHP = 1000;
	double clock = 0;
	//void aimWeapon(double angle);
	int animationFrameRate = 6;
	//void setWeapon(Weapon weapon);
	//inline Weapon& getWeapon() { return weapon; };
	void nextFrame() { texturePos++; texturePos %= spritesOnSheet; };
	void animateUpdate();
	void effectsUpdate();
	void getHit(float damage);
	void inline advanceClock(float time) {
		this->clock += time;
		//this->weapon.clock += time;
		if (jumping) {
			currentJumptime += time;
		}
		if (falling) {
			currentFalltime += time;
		}
		if (!falling) {
			currentFalltime = 0;
		}
		if (!jumping)  {
			currentJumptime -= time*2;
		}
		if (currentJumptime < 0) {
			currentJumptime = 0;
		}
			hitTimer -= time;
			if (hitTimer < 0) {
				hasShield = false;
			}
			if (hitTimer < -1 && hp<maxHP) {
				hp += time*1000;
				if (hp > maxHP) {
					hp = maxHP;
				}
			}
			
	}
	bool textureFlip;
	inline void setXSpeed(float speed) {
		xSpeed = speed;
	}
	inline void setYSpeed(float speed) {
		ySpeed = speed;
	}
	inline void addXSpeed(float speed) {
		xSpeed += speed;
	}
	inline void addYSpeed(float speed) {
		ySpeed += speed;
	}
	inline void setYAcc(float acc) {
		yAcc = acc;
	}
	inline bool isOnGround() {
		return onGround;
	}
	inline void setJump(bool state) {
		jumping = state;
		currentFalltime = 0;
		currentJumptime = 0;
		falling = false;
		if (state == true) {
			onGround = false;
		}
	}
	inline void setOnGround(bool state) {
		onGround = state;
	}
	Player();
	Player(float spawnXpos, float spawnYpos, float xSize, float ySize);
	void drawPlayer();
	bool checkCollision(GameObject &collisionObject);
	bool checkCollision(GameObject &collisionObject, float xShift, float yShift);
	bool checkCollision(vector <GameObject>& collisionObject, float xShift, float yShift);
	//bool checkCollision(vector <Terrain>& collisionObject, float xShift, float yShift);
	//bool checkOnGround(vector <Terrain>& levelGeometry);
	Point getTranslation(float offset);
	void setTexture(GLuint id) { GameObject::setTexture(id); };
	void setMountPoint(float x, float y);
	//void mirrorWeapon(bool value);
	//void attack();
	void updateProjectiles(Point &tranformation, double foffset);
	void moveToStandby();
};
#endif