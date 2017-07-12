#include "HitScanWeapon.h"

Point HitscanWeapon::getMaxPoint()
{
	Point heading = Point();

	heading.setX(cos(currentAngle));
	heading.setY(sin(-currentAngle));
	//heading = projectileSpawnBehind- projectileSpawn;
	heading = heading * range;
	heading = heading + projectileSpawn;

	return heading;
}

Point HitscanWeapon::getCollisionPoint(Box target)
{
	Point collision = Point();
	target.findFirstPointLineIntersection(projectileSpawn, endPoint, collision);
	return collision;
}

Point HitscanWeapon::getCollisionPoint(Box target, Point max)
{
	Point collision = Point();
	target.findFirstPointLineIntersection(projectileSpawn, max, collision);
	//target.checkLineIntersection(projectileSpawn, max, Point());
	return collision;
}

bool HitscanWeapon::checkHit(Box target,Point max)
{
	return target.checkLineIntersection(projectileSpawn, max, Point());
}


void HitscanWeapon::weapDraw(Point & mount, bool debug)
{
	glPointSize(5.0f);
	glColor3f(1, 0, 0);
	glPushMatrix();
	glPointSize(10.0);
	//glLineWidth(15.0);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glColor3f(red, green, blue);
	Point* points = this->basicBox.getPoints();
	glBegin(GL_QUADS);
	if (!mirror_texture) {
		glTexCoord2f(0, 0); glVertex2f(points[0].getX(), points[0].getY());
		glTexCoord2f(1, 0);	glVertex2f(points[1].getX(), points[1].getY());
		glTexCoord2f(1, 1);	glVertex2f(points[2].getX(), points[2].getY());
		glTexCoord2f(0, 1);	glVertex2f(points[3].getX(), points[3].getY());
	}
	else {
		glTexCoord2f(0, 1); glVertex2f(points[0].getX(), points[0].getY());
		glTexCoord2f(1, 1);	glVertex2f(points[1].getX(), points[1].getY());
		glTexCoord2f(1, 0);	glVertex2f(points[2].getX(), points[2].getY());
		glTexCoord2f(0, 0);	glVertex2f(points[3].getX(), points[3].getY());
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glPushMatrix();
	if (this->firing) {
		glPushMatrix();
		float xlength = endPoint.getX() - projectileSpawn.getX();
		xlength *= xlength;
		float ylength = endPoint.getY() - projectileSpawn.getY();
		ylength *= ylength;
		float lineLength = sqrt(xlength + ylength);
		glTranslatef(projectileSpawn.getX(), projectileSpawn.getY(), 0);
		glRotatef(currentAngle*-57.295779513, 0.0, 0.0, 1.0);
		glTranslatef(0, -0.02, 0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, beamTexture);
		glBegin(GL_QUADS);
		glTexCoord2f(1, 1); glVertex2f(lineLength + 0.05, 0.05);
		glTexCoord2f(1, 0); glVertex2f(lineLength + 0.05, 0);
		glTexCoord2f(0, 0); glVertex2f(0, 0);
		glTexCoord2f(0, 1); glVertex2f(0, 0.05);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, impactTexture);
		glBegin(GL_QUADS);
		glTexCoord2f(1, 1); glVertex2f(lineLength + 0.05, 0.10);
		glTexCoord2f(1, 0); glVertex2f(lineLength + 0.05, -0.05);
		glTexCoord2f(0, 0); glVertex2f(lineLength, -0.05);
		glTexCoord2f(0, 1); glVertex2f(lineLength, 0.10);
		glEnd();

		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
		if (debug) {
		glColor3f(1, 0, 0);
		glBegin(GL_POINTS);
		glVertex2f(basicBox.getXmid(), basicBox.getYmid());
		glVertex2f(projectileSpawn.getX(), projectileSpawn.getY());
		glVertex2f(getMaxPoint().getX(), getMaxPoint().getY());
		glColor3f(0, 1, 0);
		glVertex2f(endPoint.getX(), endPoint.getY());
		glEnd();
	}
	}
	glPopMatrix();
	
}

HitscanWeapon::HitscanWeapon(float spawnXpos, float spawnYpos, float xSize, float ySize, Projectile & ammo):Weapon(spawnXpos,spawnYpos,xSize,ySize,ammo)
{
	range = 3.0;
}

HitscanWeapon::HitscanWeapon():Weapon()
{
	range = 3.0;
}

void HitscanWeapon::attack()
{
	this->firing = true;
	Mix_PlayChannel(-1, this->explosion, 0);
}
