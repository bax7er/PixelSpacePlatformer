#ifndef COLLECTIBLE_H
#define COLLECTIBLE_H
#define POINTSPOWERUP 0
#define AUTOROCKETPOWERUP 1
#define QUADDAMAGE 2
#define CHECKPOINT 3
#define OUTOFMAP 4
#include <SDL_mixer.h>
#include "GameObject.h"
#include "Effect.h"
#include "Player.h"
#include "Weapon.h"
class Collectible : public GameObject {
private:
	
public:
	static Mix_Chunk* collectionSound;
	static Mix_Chunk* autoRocketSound;
	static Mix_Chunk* quadDamageSound;
	bool active;
	int type;
	int pointsValue;
	GLuint effectTexture;
	double storedValue;
	double timer;
	bool countDownTimer = false;
	inline void updateClock(double time, Player &player, Weapon &playerWeapon);
	Collectible(float spawnXpos, float spawnYpos, float xSize, float ySize, int type);
	Collectible();
	void collisionAction(Player &player,Weapon &playerWeapon);
};
void Collectible::updateClock(double time,Player &player ,Weapon &playerWeapon)
{
	if (countDownTimer) {
		timer -= time;

		
		if (timer <= 0) {
			switch (type)
			{
			case AUTOROCKETPOWERUP:
				playerWeapon.fireDelay = this->storedValue;
				break;
			case QUADDAMAGE:
				player.setColour(1, 1, 1);
				player.damageMultiplier = 1;
				break;
			default:
				break;
			}
			if (type != OUTOFMAP) {
 				this->active = false;
			}
			
		}
	}
}
#endif