#include "Collectible.h"

Mix_Chunk* Collectible::collectionSound = NULL;
Mix_Chunk* Collectible::quadDamageSound = NULL;
Mix_Chunk* Collectible::autoRocketSound = NULL;
Collectible::Collectible()
{
}
Collectible::Collectible(float spawnXpos, float spawnYpos, float xSize, float ySize, int type): GameObject(spawnXpos, spawnYpos,xSize,ySize) {
	this->type = type;
	if (type == POINTSPOWERUP) {
		this->pointsValue = 300;
	}
	else {
		this->pointsValue = 50;
	}
	if (type == AUTOROCKETPOWERUP) {
		this->timer = 1500.0;
	}
	if (type == QUADDAMAGE) {
		this->timer = 1000.0;
	}
	if (type == OUTOFMAP) {
		this->pointsValue = -50;
	}
}
void Collectible::collisionAction(Player &player, Weapon &playerWeapon)
{
	if (this->drawable) {
		player.points += pointsValue;
		switch (type)
		{
		case AUTOROCKETPOWERUP:
			Mix_PlayChannel(-1, autoRocketSound, 0);
			storedValue = playerWeapon.fireDelay;
			playerWeapon.fireDelay = 10;
			countDownTimer = true;
			break;
		case QUADDAMAGE:
			Mix_PlayChannel(-1, quadDamageSound, 0);
			player.setColour(1,0.2,0.5);
			player.damageMultiplier = 4;
			countDownTimer = true;
			break;
		case CHECKPOINT:
			player.respawnPoint = Point(this->basicBox.getXmid(), this->basicBox.getYmid());
			break;
		case OUTOFMAP:
			player.respawn = true;
			break;
		default:
			Mix_PlayChannel(-1, collectionSound, 0);
			break;
		}
		if (type != OUTOFMAP) {
			this->drawable = false;
		}
	}
}
