#include "aiPlayer.h"

double AiPlayer::clock = 0;
Point AiPlayer::react(float fOffest,Player player)
{
	bool falling = false;
	if (!onGround) {
		printf("AI IS FALLING \n");
		falling = true;
		//return Point(0, -0.01*fOffest);
	}
	if (player.basicBox.getXmin() < this->basicBox.getXmin()) {
		faceLeft = true;
		this->animationUpdate(false);
		if (!falling)
		{
			return Point(-0.003*fOffest, 0);
		}
		else {
			return Point(-0.003*fOffest, -0.01*fOffest);
		}
	}
	if (player.basicBox.getXmax() >= this->basicBox.getXmax()) {
		faceLeft = false;
		this->animationUpdate(false);
		if (!falling)
			return Point(0.003*fOffest, 0);
		else
			return Point(0.003*fOffest, -0.01*fOffest);
	}
	else {
		this->animationUpdate(true);
	}
	return Point(0, 0);
}

void AiPlayer::drawAiPlayer()
{
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glColor3f(red, green, blue);
	Point* points = this->basicBox.getPoints();
//	this->weapon.weapDraw(weaponMount);
	glBegin(GL_QUADS);
	if (!faceLeft) {
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
	//HP Total
	glColor3f(0,0,0);
	glBegin(GL_QUADS);
	glVertex2f(basicBox.getXmin(), basicBox.getYmax() + 0.01);
	glVertex2f(basicBox.getXmax(), basicBox.getYmax() + 0.01);
	glVertex2f(basicBox.getXmax() , basicBox.getYmax() + 0.04);
	glVertex2f(basicBox.getXmin(), basicBox.getYmax() + 0.04);
	glEnd();
	
	double hpEnd = (basicBox.getXmax() - 0.01) - (basicBox.getXmin() + 0.01);
	hpEnd *= (float)healthPoints/ (float)maxHP;
	glColor3f(0, 1, 0);
	if ((float)healthPoints / (float)maxHP < 0.2) {
		glColor3f(1, 0, 0);
	}
	
	glBegin(GL_QUADS);
	glVertex2f(basicBox.getXmin()+0.01, basicBox.getYmax() + 0.02);
	glVertex2f(basicBox.getXmin() + 0.01 +hpEnd, basicBox.getYmax() + 0.02);
	glVertex2f(basicBox.getXmin() + 0.01 + hpEnd, basicBox.getYmax() + 0.03);
	glVertex2f(basicBox.getXmin() + 0.01, basicBox.getYmax() + 0.03);
	glEnd();
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glPopMatrix();
}

void AiPlayer::animationUpdate(bool reversed)
{
	cout << "AI Frame:" << this->texturePos << endl;
	if ((clock - lastUpdate) >= animationFrameRate) {
		lastUpdate = clock;
		
	if (!reversed) {
			this->nextFrame();
		}
	else {
		this->moveToDefault();
	}
	}
}

void AiPlayer::moveToDefault()
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

void AiPlayer::move(Point translation)
{
	this->basicBox.move(translation.getX(), translation.getY());
	//this->weapon.move(translation);
}

AiPlayer::AiPlayer():GameObject()
{
	this->alive = true;
	this->healthPoints = 100000;
//	this->effectMoveX = 0;
	//this->effectMoveY = 0;
}

AiPlayer::AiPlayer(float spawnXpos, float spawnYpos, float xSize, float ySize) : GameObject(spawnXpos, spawnYpos, xSize, ySize)
{
	this->alive = true;
	this->healthPoints = 100000;
//	this->effectMoveX = 0;
//	this->effectMoveY = 0;
}

bool AiPlayer::checkCollision(GameObject & collisionObject, float xShift, float yShift)
{
	if (this->basicBox.axisAlinedTest(collisionObject.basicBox, xShift, yShift))
	{
		return true;
	}
	return false;
}

bool AiPlayer::checkCollision(Player & collisionObject, Point & translation)
{
	float xShift = translation.getX();
	float yShift = translation.getY();

	if (this->basicBox.axisAlinedTest(collisionObject.basicBox, xShift, yShift))
	{
		collisionObject.getHit(10);
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

bool AiPlayer::checkOnTop(GameObject & player)
{

		float boxbottom = player.basicBox.getYmin();
		float terrainTop = this->basicBox.getYmax();
		if ((boxbottom - terrainTop < 0.01) && (boxbottom - terrainTop > 0)) {
			if ((player.basicBox.getXmin() < this->basicBox.getXmax()) && (player.basicBox.getXmax() > this->basicBox.getXmin())) {
				this->alive = false;
				return true;
			}
		}
	return false;
}
