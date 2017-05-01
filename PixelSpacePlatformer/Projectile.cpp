#pragma once
#include "Projectile.h"
#include <iostream>
Point Projectile::getReactiveMove()
{
	return Point(heading.getX()*reactiveForce,heading.getY()*reactiveForce);
}
Projectile::Projectile(float spawnXpos, float spawnYpos, float xSize, float ySize,float angle) :GameObject(spawnXpos, spawnYpos, xSize, ySize) {
	this-> active = true;
	generateHeading(angle);
	basicBox.rotate(Point(basicBox.getXmid(), basicBox.getYmid()), angle);
}
Projectile::Projectile(float spawnXpos, float spawnYpos, float angle,Projectile& old) : GameObject(spawnXpos, spawnYpos,old.basicBox.getXSize(),old.basicBox.getYSize()) {
	this->active = true;
	generateHeading(angle);
	basicBox.rotate(Point(basicBox.getXmid(), basicBox.getYmid()), angle);
	this->speedMultiplier = old.speedMultiplier;
	this->distantToLive = old.distantToLive;
	this->maxDistance = old.maxDistance;
	this->textureID = old.textureID;
	this->effectTexture = old.effectTexture;
}
Projectile::Projectile() : GameObject() {
	active = false;
}
void Projectile::generateHeading(float angle) {
	heading.set(10, 0);
	heading.rotate(Point(basicBox.getXmid(),basicBox.getYmid()),angle);
	heading.Normalise();
}
void Projectile::updatePos(double foffset) {
	if (distantToLive <= 0) {
		active = false;
	}
	if (active) {
		this->basicBox.move((heading.getX()*foffset*speedMultiplier), (heading.getY()*foffset*speedMultiplier));
		distantToLive -= (foffset*speedMultiplier);
	}

}
Effect Projectile::collisionAction() {
	this->active = false;
	std::cout << "******* WE HIT A THING*****" << std::endl;
	return Effect(basicBox.getXmid(), basicBox.getYmid(), 0.1, 0.1,effectTexture);
}
