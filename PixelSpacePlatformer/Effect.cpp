#pragma once
#include "Effect.h"
Effect::Effect() : GameObject() {

}
Effect::Effect(float spawnXpos, float spawnYpos, float xSize, float ySize,GLuint sprites) :GameObject(spawnXpos, spawnYpos, xSize, ySize) {
	currentFrame = 0;
	tickRate = 1;
	framesInSet = 14;
	this->textureID = sprites;
	active = true;
}
Effect::Effect(float spawnXpos, float spawnYpos, float xSize, float ySize, Effect &base) :GameObject(spawnXpos, spawnYpos, xSize, ySize) {
	currentFrame = 0;
	tickRate = base.tickRate;
	framesInSet = base.framesInSet;
	loopType = base.loopType;
	this->textureID = base.textureID;
	active = true;
}
void Effect::DrawEffect(bool isStatic) {
	glPushMatrix();
	if (!isStatic) {
		glLoadIdentity();
	}
	if (active) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glColor3f(red, green, blue);
		Point* points = this->basicBox.getPoints();
		glBegin(GL_QUADS);
		glTexCoord2f(0 + currentFrame / framesInSet, 0); glVertex2f(points[0].getX(), points[0].getY());
		glTexCoord2f((currentFrame + 1) / framesInSet, 0);	glVertex2f(points[1].getX(), points[1].getY());
		glTexCoord2f((currentFrame + 1) / framesInSet, 1);	glVertex2f(points[2].getX(), points[2].getY());
		glTexCoord2f(0 + currentFrame / framesInSet, 1);	glVertex2f(points[3].getX(), points[3].getY());

		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
	glPopMatrix();
}
void Effect::nextFrame() {
	if (currentFrame == 0) {
		frameAdvance = 1;
	}
	currentFrame+=frameAdvance;
	if ((currentFrame >= framesInSet-1)&& loopType == PLAYONCE) {
		active = false;
	}
	else if ((currentFrame >= framesInSet - 1) && loopType == LOOP) {
		frameAdvance = -1;
	}
};
void Effect::addTick() {
	ticksWaited++;
	if (ticksWaited >= tickRate) {
		nextFrame();
		ticksWaited = 0;
	}
};