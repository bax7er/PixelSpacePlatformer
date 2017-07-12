#pragma once
#include "LEVEL.h";
#define TEXTURE 1
#define GEOMETRY 2
#define FOREGROUND 3
#define BACKGROUND 4
#define MUSIC 4
void Level::movePlayer(double movement)
{
	if (player.alive) {
		player.setXSpeed(movement * speedMultiplier);
		if (movement < 0) {
			player.textureFlip = false;
		}
		if (movement > 0) {
			player.textureFlip = true;
		}
		if (movement < 0) movement *= -1;
		player.distanceMovedX += (movement* speedMultiplier);
		if (movement == 0) {
			player.moveToStandby();
		}
	}
}
void Level::playerJump()
{
	if (player.alive) {
		printf("Player is alive \n");
		if (player.isOnGround()) {
			printf("Player is on ground \n");
			player.setJump(true);
		}
	}
}
Level::Level() {

}
Level::Level(string filename, GLFont * font) {
	ifstream  data1("effects.MANIFEST");
	this->myfont = font;
	string line;
	while (getline(data1, line))
	{
		loadEffect(line, levelEffect);
	}
	data1.close();
	ifstream  playerData("Default.plr");
	{
		getline(playerData, line, '#');
		double height = stod(line);
		getline(playerData, line);
		getline(playerData, line, '#');
		double width = stod(line);
		this->player = Player(0, -0.2, height, width);
		getline(playerData, line);
		getline(playerData, line, '#');
		char* filename = new char[line.length() + 1];
		strcpy(filename, line.c_str());
		player.setTexture(loadPNG(filename));
		delete[]filename;
		getline(playerData, line);
		getline(playerData, line, '#');
		filename = new char[line.length() + 1];
		strcpy(filename, line.c_str());
		player.shieldTexture = loadPNG(filename);
		delete[]filename;
		getline(playerData, line);
		getline(playerData, line, '#');
		player.hp = stof(line);
		getline(playerData, line);
		getline(playerData, line, '#');
		player.spritesOnSheet = stoi(line);

	}
	endPoint = GameObject(0, 0, 0.2, 0.2);
	endPoint.loadThisTexture("assets/textures/finishFlag.png");
	ifstream data(filename);
	int currentType = 0;
	while (getline(data, line))
	{
		if (line[0] == '$') {
			switch (line[1]) {
			case 'T':
				currentType = TEXTURE;
				this->textures.push_back(loadTexture(line.substr(3)));
				break;
			case 'M':
				currentType = MUSIC;
				this->loadMusic(line.substr(3));
				break;
			case 'G':
				currentType = GEOMETRY;
				if (line[2] == 'R') {

					setRepeat(line.substr(3), this->levelGeometry);
				}
				else if (line[2] == 'D') {
					setDestroyable(line.substr(3), this->levelGeometry);
				}
				else {
					this->levelGeometry.push_back(makeObjectTerrain(line.substr(3)));
				}
				break;
			case 'B':
				currentType = BACKGROUND;
				if (line[2] == 'R') {
					setRepeat(line.substr(3), this->background);
				}
				else {
					this->background.push_back(makeObjectTerrain(line.substr(2)));
				}
				break;
			case 'F':
				currentType = FOREGROUND;
				if (line[2] == 'R') {
					setRepeat(line.substr(3), this->foreground);
				}
				else {
					this->foreground.push_back(makeObjectTerrain(line.substr(2)));
				}
				break;
			case 'E':
				this->animatedEffects.push_back(makeEffect(line.substr(2)));
				break;
			case 'P':
				this->powerUPs.push_back(makeCollectible(line.substr(2)));
				break;
			case 'A':
				if (line[2] == 'W') {
					this->botsWeapons.push_back(makeAIWeapon(line.substr(3)));
				}
				else {
					this->bots.push_back(makeAI(line.substr(3)));
				}
				break;
			case 'Z':
				loadEnd(line.substr(2));
				break;
			}


		}
		else {
			cout << line << endl;
		}
	}
	data.close();

	player.setMountPoint(0, -0.18);
	player.gameOverChunk = Mix_LoadWAV("assets/music/gameOver.wav");
	player.goalChunk = Mix_LoadWAV("assets/music/goal.wav");
	player.deadTexture = loadPNG("assets/textures/deadPlayer.png");
	//Generate player weapons
	Projectile rocket = Projectile(0, 0, 0.1, 0.05, 0.1);
	rocket.effectTexture = loadPNG("assets/textures/Explode.png");
	rocket.setTexture(loadPNG("assets/textures/rocket1.png"));
	playerRocketLauncher = Weapon(0, 0, 0.26, 0.1, rocket);
	playerLaser = HitscanWeapon(0, 0, 0.2, 0.07, rocket);
	playerRocketLauncher.setBinding(0.05, 0);
	playerRocketLauncher.setSpawner(0.13, 0.02);
	playerRocketLauncher.weaponDamage = 300;
	playerRocketLauncher.setTexture(loadPNG("assets/textures/rocketLauncher.png"));
	playerRocketLauncher.explosion = Mix_LoadWAV("assets/music/Explosion3.wav");
	playerLaser.setBinding(0.05, 0);
	playerLaser.setSpawner(0.1, 0.0);
	playerLaser.weaponDamage = 5;
	playerLaser.range = 2;
	playerLaser.beamTexture = loadPNG("assets/textures/beam.png");
	playerLaser.impactTexture = loadPNG("assets/textures/laserImpact.png");
	playerLaser.setTexture(loadPNG("assets/textures/spLaser.png"));
	playerLaser.explosion = Mix_LoadWAV("assets/music/laser.wav");
	//Finished making player weapons

	//Load sound effects
	Collectible::collectionSound = Mix_LoadWAV("assets/music/powerUpCollect.wav");
	Collectible::quadDamageSound = Mix_LoadWAV("assets/music/quadDamage.wav");
	Collectible::autoRocketSound = Mix_LoadWAV("assets/music/unstoppable.wav");
	critSFX = Mix_LoadWAV("assets/music/headshot.wav");
}
void Level::setDestroyable(string &input, vector <Terrain> &type) {
	int values[2];
	string in;
	istringstream ss(input);
	int i = 0;
	while (getline(ss, in, ',') && i < 2) {
		values[i] = stoi(in);
		i++;
	}
	Terrain * target = &type[values[0]];
	target->isDestroyable = true;
	target->destroyedEffect = levelEffect[values[1]];
}
void Level::cleanUp()
{
	int i = 0;
	while (i < levelGeometry.size()) {
		if (levelGeometry[i].isDestroyed) {
			levelGeometry.erase(levelGeometry.begin() + i);
			i--;
		}
		i++;
	}
	i = 0;
	while (i < bots.size()) {
		if (!bots[i].alive) {
			bots.erase(bots.begin() + i);
			botsWeapons.erase(botsWeapons.begin() + i);
			i--;
		}
		i++;
	}
	i = 0;
	while (i < powerUPs.size()) {
		if (!powerUPs[i].active) {
			powerUPs.erase(powerUPs.begin() + i);
			i--;
		}
		i++;
	}
}
Terrain Level::makeObjectTerrain(string input) {
	float values[5];
	string in;
	istringstream ss(input);
	int i = 0;
	while (getline(ss, in, ',') && i < 5) {
		values[i] = stof(in);
		i++;
	}
	Terrain finished = Terrain(values[0], values[1], values[2], values[3]);
	finished.setTexture(this->textures[values[4]]);
	return finished;
}
GLuint Level::loadTexture(string &input) {

	char* filename = new char[input.length() + 1];
	strcpy(filename, input.c_str());
	GLuint finished = loadPNG(filename);
	delete[]filename;
	return finished;
}
Collectible Level::makeCollectible(string & input)
{
	float values[6];
	string in;
	istringstream ss(input);
	int i = 0;
	while (getline(ss, in, ',') && i < 6) {
		values[i] = stof(in);
		i++;
	}
	Collectible finished = Collectible(values[0], values[1], values[2], values[3], values[4]);
	finished.textureID = ((this->textures[values[5]]));
	return finished;
}
AiPlayer Level::makeAI(string & input)
{
	float values[14];
	string in;
	istringstream ss(input);
	int i = 0;
	while (getline(ss, in, ',') && i < 14) {
		values[i] = stof(in);
		i++;
	}
	AiPlayer finished = AiPlayer(values[0], values[1], values[2], values[3]);
	finished.textureID = ((this->textures[values[4]]));
	finished.spritesOnSheet = values[5];
	finished.maxHP = values[6];
	finished.healthPoints = values[6];
	finished.hasCritical = (values[13] == 1.0);
	finished.setColour(values[7], values[8], values[9]);
	finished.weaponMount = Point(values[10], values[11]);
	finished.aiMode = values[12];
	return finished;
}
Weapon Level::makeAIWeapon(string & input)
{
	Weapon newWeapon;
	if (ammo.size() < 3) {
		ammo.push_back(Projectile(0, 0, 0.1, 0.05, 0.1));
		ammo[0].effectTexture = loadPNG("assets/textures/Explode.png");
		ammo[0].setTexture(loadPNG("assets/textures/rocket1.png"));
		ammo.push_back(Projectile(0, 0, 0.05, 0.07, 0.1));
		ammo[1].effectTexture = loadPNG("assets/textures/Explode.png");
		ammo[1].setTexture(loadPNG("assets/textures/minigunBullet.png"));
		ammo.push_back(Projectile(0, 0, 0.05, 0.07, 0.1));
		ammo[2].effectTexture = loadPNG("assets/textures/Explode.png");
		ammo[2].setTexture(loadPNG("assets/textures/fireBall.png"));

	}
	if (input == " AI_RocketLauncher") {
		newWeapon = Weapon(0, 0, 0.26, 0.1, ammo[0]);
		newWeapon.setSpawner(0.13, 0.02);
		newWeapon.weaponDamage = 100;
		newWeapon.setTexture(loadPNG("assets/textures/rocketLauncher.png"));
	}
	else
		if (input == " AI_TurretLaser") {
			newWeapon = Weapon(0, 0, 0.26, 0.1, ammo[1]);
			newWeapon.setSpawner(0.13, 0.02);
			newWeapon.weaponDamage = 50;
			newWeapon.setTexture((loadPNG("assets/textures/turretGun.png")));
			newWeapon.fireDelay = 10;
			newWeapon.animatedWeapon = true;
			newWeapon.spritesInSet = 4;
			newWeapon.currentFrame = 0;
		}
		else
			if (input == " AI_DummyWeapon") {
				newWeapon = Weapon(0, 0, 0.26, 0.1, ammo[1]);
				newWeapon.setSpawner(0.13, 0.02);
				newWeapon.weaponDamage = 0;
				newWeapon.setTexture((loadPNG("assets/textures/turretGun.png")));
				newWeapon.fireDelay = 100;
				newWeapon.animatedWeapon = true;
				newWeapon.spritesInSet = 4;
				newWeapon.currentFrame = 0;
			}
			else
				if (input == " AI_Unarmed") {
					newWeapon = Weapon(0, 0, 0.26, 0.1, ammo[1]);
					newWeapon.setSpawner(0.13, 0.02);
					newWeapon.weaponDamage = 0;
					newWeapon.setTexture((loadPNG("assets/textures/blank.png")));
					newWeapon.fireDelay = 10000000;
					newWeapon.animatedWeapon = true;
					newWeapon.spritesInSet = 4;
					newWeapon.currentFrame = 0;
				}
				else
					if (input == " AI_PPlant") {
						newWeapon = Weapon(0, 0, 0.15, 0.15, ammo[2]);
						newWeapon.setSpawner(0.0, 0.0);
						newWeapon.weaponDamage = 300;
						newWeapon.setTexture((loadPNG("assets/textures/plantHead.png")));
						newWeapon.fireDelay = 80;
						newWeapon.animatedWeapon = true;
					}
	return newWeapon;
}
void Level::loadEnd(string & input)
{
	float values[2];
	string in;
	istringstream ss(input);
	int i = 0;
	while (getline(ss, in, ',') && i < 2) {
		values[i] = stof(in);
		i++;
	}
	this->endPoint.basicBox.set(values[0], values[1]);
}
void Level::loadMusic(string &input) {
	char* filename = new char[input.length() + 1];
	strcpy(filename, input.c_str());
	this->levelMusic = Mix_LoadMUS(filename);
}
void Level::setRepeat(string &input, vector <Terrain> &type) {
	float values[3];
	string in;
	istringstream ss(input);
	int i = 0;
	while (getline(ss, in, ',') && i < 3) {
		values[i] = stof(in);
		i++;
	}
	Terrain * target = &type[values[0]];
	if (values[1] != 0) {
		target->setXRepeat(values[1]);
	}
	if (values[2] != 0) {
		target->setYRepeat(values[2]);
	}
}
Effect Level::makeEffect(string input) {
	float pos[4];
	int effectManifest;
	string in;
	istringstream ss(input);
	int i = 0;
	while (getline(ss, in, ',') && i < 4) {
		pos[i] = stof(in);
		i++;
	}
	getline(ss, in, ',');
	effectManifest = stoi(in);
	Effect finished = Effect(pos[0], pos[1], pos[2], pos[3], levelEffect[effectManifest]);
	getline(ss, in, ',');
	effectManifest = stoi(in);
	finished.loopType = effectManifest;
	return finished;
}
void Level::loadEffect(string &input, vector <Effect> &type) {
	string fileName;
	int spriteSet;
	int speed;
	string in;
	istringstream ss(input);
	getline(ss, in, ',');
	fileName = in;
	cout << fileName << endl;
	char* filename = new char[fileName.length() + 1];
	strcpy(filename, fileName.c_str());
	type.push_back(Effect(1, 1, 1, 1, loadPNG(filename)));
	getline(ss, in, ',');
	spriteSet = stoi(in);
	type[type.size() - 1].framesInSet = spriteSet;
	getline(ss, in, ',');
	speed = stoi(in);
	cout << "$$$$ TickRate = " << speed << endl;
	type[type.size() - 1].tickRate = speed;
}

void Level::draw(bool debugMode)
{

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (Terrain &terrain : background)
	{
		terrain.setColour(1, 1, 1);
		terrain.drawTextured();
	}

	for (Terrain &terrain : levelGeometry)
	{
		terrain.setColour(1, 1, 1);
		terrain.drawTextured();
		if (debugMode) {
			terrain.drawBox();
		}
	}
	if (player.alive) {
		player.drawPlayer();
		if (debugMode) {
			player.drawBox();
		}
	}

	for (AiPlayer &ai : bots)
	{
		ai.drawAiPlayer();
		if (debugMode) {
			ai.drawBox();
			if (ai.hasCritical) {
				ai.criticalRegion.drawBox();
			}
		}
	}

	int count = 0;
	for (Weapon &aiGun : botsWeapons)
	{
		aiGun.weapDraw(Point(bots[count].basicBox.getXmid(), bots[count].basicBox.getYmid()));
		count++;
		if (debugMode) {
			aiGun.drawBox();
			aiGun.projDraw(true);
		}
	}
	if (player.alive) {
		switch (currentWeapon)
		{
		case ROCKETLAUNCHER:
			playerRocketLauncher.weapDraw(player.weaponMount);
			if (debugMode) {
				playerRocketLauncher.projDraw(true);
			}
			break;
		case LASER:
			
			if (debugMode) {
				playerRocketLauncher.projDraw(true);
				playerLaser.weapDraw(player.weaponMount,true);
			}
			else {
				playerLaser.weapDraw(player.weaponMount, false);
				playerRocketLauncher.projDraw(false); // Keep drawing the rockets even if we switch weapon
			}
			break;
		default:
			break;
		}
	}
	for (Collectible &col : powerUPs) {
		if (col.active) {
			col.drawTextured();
			if (debugMode) {
				col.drawBox();
			}
		}
	}
	for (Terrain &terrain : foreground) // access by reference to avoid copying
	{
		terrain.setColour(1, 1, 1);
		terrain.drawTextured();
	}
	for (Effect &effect : animatedEffects) // access by reference to avoid copying
	{
		effect.DrawEffect(true);
	}

	if (debugMode) {
		for (Point &p : intersectionMarkers) // access by reference to avoid copying
		{
			glPointSize(10.0);
			glBegin(GL_POINTS);
			glVertex2f(p.getX(), p.getY());
			glEnd();
		}
	}
	/* Uncomment to draw points of intersection*/
	endPoint.drawTextured();
	if (debugMode) {
		endPoint.drawBox();
	}
	//End of game hud
	if (!player.alive) {
		glColor4f(0, 0, 0, deathScreenAlpha);
		deathScreenAlpha += 0.005*speedMultiplier;
		glBegin(GL_QUADS);
		glVertex2f(-10, -10);
		glVertex2f(10, -10);
		glVertex2f(10, 10);
		glVertex2f(-10, 10);
		glEnd();
		glColor4f(1, 1, 1, deathScreenAlpha);
		stringstream lvlScore;
		lvlScore << "Level Score: ";
		lvlScore << player.points;
		stringstream totalScore;
		totalScore << "Total Score: ";
		if (!player.goalAchieved) {
			player.drawDead();
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
			myfont->Begin();
			myfont->DrawString("YOU DIED", 0.002, -0.5, 0.5);
			myfont->DrawString(lvlScore.str(), 0.001, -0.5, 0.2);
			totalScore << player.points;
			myfont->DrawString(totalScore.str(), 0.001, -0.5, 0.1);

		}
		else {
			stringstream timeBonus;
			stringstream completionBonus;
			float scoreBonus = (60 - (this->clock)) * 50;
			if (scoreBonus < 0) { scoreBonus = 0; };
			timeBonus << "Time Bonus: ";
			timeBonus << fixed << setprecision(0) << scoreBonus;
			completionBonus << "Completion Bonus: " << LEVELCOMPLETIONBONUS;
			totalScore << fixed << setprecision(0) << (player.points + scoreBonus + LEVELCOMPLETIONBONUS);
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
			myfont->Begin();
			myfont->DrawString("YOU WON", 0.002, -0.5, 0.5);
			myfont->DrawString(lvlScore.str(), 0.001, -0.5, 0.2);
			myfont->DrawString(timeBonus.str(), 0.001, -0.5, 0.1);
			myfont->DrawString(completionBonus.str(), 0.001, -0.5, 0.0);
			myfont->DrawString(totalScore.str(), 0.001, -0.5, -0.1);
		}
	}
}


void Level::playerAttack(bool isAttack)
{
	if (player.alive) {
		if (isAttack) {
			switch (currentWeapon)
			{
			case ROCKETLAUNCHER:
				playerRocketLauncher.attack();
				playerLaser.firing = false;
				break;
			case LASER:
				playerLaser.attack();
				break;
			default:
				break;
			}

		}
		else {
			playerLaser.firing = false;
		}
	}
	else {
		playerLaser.firing = false;
	}
}

void Level::aimBots()
{
	int i = 0;
	while (i < bots.size() && i < botsWeapons.size()) {
		if (bots[i].canShoot) {
			float dirToEnemyX = bots[i].basicBox.getXmid() - player.basicBox.getXmid();
			float dirToEnemyY = bots[i].basicBox.getYmid() - (player.basicBox.getYmax());
			float playerRotation = atan2(-dirToEnemyY, -dirToEnemyX);
			botsWeapons[i].aim(-playerRotation);
		}
		i++;
	}
}

void Level::switchWeapon()
{
	currentWeapon++;
	if (currentWeapon > maxWeapons) {
		currentWeapon = 0;
	}
}
void Level::respawnPlayer() {
	player.respawn = false;
	Point movement = player.respawnPoint * -1;
	for (Terrain &terrain : background) // access by reference to avoid copying
	{
		terrain.move(movement);
	}
	for (Terrain &terrain : levelGeometry) // access by reference to avoid copying
	{
		terrain.move(movement);
	}
	for (Collectible &col : powerUPs) {
		col.move(movement);
	}
	for (Terrain &terrain : foreground) // access by reference to avoid copying
	{
		terrain.move(movement);
	}
	for (Effect &effect : animatedEffects) // access by reference to avoid copying
	{
		effect.move(movement);
	}
	for (AiPlayer &ai : bots) // access by reference to avoid copying
	{
		ai.move(movement);
	}
	endPoint.move(movement);
	player.respawnPoint = Point(0, 0);
}
void Level::update(double speedMultiplier, GameObject cursor)
{
	if (player.respawn) {
		respawnPlayer();
	}
	player.speedHealMuliplier = speedMultiplier;
	intersectionMarkers.clear(); //Remove last frames intersection points
	cleanUp(); // Removes destroyed terrain and dead bots

	//Update time since last shot
	playerRocketLauncher.lastFired -= speedMultiplier;

	//Code to aim player weapon
	Point target = Point(cursor.basicBox.getXmid(), cursor.basicBox.getYmid());
	float dirToCursorX = player.basicBox.getXmid() - cursor.basicBox.getXmid();
	float dirToCursorY = player.basicBox.getYmid() - cursor.basicBox.getYmid();
	float cursorRotation = atan2(dirToCursorX, dirToCursorY);
	playerRocketLauncher.aim(cursorRotation + 1.5708);
	playerLaser.aim(cursorRotation + 1.5708);

	//move background - first background element will alway scrolls
	background[0].texofset += speedMultiplier*0.01;

	//Advance animation
	if (ticks >= 5) {
		ticks = 0;
		for (Effect &effect : animatedEffects) {
			effect.addTick();
		}
	}

	bool flag = false;
	this->intersectionMarkers.push_back(playerLaser.getMaxPoint());
	playerLaser.endPoint = playerLaser.getMaxPoint();
	for (Terrain &terrain : levelGeometry) {

		//Check if the player laser intersects terrain, clip if it does
		if (currentWeapon == LASER && playerLaser.checkHit(terrain.basicBox, playerLaser.endPoint)) {
			this->intersectionMarkers.push_back(playerLaser.getCollisionPoint(terrain.basicBox));
			playerLaser.endPoint = playerLaser.getCollisionPoint(terrain.basicBox);
		}

		//Check the ai projectiles for collision with terrain
		for (Weapon &aiGun : botsWeapons) {
			aiGun.checkProjectileCollision(terrain.basicBox, animatedEffects);
		}

		//Check the rocket launcher for collsion with terrian
		if (playerRocketLauncher.checkProjectileCollision(terrain.basicBox, animatedEffects)) {
			//if terrain is destroyable, destroy it
			if (terrain.isDestroyable) {
				terrain.isDestroyed = true;
				terrain.drawable = false;
				terrain.setGround(false);
				animatedEffects.push_back(terrain.destroyedEffect);
				animatedEffects[animatedEffects.size() - 1].basicBox.set(terrain.basicBox.getXmid(), terrain.basicBox.getYmid());
				animatedEffects[animatedEffects.size() - 1].basicBox.resize(terrain.basicBox.getXSize(), terrain.basicBox.getYSize());
			}
		}
		// check if the player is on the ground
		if (terrain.checkOnGround(player.basicBox)) {
			flag = true;
		}
	}

	//Update the player on ground status
	player.setOnGround(flag);

	//Check if the ai hit the player
	for (Weapon &aiGun : botsWeapons) {
		if (aiGun.checkProjectileCollision(player.basicBox, animatedEffects)) {
			player.getHit(aiGun.weaponDamage);
		}
	}


	AiPlayer * toBeHit; // Closest ai we have found that will be hit
	bool foundTarget = false; // If any ai will be hit 


	for (AiPlayer &ai : bots) {
		//Check if the laser hits
		if (playerLaser.checkHit(ai.basicBox, playerLaser.endPoint)) {
			this->intersectionMarkers.push_back(playerLaser.getCollisionPoint(ai.basicBox));
			//Clip the end of the laser
			playerLaser.endPoint = playerLaser.getCollisionPoint(ai.basicBox);
			toBeHit = &ai;
			foundTarget = true;
		}
		//Check if the player is standing on the ai head
		if (ai.checkOnTop(player)) {

			//make the player jump and damage us
			player.setJump(true);
			ai.healthPoints -= 300;
			//kill us if needed
			if (ai.healthPoints <= 0) {
				ai.alive = false;
				//crit points for head jump kill
				player.points += ai.pointsForCritKill;
			}
		}
	}
	if (foundTarget && playerLaser.firing) {
		toBeHit->healthPoints -= playerLaser.weaponDamage *speedMultiplier*(float)player.damageMultiplier;
		//Did the laser kill us?
		if (toBeHit->healthPoints <= 0) {
			toBeHit->alive = false;
			player.points += toBeHit->pointsForKill;
		}
	}

	//Generate the vector for moving the level
	Point movement = player.getTranslation(speedMultiplier);

	bool hitHead = false; // True if we want to stop the jump due to top collision

	for (Terrain &terrain : levelGeometry) {
		//Check if we are going to collide
		if (terrain.basicBox.axisAlinedTest(player.basicBox, movement.getX(), movement.getY())) {
			terrain.basicBox.resolveColision(player.basicBox, movement, hitHead);
		}
	}

	for (AiPlayer &ai : bots) {
		//Check if we are going to collide
		if (ai.basicBox.axisAlinedTest(player.basicBox, movement.getX(), movement.getY())) {
			ai.basicBox.resolveColision(player.basicBox, movement, hitHead);
		}

		//Check if the players rockets hit critical
		if (playerRocketLauncher.canCritHit && ai.hasCritical && playerRocketLauncher.checkProjectileCollision(ai.criticalRegion, animatedEffects)) {
			ai.healthPoints -= playerRocketLauncher.weaponDamage*player.damageMultiplier*2.5;
			if (ai.healthPoints <= 0) {
				ai.alive = false;
				player.points += ai.pointsForCritKill;
				Mix_PlayChannel(-1, this->critSFX, 0);
			}
		}
		//Check if the players rockets hit non-critical
		else if (playerRocketLauncher.checkProjectileCollision(ai.basicBox, animatedEffects)) {
			ai.healthPoints -= playerRocketLauncher.weaponDamage*player.damageMultiplier;
			if (ai.healthPoints <= 0) {
				ai.alive = false;
				player.points += ai.pointsForKill;
			}
		}

	}
	if (hitHead) {
		player.setJump(false);
		player.setYSpeed(0);
	}


	for (Terrain &terrain : background) // update with new position
	{
		terrain.move(movement);
	}

	for (Terrain &terrain : levelGeometry) //  update with new position
	{
		terrain.move(movement);
	}

	for (Collectible &col : powerUPs) {
		col.updateClock(this->speedMultiplier, player, playerRocketLauncher); //Power up timers
		col.move(movement);//  update with new position
		if (col.active && col.basicBox.axisAlinedTest(player.basicBox)) {
			col.collisionAction(player, playerRocketLauncher);//Pick up the power up
		}
	}

	int currentAI = 0;
	for (AiPlayer &ai : bots)
	{
		ai.move(movement); // update with new position
		bool flag = false; // Check if we need to fall
		for (Terrain &terrain : levelGeometry) {
			if (terrain.checkOnGround(ai.basicBox)) {
				flag = true;
			}
		}
		ai.onGround = flag;

		//Generate our intended movement
		Point aiMovement = ai.react(speedMultiplier, player);

		//Invert out movement as these tests were designed for the player to move the level
		aiMovement.setX(aiMovement.getX()*-1);
		aiMovement.setY(aiMovement.getY()*-1);

		for (Terrain &terrain : levelGeometry) {
			//Check collision with level
			if (terrain.basicBox.axisAlinedTest(ai.basicBox, aiMovement.getX(), aiMovement.getY())) {
				terrain.basicBox.resolveColision(ai.basicBox, aiMovement, hitHead);
			}
		}
		//Check collision with player
		if (player.basicBox.axisAlinedTest(ai.basicBox, aiMovement.getX(), aiMovement.getY())) {
			player.basicBox.resolveColision(ai.basicBox, aiMovement, hitHead);
		}
		//Check collision with other ai
		for (int i = currentAI + 1; i < bots.size(); i++) {
			if (ai.basicBox.axisAlinedTest(bots[i].basicBox, aiMovement.getX(), aiMovement.getY())) {
				ai.basicBox.resolveColision(bots[i].basicBox, aiMovement, hitHead);
			}
		}

		//Revert the inversion
		aiMovement.setY(aiMovement.getY()*-1);
		aiMovement.setX(aiMovement.getX()*-1);
		//Move to new position
		ai.move(aiMovement);

		//Update movement for distance based animation
		if (aiMovement.getX() < 0) {
			ai.distanceMovedX += (aiMovement.getX()*-1);
		}
		else {
			ai.distanceMovedX += aiMovement.getX();
		}
		if (ai.distanceMovedX > 0.1) {
			ai.nextFrame();
			ai.distanceMovedX = 0;
		}
		currentAI++;
	}


	int count = 0;//Keep track of which ai this weapon is for
	aimBots(); //Aim ai weapons

	for (Weapon &aiGun : botsWeapons)
	{
		aiGun.lastFired -= speedMultiplier;
		bool intersectionCheck = false;
		aiGun.rebind(Point(bots[count].basicBox.getXmid(), bots[count].basicBox.getYmid()));
		aiGun.updateProjectiles(movement, speedMultiplier);
		for (Terrain &terrain : levelGeometry) {
			Point test;
			//Check if we have line of sight on player
			if (intersectionCheck == false && bots[count].canShoot && terrain.basicBox.checkLineIntersection(Point(bots[count].basicBox.getXmid(), bots[count].basicBox.getYmid()), Point(player.basicBox.getXmid(), player.basicBox.getYmid()), test)) {
				intersectionCheck = true;
				intersectionMarkers.push_back(test);
			}
		}
		//Fire at player if we have line of sight
		if (!intersectionCheck && bots[count].canShoot) {
			aiGun.attack();
		}
		count++;
	}
	for (Terrain &terrain : foreground) //update to new position
	{
		terrain.move(movement);
	}
	for (Effect &effect : animatedEffects) //update to new position
	{
		effect.move(movement);
	}
	if (cursor.basicBox.getXmid() > 0) { // Flip our weapon if we are aiming the other way, make it not upside down
		playerRocketLauncher.mirror(false);
		playerLaser.mirror(false);
	}
	else {
		playerRocketLauncher.mirror(true);
		playerLaser.mirror(true);
	}

	playerRocketLauncher.updateProjectiles(movement, speedMultiplier);//Move our rockets
	if (currentWeapon == ROCKETLAUNCHER) {
		playerRocketLauncher.rebind(player.weaponMount);
	}
	if (currentWeapon == LASER) {
		playerLaser.rebind(player.weaponMount);
	}

	//Update our walking animation
	if (player.distanceMovedX > 0.1) {
		player.animateUpdate();
		player.distanceMovedX = 0;
	}
	//Check if we have hit the end marker
	if (player.basicBox.axisAlinedTest(endPoint.basicBox)) {
		player.alive = false;
		player.finished();
	}
	//Move the end point and respawn points
	player.respawnPoint.move(movement.getX(), movement.getY());
	endPoint.move(movement);
}
