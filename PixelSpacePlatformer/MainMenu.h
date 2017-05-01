#pragma once
#ifndef MAINMENU_H
#define MAINMENU_H
#include "GameObject.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
using namespace std;
#include "glfont2.h"
using namespace glfont;

class LevelButton : public GameObject {
private:
	
public:
	string levelName;
	string levelPath;
	static GLuint backGround;
	GLFont * font;
	void draw();
	LevelButton(string title,string filePath, float yPos);
	float getLowerEdge() { return this->basicBox.getYmin(); };
};

class MainMenu {
private:
	
	
public:
	int state = 0;
	bool startPlaying = false;
	string filePath = "";
	vector<LevelButton> level;
	float alpha = 1;
	bool isActive = true;
	vector<GameObject>buttons;
	vector<GameObject>background;
	void drawMenu();
	void checkButtons(GameObject cursor);
	MainMenu(GLFont * font);
	MainMenu();
	void loadLevelPath(string line, vector<LevelButton> &levels);
	void reset();
};
#endif