#pragma once

#include "Player.h";

#include <iostream>;

Player::Player() :GameObject()
{
}


Player::Player(float spawnXpos, float spawnYpos, float xSize, float ySize) :GameObject(spawnXpos, spawnYpos, xSize, ySize) {
	onGround = false;
	jumpedHeight = 0;
	maxJumpHeight = -0.7;
	maxXspeed = 0.03;
	texturePos = 0;
}

void Player::drawPlayer() {

	if (textureFlip) {
		weaponMount.set(-0.02, -0.18);
	}
	else {
		weaponMount.set(0, -0.18);
	}
//	weapon.rebind(weaponMount);
	glPushMatrix();
		//glLoadIdentity();
		glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glColor3f(red, green, blue);
			Point* points = this->basicBox.getPoints();
			glBegin(GL_QUADS);
				if (!textureFlip) {
					glTexCoord2f(0 + texturePos / 11.0, 0); glVertex2f(points[0].getX(), points[0].getY());
					glTexCoord2f((texturePos + 1) / 11.0, 0);	glVertex2f(points[1].getX(), points[1].getY());
					glTexCoord2f((texturePos + 1) / 11.0, 1);	glVertex2f(points[2].getX(), points[2].getY());
					glTexCoord2f(0 + texturePos / 11.0, 1);	glVertex2f(points[3].getX(), points[3].getY());
				}
				else {
					glTexCoord2f((texturePos + 1) / 11.0, 0); glVertex2f(points[0].getX(), points[0].getY());
					glTexCoord2f(0 + texturePos / 11.0, 0);	glVertex2f(points[1].getX(), points[1].getY());
					glTexCoord2f(0 + texturePos / 11.0, 1);	glVertex2f(points[2].getX(), points[2].getY());
					glTexCoord2f((texturePos + 1) / 11.0, 1);	glVertex2f(points[3].getX(), points[3].getY());
				}
			glEnd();
		glDisable(GL_TEXTURE_2D);
		if (hasShield) {
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, shieldTexture);
			glColor3f(red, green, blue);
			glBegin(GL_QUADS);
				glTexCoord2f(0, 0); glVertex2f(points[0].getX(), points[0].getY());
				glTexCoord2f( 1, 0);	glVertex2f(points[1].getX(), points[1].getY());
				glTexCoord2f( 1, 1);	glVertex2f(points[2].getX(), points[2].getY());
				glTexCoord2f(0 , 1);	glVertex2f(points[3].getX(), points[3].getY());
				glEnd();
				glDisable(GL_TEXTURE_2D);
		}
		//this->weapon.weapDraw(weaponMount);
	
	glPopMatrix();
	//HP Bar Total
	glColor3f(0, 0, 0);
	glBegin(GL_QUADS);
	glVertex2f(basicBox.getXmin(), basicBox.getYmax() + 0.01);
	glVertex2f(basicBox.getXmax(), basicBox.getYmax() + 0.01);
	glVertex2f(basicBox.getXmax(), basicBox.getYmax() + 0.04);
	glVertex2f(basicBox.getXmin(), basicBox.getYmax() + 0.04);
	glEnd();

	double hpEnd = (basicBox.getXmax() - 0.01) - (basicBox.getXmin() + 0.01);
	hpEnd *= (float)hp / (float)maxHP;
	glColor3f(0, 1, 0);
	if ((float)hp / (float)maxHP < 0.2) {
		glColor3f(1, 0, 0);
	}
	//HP Bar Filled
	glBegin(GL_QUADS);
	glVertex2f(basicBox.getXmin() + 0.01, basicBox.getYmax() + 0.02);
	glVertex2f(basicBox.getXmin() + 0.01 + hpEnd, basicBox.getYmax() + 0.02);
	glVertex2f(basicBox.getXmin() + 0.01 + hpEnd, basicBox.getYmax() + 0.03);
	glVertex2f(basicBox.getXmin() + 0.01, basicBox.getYmax() + 0.03);
	glEnd();
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glPopMatrix();
}
bool Player::checkCollision(GameObject &collisionObject){
		if (this->basicBox.getXmin() < collisionObject.basicBox.getXmax() &&
			this->basicBox.getXmax() > collisionObject.basicBox.getXmin() &&
			this->basicBox.getYmin() < collisionObject.basicBox.getYmax() &&
			this->basicBox.getYmax() > collisionObject.basicBox.getYmin())
		{
			return true;
	}
	return false;
}
bool Player::checkCollision(GameObject &collisionObject,float xShift,float yShift) {
	if ((this->basicBox.getXmin() +xShift) < collisionObject.basicBox.getXmax() &&
		(this->basicBox.getXmax() + xShift) > collisionObject.basicBox.getXmin() &&
		(this->basicBox.getYmin() + yShift) < collisionObject.basicBox.getYmax() &&
		(this->basicBox.getYmax() + yShift) > collisionObject.basicBox.getYmin())
	{
		return true;
	}
	return false;
}
bool Player::checkCollision(vector <GameObject>& collisionObject, float xShift, float yShift) {
	for (GameObject &object : collisionObject) {
		if  ((this->basicBox.getXmin() + xShift) < object.basicBox.getXmax() &&
			(this->basicBox.getXmax() + xShift) > object.basicBox.getXmin() &&
			(this->basicBox.getYmin() + yShift) < object.basicBox.getYmax() &&
			(this->basicBox.getYmax() + yShift) > object.basicBox.getYmin())
		{
			return true;
		}
	}
	return false;
}
/*
bool Player::checkCollision(vector <Terrain>& collisionObject, float xShift, float yShift) {
	for (GameObject &object : collisionObject) {
		if ((this->basicBox.getXmin() + xShift) < object.basicBox.getXmax() &&
			(this->basicBox.getXmax() + xShift) > object.basicBox.getXmin() &&
			(this->basicBox.getYmin() + yShift) < object.basicBox.getYmax() &&
			(this->basicBox.getYmax() + yShift) > object.basicBox.getYmin())
		{
			return true;
		}
	}
	return false;
}
*/
/*bool Player::checkOnGround(vector <Terrain>& levelGeometry) {
	this->onGround = false;
	for (Terrain &terrain : levelGeometry)
	{
		float boxbottom = this->basicBox.getYmin();
		float terrainTop = terrain.basicBox.getYmax();
		if ((boxbottom - terrainTop < 0.01) && (boxbottom - terrainTop > 0)) {
			if ((this->basicBox.getXmin() < terrain.basicBox.getXmax()) && (this->basicBox.getXmax() > terrain.basicBox.getXmin())) {
				onGround = true;
				return true;
			}
		}
	}
	return false;
}
*/
Point Player::getTranslation(float offset) {
	maxJumpTime = 0.6;
	yAcc = 0.0005*offset*offset;
	if (jumping) {
		cout << "Time take" << currentJumptime << " max " << maxJumpTime << endl;
	//	cout << "Yspeed =" << ySpeed << "    Yacc = "<<yAcc<<endl;
		if (currentJumptime <= maxJumpTime) {
			float move = (maxJumpTime - currentJumptime) / maxJumpTime;
			ySpeed = -0.02*offset*move;
			//ySpeed = -0.01*offset;
			//jumpedHeight += ySpeed;
			//cout << "Current Jump Height = " << jumpedHeight << endl;
		}
		else {
			cout << "Top of Jump" << endl;
			jumping = false;
			falling = true;
			//jumpedHeight = 0;
			//currentJumptime = 0;
		}
	}
	if (onGround ) {
		falling = false;
	}
	if (!onGround && !jumping) {
		falling = true;
	}
	if (falling) {
		float move;
	    move = currentFalltime;
		if (move > 1) {
			move = 1;
		}
		
		ySpeed = 0.03*offset*move;//cout << "Yspeed =" << ySpeed << "    Yacc = " << yAcc << endl;
	}
	xSpeed += effectMoveX*offset;
	effectMoveX /= 1.1;
	ySpeed += effectMoveY*offset;
	effectMoveY /= 1.1;
	if (effectMoveY > 0) {
		cout << "oops" << endl;
	}
	return Point(xSpeed, ySpeed);
}
void Player::setMountPoint(float x, float y) {
	weaponMount.set(x, y);
}
/*
void Player::aimWeapon(double angle) {
	if (angle > -3.2 && angle < 3.2) {
		this->weapon.resetPos();
		weapon.rotateWeapon(angle);
		currentAngle = angle;
	}
}
void Player::setWeapon(Weapon weapon) {
	this->weapon = weapon;
	weapon.rebind(weaponMount);
}
void Player::mirrorWeapon(bool value) {
	this->weapon.mirror(value);
}
void Player::attack() {
	Point & move = this->weapon.attack();
	if (!this->onGround) {
		this->effectMoveX = 0.05*move.getX();
		falling = true;
		if (move.getY() < 0) {
			this->effectMoveY = 0.05*move.getY();
			this->currentFalltime = 0;
		}
	}
	else {
		if (move.getY() < -0.3) {
			jumping = true;
		}
	}

}
*/
void Player::updateProjectiles(Point &tranformation,double foffset) {
	//this->weapon.updateProjectiles(tranformation, foffset);
}
void Player::moveToStandby()
{
	if (texturePos != 0) {

		if (texturePos < spritesOnSheet / 2) {
			texturePos--;
			cout << "moving back frame" << endl;
		}
		if (texturePos >= spritesOnSheet / 2) {
			texturePos++;
			texturePos %= spritesOnSheet - 1;
			cout << "moving forward frame" << endl;
		}
	}
}
void Player::finished()
{
	if (!goalAchieved) {
		Mix_FadeOutMusic(1000);
		Mix_PlayChannel(-1, this->goalChunk, 0);
		goalAchieved = true;
	}
}
void Player::drawDead()
{
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, deadTexture);
	glColor3f(red, green, blue);
	Point* points = this->basicBox.getPoints();
	//glRotatef(20, 0.0, 0.0, 1.0);
	glBegin(GL_QUADS);
		glTexCoord2f(0 , 0); glVertex2f(points[0].getX(), points[0].getY());
		glTexCoord2f( 1 , 0);	glVertex2f(points[1].getX()+0.05, points[1].getY());
		glTexCoord2f(1, 1);	glVertex2f(points[2].getX() + 0.05, points[2].getY());
		glTexCoord2f(0 , 1);	glVertex2f(points[3].getX(), points[3].getY());
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
void Player::animateUpdate() {
	//cout << "CLOCK" << this->clock << " Last update At:" << this->lastUpdate << endl;
	//cout << "Time since last update :" << clock - lastUpdate << endl;
	//if ((clock - lastUpdate) >= (1.0 / animationFrameRate)) {
		this->nextFrame();
		lastUpdate = clock;
		cout << "Frame:" << this->texturePos << endl;
	//}
	//this->weapon->effectUpdate(frameCounter);
}
void Player::effectsUpdate() {

	//this->weapon.effectUpdate(1);
}

void Player::getHit(float damage)
{
	if (hitTimer <= 0) {
		hitTimer = 0.5;
		hp -= damage;
		printf("Player Hit \n");
		hasShield = true;
		if (hp <= 0) {
			alive = false;
			Mix_FadeOutMusic(1000);
			Mix_PlayChannel(-1, this->gameOverChunk, 0);
		}
	}
}
