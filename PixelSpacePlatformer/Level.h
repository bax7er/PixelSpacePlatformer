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
#include "Weapon.h"
#include "aiPlayer.h"
#include <SDL_mixer.h>
using namespace std;
class Level {
private:
	double clock;
	double ticks; // Frame of expected length
	double speedMultiplier;
	Weapon playerWeapon;
public:
	Player player;
	double getClock() { return clock; };
	void setSpeedMulitplier(double speed) { speedMultiplier = speed; };
	void movePlayer(double movement);
	void playerJump();
	void resetClock() { clock = 0; };
	void advanceClock(double time) { clock += time; player.advanceClock(time); };
	void resetTicks() { ticks = 0; };
	void advanceTicks(double time) { ticks += time; };
	Mix_Music *levelMusic = NULL;
	Mix_Chunk *critSFX = NULL;
	vector <Point> intersectionMarkers;
	vector <GLuint> textures;
	vector <Terrain> levelGeometry;
	vector <Terrain> foreground;
	vector <Terrain>  background;
	vector <Effect>  levelEffect;
	vector <Effect>  animatedEffects;
	vector <AiPlayer>  bots;
	vector <Weapon>  botsWeapons;
	Level();
	Level(string filename);
	Terrain makeObjectTerrain(string input);
	Effect makeEffect(string input);
	GLuint loadTexture(string &input);
	void loadMusic(string &input);
	void setRepeat(string &input, vector <Terrain> &type);
	void loadEffect(string &input, vector <Effect> &type);
	void setDestroyable(string &input, vector <Terrain> &type);
	void cleanUp();
	void draw();
	void update(double speedMultiplier,GameObject cursor);
	void movePlayer();
	void playerAttack();
	void aimBots();
};

#endif