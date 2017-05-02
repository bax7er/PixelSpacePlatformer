#pragma once
#include "LEVEL.h";
#define TEXTURE 1
#define GEOMETRY 2
#define FOREGROUND 3
#define BACKGROUND 4
#define MUSIC 4
void Level::movePlayer(double movement)
{
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
void Level::playerJump()
{
	if (player.isOnGround()) {
		player.setJump(true);
	}
}
Level::Level() {

}
Level::Level(string filename) {
	ifstream  data1("effects.MANIFEST");

	string line;
	while (getline(data1, line))
	{
		loadEffect(line,levelEffect);
	}
	data1.close();
	ifstream  playerData("Default.plr");
	{
		getline(playerData, line,'#');
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
				if (line[2] =='R') {

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

			}
			
		}
		else {
			cout << line << endl;
		}
	}
	data.close();

	player.setMountPoint(0, -0.18);
	Projectile rocket = Projectile(0, 0, 0.1, 0.05, 0.1);
	rocket.effectTexture = loadPNG("assets/textures/Explode.png");
	rocket.setTexture(loadPNG("assets/textures/rocket1.png"));
	playerWeapon = Weapon(0, 0, 0.26, 0.1, rocket);
	playerWeapon.setBinding(0.05, 0);
	playerWeapon.setSpawner(0.13, 0.02);
	playerWeapon.weaponDamage = 300;
	playerWeapon.setTexture(loadPNG("assets/textures/rocketLauncher.png"));
	playerWeapon.explosion = Mix_LoadWAV("assets/music/Explosion3.wav");
	//
	bots.push_back(AiPlayer(0.0, 1, 0.2, 0.2));
	bots[0].setTexture(loadPNG("assets/textures/Player.png"));
	bots[0].setColour(0.9, 0.5, 0.2);
	bots[0].weaponMount = Point(1,0);
	botsWeapons.push_back(Weapon(0, 0, 0.26, 0.1, rocket));
	botsWeapons[0].setSpawner(0.13, 0.02);
	botsWeapons[0].weaponDamage = 100;
	botsWeapons[0].setTexture(playerWeapon.textureID);
	bots[0].gun = &botsWeapons[0];
	//
	Projectile bullet = Projectile(0, 0, 0.05, 0.07, 0.1);
	bullet.effectTexture = loadPNG("assets/textures/Explode.png");
	bullet.setTexture(loadPNG("assets/textures/minigunBullet.png"));
	//bots.push_back(AiPlayer(-1.4, 0.85, 0.2, 0.2));
	bots.push_back(AiPlayer(-1.4, 0.5, 0.2, 0.2));
	bots[1].setTexture(loadPNG("assets/textures/turretBody.png"));
	bots[1].spritesOnSheet = 1;
	bots[1].maxHP = 3000;
	bots[1].healthPoints = bots[1].maxHP;
	bots[1].hasCritical = false;
	bots[1].setColour(1, 1, 1);
	bots[1].weaponMount = Point(1, 0);
	bots[1].aiMode = AIMODE_STATIC;
	botsWeapons.push_back(Weapon(0, 0, 0.26, 0.1,bullet));
	botsWeapons[1].setSpawner(0.13, 0.02);
	botsWeapons[1].weaponDamage = 50;
	botsWeapons[1].setTexture((loadPNG("assets/textures/turretGun.png")));
	botsWeapons[1].fireDelay = 10;
	bots[1].gun = &botsWeapons[1];
	//
	critSFX = Mix_LoadWAV("assets/music/headshot.wav");
}
void Level::setDestroyable(string &input, vector <Terrain> &type) {
	int values[2];
	string in;
	istringstream ss(input);
	int i = 0;
	while (getline(ss, in, ',') && i< 2) {
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
}
Terrain Level::makeObjectTerrain(string input) {
	float values[5];
	string in;
	istringstream ss(input);
	int i = 0;
	while (getline(ss, in, ',') && i<5) {
		values[i]= stof(in);
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
	return finished;
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
	while (getline(ss, in, ',') && i< 3) {
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
	type[type.size()-1].framesInSet = spriteSet;
	getline(ss, in, ',');
	speed = stoi(in);
	cout << "$$$$ TickRate = " << speed << endl;
	type[type.size() - 1].tickRate = speed;
}

void Level::draw()
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (Terrain &terrain : background) // access by reference to avoid copying
	{
		//terrain.move(translation);
		terrain.setColour(1, 1, 1);
		terrain.drawTextured();
	}

	for (Terrain &terrain : levelGeometry) // access by reference to avoid copying
	{
		//terrain.move(translation);
		terrain.setColour(1, 1, 1);
		terrain.drawTextured();
	}
	player.drawPlayer();
	playerWeapon.weapDraw(player.weaponMount);
	for (AiPlayer &ai : bots) // access by reference to avoid copying
	{
		//ai.setColour(1, 0, 0.2);
		ai.drawAiPlayer();
		//ai.gun->weapDraw(Point(ai.basicBox.getXmid(), ai.basicBox.getYmid()));
	}

	int count = 0;
	for (Weapon &aiGun : botsWeapons) // access by reference to avoid copying
	{
	//	aiGun.setColour(1, 0, 0.2);
		aiGun.weapDraw(Point(bots[count].basicBox.getXmid(), bots[count].basicBox.getYmid()));
		count++;
	}
	for (Terrain &terrain : foreground) // access by reference to avoid copying
	{
		//terrain.move(translation);
		terrain.setColour(1, 1, 1);
		terrain.drawTextured();
	}
	for (Effect &effect : animatedEffects) // access by reference to avoid copying
	{
		//effect.move(translation);
		effect.DrawEffect(true);
	}
	for (Point &p : intersectionMarkers) // access by reference to avoid copying
	{
		glPointSize(10.0);
		glBegin(GL_POINTS);
		glVertex2f(p.getX(), p.getY());
		glEnd();
	}
}

void Level::movePlayer()
{
}

void Level::playerAttack()
{
	playerWeapon.attack();
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

void Level::update(double speedMultiplier,GameObject cursor)
{
	intersectionMarkers.clear();
	printf("Player points: %i \n", player.points);
	/*
	Point target = Point(cursor.basicBox.getXmid(), cursor.basicBox.getYmid());
	target.Normalise();
	double dot = DotProduct(player.basicBox.getXmid() + 1, player.basicBox.getXmid(), target);
	double angle = (acos(dot));
	if (cursor.basicBox.getYmid() < 0) {
		angle *= -1;
	}
	playerWeapon.aim(-angle);
	*/
	cleanUp(); // Removes destroyed terrain and dead bots
	//aimBots();
	playerWeapon.lastFired -= speedMultiplier;
	Point target = Point(cursor.basicBox.getXmid(), cursor.basicBox.getYmid());

	float dirToCursorX = player.basicBox.getXmid()- cursor.basicBox.getXmid();
	float dirToCursorY = player.basicBox.getYmid()- cursor.basicBox.getYmid();

	float cursorRotation = atan2(dirToCursorX, dirToCursorY);
	playerWeapon.aim(cursorRotation+ 1.5708);

	background[0].texofset += speedMultiplier*0.01;
	if (ticks >= 5) {
		ticks = 0;
		for (Effect &effect : animatedEffects) {
			effect.addTick();
		}
	}
	
	bool flag = false;
	
	for (Terrain &terrain : levelGeometry) {
		
		for (Weapon &aiGun : botsWeapons) {
			aiGun.checkProjectileCollision(terrain.basicBox, animatedEffects);
		}
		if (playerWeapon.checkProjectileCollision(terrain.basicBox, animatedEffects)) {
			if (terrain.isDestroyable) {
				terrain.isDestroyed = true;
				terrain.drawable = false;
				terrain.setGround(false);
				animatedEffects.push_back(terrain.destroyedEffect);
				animatedEffects[animatedEffects.size() - 1].basicBox.set(terrain.basicBox.getXmid(), terrain.basicBox.getYmid());
				animatedEffects[animatedEffects.size() - 1].basicBox.resize(terrain.basicBox.getXSize(), terrain.basicBox.getYSize());
			}
		}
		if (terrain.checkOnGround(player.basicBox)) {
			flag = true;
		}
	}
	
	
	for (Weapon &aiGun : botsWeapons) {
		if (aiGun.checkProjectileCollision(player.basicBox, animatedEffects)) {
			player.getHit(aiGun.weaponDamage);
		}
	}
	player.setOnGround(flag);
	for (AiPlayer &ai : bots) {
		if (ai.checkOnTop(player)) {
			player.setJump(true);
			ai.healthPoints -= 300;
			if (ai.healthPoints <= 0) {
				ai.alive = false;
				player.points += ai.pointsForCritKill;
			}
		}
	}
	Point movement =player.getTranslation(speedMultiplier);
	bool hitHead = false;
	for (Terrain &terrain : levelGeometry) {
		if (terrain.basicBox.axisAlinedTest(player.basicBox,movement.getX(),movement.getY())) {
			terrain.basicBox.resolveColision(player.basicBox, movement,hitHead);
		}
	}

	for (AiPlayer &ai : bots) {

		if (ai.basicBox.axisAlinedTest(player.basicBox, movement.getX(), movement.getY())) {
			ai.basicBox.resolveColision(player.basicBox, movement, hitHead);
		}
		if (playerWeapon.canCritHit && ai.hasCritical) {
			if (playerWeapon.checkProjectileCollision(ai.criticalRegion, animatedEffects)) {
				ai.healthPoints -= ai.healthPoints;
				ai.alive = false;
				player.points += ai.pointsForCritKill;
				Mix_PlayChannel(-1, this->critSFX, 0);
			}
		}else
		if (playerWeapon.checkProjectileCollision(ai.basicBox, animatedEffects)) {
			ai.healthPoints -= playerWeapon.weaponDamage;
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


	for (Terrain &terrain : background) // access by reference to avoid copying
	{
		terrain.move(movement);
	}

	for (Terrain &terrain : levelGeometry) // access by reference to avoid copying
	{
		terrain.move(movement);
	}
	int currentAI = 0;
	for (AiPlayer &ai : bots) // access by reference to avoid copying
	{
		ai.move(movement);
		bool flag = false;
		for (Terrain &terrain : levelGeometry) {
			if (terrain.checkOnGround(ai.basicBox)) {
				flag = true;
			}
		}
		ai.onGround = flag;
		Point aiMovement = ai.react(speedMultiplier, player);
		aiMovement.setX(aiMovement.getX()*-1);
		aiMovement.setY(aiMovement.getY()*-1);
		for (Terrain &terrain : levelGeometry) {
			if (terrain.basicBox.axisAlinedTest(ai.basicBox, aiMovement.getX(), aiMovement.getY())) {
				//aiMovement.setX(0);
				//aiMovement.setY(0);
				terrain.basicBox.resolveColision(ai.basicBox, aiMovement, hitHead);
			}
		}
			if (player.basicBox.axisAlinedTest(ai.basicBox, aiMovement.getX(), aiMovement.getY())) {
				//aiMovement.setX(aiMovement.getX()*-1);
				//aiMovement.setY(aiMovement.getY()*-1);
				player.basicBox.resolveColision(ai.basicBox, aiMovement, hitHead);
			}
			for (int i = currentAI + 1; i < bots.size(); i++) {
				if (ai.basicBox.axisAlinedTest(bots[i].basicBox, aiMovement.getX(), aiMovement.getY())) {
					//aiMovement.setX(aiMovement.getX()*-1);
					//aiMovement.setY(aiMovement.getY()*-1);
					ai.basicBox.resolveColision(bots[i].basicBox, aiMovement, hitHead);
				}
			}
			aiMovement.setY(aiMovement.getY()*-1);
			aiMovement.setX(aiMovement.getX()*-1);
		ai.move(aiMovement);
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
	int count = 0;
	aimBots();
	for (Weapon &aiGun : botsWeapons) // access by reference to avoid copying
	{
		aiGun.lastFired -= speedMultiplier;
		bool intersectionCheck = false;
		aiGun.rebind(Point(bots[count].basicBox.getXmid(), bots[count].basicBox.getYmid()));
		aiGun.updateProjectiles(movement, speedMultiplier);
		for (Terrain &terrain : levelGeometry) {
			Point test;
			if (terrain.basicBox.checkLineIntersection(Point(bots[count].basicBox.getXmid(), bots[count].basicBox.getYmid()), Point(player.basicBox.getXmid(), player.basicBox.getYmid()),test)) {
				intersectionCheck = true;
				intersectionMarkers.push_back(test);
			}
		}
		if (!intersectionCheck && bots[count].canShoot) {
			aiGun.attack();
		}
		count++;
	}
	for (Terrain &terrain : foreground) // access by reference to avoid copying
	{
		terrain.move(movement);
	}
	for (Effect &effect : animatedEffects) // access by reference to avoid copying
	{
		effect.move(movement);
	}
	//botsWeapons[0].updateProjectiles(movement, speedMultiplier);
	playerWeapon.updateProjectiles(movement, speedMultiplier);
	if (cursor.basicBox.getXmid() > 0) {
		playerWeapon.mirror(false);
	}
	else {
		playerWeapon.mirror(true);
	}
	playerWeapon.rebind(player.weaponMount);
	if (player.distanceMovedX > 0.1) {
		player.animateUpdate();
		player.distanceMovedX = 0;
	}


}
