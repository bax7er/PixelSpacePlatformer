#include "aiPlayer.h"

double AiPlayer::clock = 0;
Point AiPlayer::react(float fOffest,Player player)
{
	
	double xDif = this->basicBox.getXmid() - player.basicBox.getXmid();
	double yDif = this->basicBox.getYmid() - player.basicBox.getYmid();
	double distance = sqrt((xDif*xDif) + (yDif*yDif));
	canShoot = true;

	if (distance > 2) {
		canShoot = false;
	}
	if (aiMode == AIMODE_STATIC) {
		return Point(0, 0);
	}
	if (aiMode == AIMODE_WALK) {
		bool falling = false;
		if (!onGround) {
			falling = true;
		}
		if (distance < 2 && distance > 0.4) {
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
		}
		if (falling)
		{
			return Point(0, -0.01*fOffest);
		}
		return Point(0, 0);
	}
	if (aiMode == AIMODE_FLY) {
		double xMove = 0;
		double yMove = 0;
		if (distance < 2) {
			if (player.basicBox.getXmin() < this->basicBox.getXmin()) {
				faceLeft = true;
				xMove = -0.003*fOffest;
			}
			else if (player.basicBox.getXmax() > this->basicBox.getXmax()) {
					faceLeft = true;
					xMove = 0.003*fOffest;
				}
			if (player.basicBox.getYmid() < this->basicBox.getYmin()) {
				yMove = -0.003*fOffest;
			}
			if (player.basicBox.getYmid() > this->basicBox.getYmid()) {
				yMove = 0.003*fOffest;
			}
		}
		return Point(xMove, yMove);
	}
}

void AiPlayer::drawAiPlayer()
{
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glColor3f(red, green, blue);
	Point* points = this->basicBox.getPoints();
	glBegin(GL_QUADS);
	if (!faceLeft) {
		glTexCoord2f(0 + texturePos / (float) spritesOnSheet, 0); glVertex2f(points[0].getX(), points[0].getY());
		glTexCoord2f((texturePos + 1) / (float)spritesOnSheet, 0);	glVertex2f(points[1].getX(), points[1].getY());
		glTexCoord2f((texturePos + 1) / (float)spritesOnSheet, 1);	glVertex2f(points[2].getX(), points[2].getY());
		glTexCoord2f(0 + texturePos / (float)spritesOnSheet, 1);	glVertex2f(points[3].getX(), points[3].getY());
	}
	else {
		glTexCoord2f((texturePos + 1) / (float)spritesOnSheet, 0); glVertex2f(points[0].getX(), points[0].getY());
		glTexCoord2f(0 + texturePos / (float)spritesOnSheet, 0);	glVertex2f(points[1].getX(), points[1].getY());
		glTexCoord2f(0 + texturePos / (float)spritesOnSheet, 1);	glVertex2f(points[2].getX(), points[2].getY());
		glTexCoord2f((texturePos + 1) / (float)spritesOnSheet, 1);	glVertex2f(points[3].getX(), points[3].getY());
	}
	glEnd();
	
	glDisable(GL_TEXTURE_2D);
	//HP Bar Total
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
	//HP Bar Filled
	glBegin(GL_QUADS);
	glVertex2f(basicBox.getXmin()+0.01, basicBox.getYmax() + 0.02);
	glVertex2f(basicBox.getXmin() + 0.01 +hpEnd, basicBox.getYmax() + 0.02);
	glVertex2f(basicBox.getXmin() + 0.01 + hpEnd, basicBox.getYmax() + 0.03);
	glVertex2f(basicBox.getXmin() + 0.01, basicBox.getYmax() + 0.03);
	glEnd();
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glPopMatrix();
	//criticalRegion.drawBox(); //Uncomment to show head
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
	this->criticalRegion.move(translation.getX(), translation.getY());

}
AiPlayer::AiPlayer():GameObject()
{
	this->alive = true;
	this->healthPoints = 1000;
//	this->effectMoveX = 0;
	//this->effectMoveY = 0;
}

AiPlayer::AiPlayer(float spawnXpos, float spawnYpos, float xSize, float ySize) : GameObject(spawnXpos, spawnYpos, xSize, ySize)
{
	this->alive = true;
	this->healthPoints = 1000;
	this->criticalRegion = Box(spawnXpos,(spawnYpos+ySize/3),xSize/2,ySize/2.5);
	
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


bool AiPlayer::checkOnTop(GameObject & player)
{

		float boxbottom = player.basicBox.getYmin();
		float terrainTop = this->basicBox.getYmax();
		if ((boxbottom - terrainTop < 0.01) && (boxbottom - terrainTop > 0)) {
			if ((player.basicBox.getXmin() < this->basicBox.getXmax()) && (player.basicBox.getXmax() > this->basicBox.getXmin())) {
				return true;
			}
		}
	return false;
}
