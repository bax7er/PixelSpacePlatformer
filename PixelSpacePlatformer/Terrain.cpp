#pragma once
#include "Terrain.h"
#include <iostream>
Terrain::Terrain(float spawnXpos, float spawnYpos, float xSize, float ySize):GameObject(spawnXpos, spawnYpos, xSize, ySize) {
	this->ground = true;
}
Terrain::Terrain() : GameObject() {

}
/*
bool Terrain::checkCollision(GameObject &collisionObject) {
	if (this->isDestroyed) {
		return false;
	}
	if (this->basicBox.getXmin() < collisionObject.basicBox.getXmax() &&
		this->basicBox.getXmax() > collisionObject.basicBox.getXmin() &&
		this->basicBox.getYmin() < collisionObject.basicBox.getYmax() &&
		this->basicBox.getYmax() > collisionObject.basicBox.getYmin())
	{
		return true;
	}
	return false;
}
bool Terrain::checkCollision(GameObject &collisionObject, float xShift, float yShift) {
	if (this->basicBox.axisAlinedTest(collisionObject.basicBox,xShift,yShift))
	{
		return true;
	}
	return false;
}
bool Terrain::checkCollision(Player &collisionObject, Point &translation) {
	float xShift = translation.getX();
	float yShift = translation.getY();
	
	if (this->isDestroyed) {
		return false;
	}
	if (collisionObject.getWeapon().checkProjectileCollision(this->basicBox)&& this->isDestroyable) {
		destroyedEffect.basicBox.set(this->basicBox.getXmid(), this->basicBox.getYmid());
		destroyedEffect.basicBox.resize(this->basicBox.getXSize(), this->basicBox.getYSize());
		collisionObject.getWeapon().effects.push_back(destroyedEffect);
		this->isDestroyed = true;
		this->drawable = false;
		this->ground = false;
	}
	if (this->basicBox.axisAlinedTest(collisionObject.basicBox,xShift,yShift))
	{
		
		if (checkCollision(collisionObject, xShift, 0)) {
			if (checkCollision(collisionObject, xShift / 2, 0)) {
				xShift = 0;
			}
			else {
				xShift /= 2;
			}
		}
		else if (checkCollision(collisionObject, 0, yShift)) {
			if (checkCollision(collisionObject, 0, yShift / 2)) {
				yShift = 0;
				collisionObject.setJump(false);
				collisionObject.setYSpeed(0);
			}
			else {
				yShift /= 2;
			}
			
		}
		translation.set(xShift, yShift);
		return true;
	}
	return false;
}
bool Terrain::checkCollision(AiPlayer &collisionObject, Point &translation) {
	float xShift = translation.getX();
	float yShift = translation.getY();

	if (this->isDestroyed) {
		return false;
	}
	if (this->basicBox.axisAlinedTest(collisionObject.basicBox, xShift, yShift))
	{

		if (checkCollision(collisionObject, xShift, 0)) {
			if (checkCollision(collisionObject, xShift / 2, 0)) {
				xShift = 0;
			}
			else {
				xShift /= 2;
			}
		}
		else if (checkCollision(collisionObject, 0, yShift)) {
			if (checkCollision(collisionObject, 0, yShift / 2)) {
				yShift = 0;
			}
			else {
				yShift /= 2;
			}

		}
		translation.set(xShift, yShift);
		return true;
	}
	return false;
}*/
bool Terrain::checkOnGround(Box &box) {
	if (this->ground) {
		float boxbottom = box.getYmin();
		float terrainTop = this->basicBox.getYmax();
		if ((boxbottom - terrainTop < 0.01) && (boxbottom - terrainTop > 0)) {
			if ((box.getXmin() < this->basicBox.getXmax()) && (box.getXmax() > this->basicBox.getXmin())) {
				return true;
			}
		}
	}
	return false;
}


bool Terrain::checkOnGround(GameObject & player)
{
	if(this->ground) {
		float boxbottom = player.basicBox.getYmin();
		float terrainTop = this->basicBox.getYmax();
		if ((boxbottom - terrainTop < 0.01) && (boxbottom - terrainTop > 0)) {
			if ((player.basicBox.getXmin() < this->basicBox.getXmax()) && (player.basicBox.getXmax() > this->basicBox.getXmin())) {
				return true;
			}
		}
	}
	return false;
}
