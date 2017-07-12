#pragma once

#include "Weapon.h";

Weapon::Weapon(float spawnXpos, float spawnYpos, float xSize, float ySize, Projectile &ammo) :GameObject(spawnXpos, spawnYpos, xSize, ySize) {
	initSpawnX = spawnXpos;
	initSpawnY = spawnYpos;
	sample = ammo;
	clock = 0;
	clip = vector<Projectile>();
}
Weapon::Weapon() : GameObject() {

}
void Weapon::setBinding(float x, float y) {
	this->bindPoint.set(x, y);
}
void Weapon::setBinding() {
	this->bindPoint.set(this->basicBox.getXmin(), this->basicBox.getYmid());
}
void Weapon::rebind(Point &mount) {
	float xmove = this->bindPoint.getX() - mount.getX();
	float ymove = this->bindPoint.getY() - mount.getY();
	this->basicBox.move(-xmove, -ymove);
	this->bindPoint.move(-xmove, -ymove);
	this->projectileSpawn.move(-xmove, -ymove);
}
void Weapon::weapDraw(Point &mount) {
	/*
	glPointSize(10.0f);
	glColor3f(1, 0, 0);
	glBegin(GL_POINTS);
	glVertex2f(projectileSpawn.getX(), projectileSpawn.getY());
	glVertex2f(bindPoint.getX(), bindPoint.getY());
	glEnd();/**/ //Uncomment to show projectile spawn points
	glPointSize(5.0f);
	glColor3f(0, 1, 0);

	/*
	glBegin(GL_POINTS);
	glVertex2f(mount.getX(), mount.getY());
	glEnd();/**/ //Uncomment to show weapon mount point
	glPushMatrix();
	//glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glColor3f(red, green, blue);
	Point* points = this->basicBox.getPoints();
	glBegin(GL_QUADS);
	if (!mirror_texture) {
		glTexCoord2f(0 + (float)currentFrame / spritesInSet, 0); glVertex2f(points[0].getX(), points[0].getY());
		glTexCoord2f((currentFrame + 1.0) / spritesInSet, 0);	glVertex2f(points[1].getX(), points[1].getY());
		glTexCoord2f((currentFrame + 1.0) / spritesInSet, 1);	glVertex2f(points[2].getX(), points[2].getY());
		glTexCoord2f(0 + (float)currentFrame / spritesInSet, 1);	glVertex2f(points[3].getX(), points[3].getY());
	}
	else {
		glTexCoord2f(0 + (float)currentFrame / spritesInSet,1); glVertex2f(points[0].getX(), points[0].getY());
		glTexCoord2f((currentFrame + 1.0) / spritesInSet, 1);	glVertex2f(points[1].getX(), points[1].getY());
		glTexCoord2f((currentFrame + 1.0) / spritesInSet,0);	glVertex2f(points[2].getX(), points[2].getY());
		glTexCoord2f(0 + (float)currentFrame / spritesInSet, 0);	glVertex2f(points[3].getX(), points[3].getY());
	}
	glEnd();
	for (Projectile &Projectile : clip) {
		if (Projectile.active) {
			Projectile.drawTextured();
		}
	}
	glDisable(GL_TEXTURE_2D);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glPopMatrix();
}
void Weapon::projDraw(bool debug) {
	for (Projectile &projectile : clip) {
		if (projectile.active) {
			projectile.drawTextured();
			if (debug) {
				projectile.drawBox();
			}
		}
	}
}
void Weapon::setSpawner(float x, float y) {
	this->projectileSpawn.set(x, y);
	this->initSpawnerX = x;
	this->initSpawnerY = y;
	//std::cout << initSpawnerX << " ******************************************" << initSpawnerY << endl;
}
void Weapon::setSpawner() {
	this->projectileSpawn.set(this->basicBox.getXmax(), this->basicBox.getYmid());
}
void Weapon::rotateWeapon(float angle) {
	currentAngle = angle;
	this->basicBox.move(bindPoint.getX(), bindPoint.getY());
	projectileSpawn.move(bindPoint.getX(), bindPoint.getY());
	this->basicBox.rotate(bindPoint, angle);
	projectileSpawn.rotate(bindPoint, angle);
}
void Weapon::resetPos() {
	this->basicBox.set(initSpawnX, initSpawnY);
	this->projectileSpawn.set(initSpawnerX, initSpawnerY);
}
void Weapon::attack() {
	
	if (lastFired < 0) {
		clip.push_back(Projectile(this->projectileSpawn.getX(), this->projectileSpawn.getY(), currentAngle, sample));
		currentProjectiles++;
		lastFired = fireDelay;
		currentFrame++;
		if (currentFrame + 1 > spritesInSet) {
			currentFrame = 0;
		}
	}
		//return clip[clip.size() - 1].getReactiveMove();

	
	//return Point(0, 0);
}
void Weapon::updateProjectiles(Point &tranformation, double fOffset) {
	size_t i = 0;
	bool found = false;
	while (i < clip.size()) {
		if (!clip[i].active) {
			clip.erase(clip.begin() + i);
			//found = true;
			cout << "killing projectile" << endl;
			i--;
			currentProjectiles--;
		}
		i++;
	}
	for (Projectile &Projectile : clip) {
		Projectile.updatePos(fOffset);
		Projectile.basicBox.move(tranformation.getX(), tranformation.getY());
	}
}
bool Weapon::checkProjectileCollision(Box &collision,vector <Effect> &effectList) {
	bool flag = false;
	size_t i = 0;
	while (i < clip.size()) {
		if ((clip[i].basicBox.getXmin()) < collision.getXmax() &&
			(clip[i].basicBox.getXmax()) > collision.getXmin() &&
			(clip[i].basicBox.getYmin()) < collision.getYmax() &&
			(clip[i].basicBox.getYmax()) > collision.getYmin()) {
			cout << "HIT first collision test" << endl;
			if (clip[i].basicBox.SepAxisText(collision)) {
				cout << "HIT second collision test" << endl;
				effectList.push_back(clip[i].collisionAction());
				//PlaySound(TEXT("explode.wav"), NULL, SND_FILENAME | SND_ASYNC);
				Mix_PlayChannel(-1, this->explosion, 0);
				flag = true;
				cout << "WE have a hit" << endl;
				clip.erase(clip.begin() + i);
				cout << "killing projectile" << endl;
				i--;
				currentProjectiles--;
			}
		}
		i++;
	}

	return flag;
}
void Weapon::effectUpdate(int frameCounter) {
	if (!effects.empty()) {
	//	cout << "CLOCK" << this->clock << " Last update At:" << this->lastUpdate << endl;
	//	cout << "Time since last update :" << clock - lastUpdate << endl;
		if ((clock - lastUpdate) >= 0.003) {
			lastUpdate = clock;
			//cout << "Advancing animation" << endl;
			int i = 0;
			while (i < effects.size()) {
				if (effects[i].active) {
					effects[i].addTick(); //advance frame
				}
				else {
					effects.erase(effects.begin() + i);
					i--;
					cout << "animation finished - destroying object" << endl;
				}
				i++;
			}
		}
	}
}

void Weapon::aim(double angle)
{
	//if (angle > -3.2 && angle < 3.2) {
		resetPos();
		rotateWeapon(angle);
		currentAngle = angle;
	//}
}
