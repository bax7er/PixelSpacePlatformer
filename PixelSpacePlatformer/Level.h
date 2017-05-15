#pragma once
#ifndef LEVEL_H
#define LEVEL_H
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "Terrain.h"
#include "Effect.h"
#include "Player.h"
#include "HitScanWeapon.h"
#include "aiPlayer.h"
#include "MainMenu.h"
#include "Collectible.h"
#include <SDL_mixer.h>
#include <iomanip>
#define ROCKETLAUNCHER 0
#define LASER 1
#define LEVELCOMPLETIONBONUS 10000
using namespace std;
class Level {
private:
	//Resets player position to last collected checkpoint
	void respawnPlayer();

	//Tracks current weapons
	int currentWeapon = 0;
	int maxWeapons = 1;

	double clock;
	double ticks; // Frame of expected length or fraction of expected length

	double speedMultiplier;// delta t

	Weapon playerRocketLauncher;
	HitscanWeapon playerLaser;

	//Player Goal
	GameObject endPoint;
public:
	//Fade death screen
	float deathScreenAlpha = 0;

	GLFont * myfont;

	Player player;

	double getClock() { return clock; };
	void setSpeedMulitplier(double speed) { speedMultiplier = speed; };
	void movePlayer(double movement);
	void playerJump();
	void resetClock() { clock = 0; };
	void advanceClock(double time) { if (player.alive) { clock += time; player.advanceClock(time); } };
	void resetTicks() { ticks = 0; };
	void advanceTicks(double time) { ticks += time; };

	//Played on loop
	Mix_Music *levelMusic = NULL;

	//Played when a ai is killed with a critical hit
	Mix_Chunk *critSFX = NULL;

	//Tracks points of intersections
	vector <Point> intersectionMarkers;
	vector <GLuint> textures;

	//We can collide with this
	vector <Terrain> levelGeometry;

	//Renders infront of player
	vector <Terrain> foreground;

	//Renders behind player
	vector <Terrain>  background;

	vector <Effect>  levelEffect;
	vector <Effect>  animatedEffects;

	//Stores ai, each AI must have a weapon
	vector <AiPlayer>  bots;
	vector <Weapon>  botsWeapons;

	vector <Collectible>  powerUPs;
	vector <Projectile>  ammo;

	//Default constructor is not used
	Level();

	//Loads level from .lvl file, font to use for death screen
	Level(string filename, GLFont * font);

	//Methods to handle different aspects of file loading
	Terrain makeObjectTerrain(string input);
	Effect makeEffect(string input);
	GLuint loadTexture(string &input);
	Collectible makeCollectible(string &input);
	AiPlayer makeAI(string &input);
	Weapon makeAIWeapon(string &input);
	void loadEnd(string &input);
	void loadMusic(string &input);
	void setRepeat(string &input, vector <Terrain> &type);
	void loadEffect(string &input, vector <Effect> &type);
	void setDestroyable(string &input, vector <Terrain> &type);

	// Clears elements from the vectors that are no longer active
	void cleanUp();

	void draw();

	void update(double speedMultiplier,GameObject cursor);
	void playerAttack(bool isAttack);
	
	//Aims all bots that can shoot at the player
	void aimBots();

	//Switches the currently used weapon
	void switchWeapon();
};

#endif