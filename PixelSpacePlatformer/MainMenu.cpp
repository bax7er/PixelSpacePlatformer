#define TITLESCREEN 0
#define LEVELSELECT 1
#include "MainMenu.h"
#include <windows.h>	
GLuint LevelButton::backGround = 99;
string getFileLocation(char * windowMessage);
void MainMenu::drawMenu()
{
		for (GameObject g : background) {
			g.setColour(1, 1, 1);
			g.setAlpha(1);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			g.drawTextured();
		}
		if (state == TITLESCREEN) {
			for (GameObject b : buttons) {
				b.setColour(1, 1, 1);
				b.setAlpha(1);
				b.drawTextured();
			}
		}
		if (state == LEVELSELECT) {
			for (LevelButton b : level) {
				b.draw();
			}
	}
}

void MainMenu::checkButtons(GameObject cursor){

	if (state == TITLESCREEN) {

		if (buttons[0].basicBox.axisAlinedTest(cursor.basicBox)) {
			cout << "Hit Play" << endl;
			background[2].drawable = true;
			background[1].drawable = false;
			background[0].drawable = true;
			buttons[0].drawable = false;
			buttons[1].drawable = false;
			state = 1;
			return;
		}
		if (buttons[1].basicBox.axisAlinedTest(cursor.basicBox)) {
			cout << "Hit Quit" << endl;
			exit(EXIT_FAILURE);
		}
	}
	if (state == LEVELSELECT) {
		for (LevelButton &b : level) {
			if (b.basicBox.axisAlinedTest(cursor.basicBox)) {
				if (b.levelPath != "$") {
					this->filePath = b.levelPath;
					this->startPlaying = true;
					return;
				}
				else {
					this->filePath = getFileLocation("Select a level file to load");
					if (filePath != "") {
						this->startPlaying = true;
					}
				}
			}
		}
}
}

MainMenu::MainMenu(GLFont * font)
{
	ifstream data("levels.MANIFEST");
	string line;
	while (getline(data, line))
	{
		loadLevelPath(line, level);
	}
	float yPos = 0;
	if ((int)level.size() > 0) {
		yPos = level[level.size() - 1].getLowerEdge();
		yPos += 0.1;
	}
	level.push_back(LevelButton("Load other", "$", yPos));
	for (LevelButton &b : level) {
		b.font = font;
	}
	state = TITLESCREEN;
	background.push_back(GameObject(0, 0, 8, 5));
	background[0].loadThisTexture("assets/textures/mainMenu.png");
	background.push_back(GameObject(0, 0.5, 1.5, 1));
	background[1].loadThisTexture("assets/textures/logo.png");
	background.push_back(GameObject(0, 0, 1.75, 1.9));
	background[2].loadThisTexture("assets/textures/levelSelectBackGround.png");
	background[2].drawable = false;
	buttons.push_back(GameObject(0, -0.20, 1.5, 0.25));
	buttons[0].loadThisTexture("assets/textures/play.png");
	buttons.push_back(GameObject(0, -0.60, 1.5, 0.25));
	buttons[1].loadThisTexture("assets/textures/exit.png");
	LevelButton::backGround = loadPNG("assets/textures/shipHull.png");
}

MainMenu::MainMenu()
{
}

void MainMenu::loadLevelPath(string line, vector<LevelButton>& levels)
{
	string fileName;
	string levelName;
	string in;
	istringstream ss(line);
	getline(ss, fileName, ',');
	getline(ss, levelName);
	cout << fileName <<"'"<<levelName<< endl;
	float yPos = 0;
	if (levels.size() != 0) {
		yPos = levels[levels.size() - 1].getLowerEdge();
	}
	else {
		yPos = 1;
	}
	yPos += 0.1;
	levels.push_back(LevelButton(levelName,fileName,yPos));
}

void MainMenu::reset()
{
	state = TITLESCREEN;
	background[2].drawable = false;
	background[1].drawable = true;
	background[0].drawable = true;
	buttons[0].drawable = true;
	buttons[1].drawable = true;
}

void LevelButton::draw()
{
	this->setColour(1, 1, 1);
	this->textureID = LevelButton::backGround;
	this->drawTextured();
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	font->Begin();
	glLoadIdentity();
	glPushMatrix();
	font->DrawString(levelName, 0.002, this->basicBox.getXmin(), this->basicBox.getYmax());
	glPopMatrix();
}

LevelButton::LevelButton(string title, string filePath,float yPos) :GameObject (0,yPos-0.4,1.5, 0.25)
{
	levelName = title;
	levelPath = filePath;
	this->textureID = LevelButton::backGround;
}

string getFileLocation(char * windowMessage) {
	char filename[MAX_PATH];

	OPENFILENAME ofn;
	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
	ofn.lpstrFilter = "Level Files\0*.lvl\0Any File\0*.*\0";
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = windowMessage;// 
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

	if (GetOpenFileNameA(&ofn))
	{
		std::cout << "You chose the file \"" << filename << "\"\n";
	}
	else
	{
		// All this stuff below is to tell you exactly how you messed up above. 
		// Once you've got that fixed, you can often (not always!) reduce it to a 'user cancelled' assumption.
		switch (CommDlgExtendedError())
		{
		case CDERR_DIALOGFAILURE: std::cout << "CDERR_DIALOGFAILURE\n";   break;
		case CDERR_FINDRESFAILURE: std::cout << "CDERR_FINDRESFAILURE\n";  break;
		case CDERR_INITIALIZATION: std::cout << "CDERR_INITIALIZATION\n";  break;
		case CDERR_LOADRESFAILURE: std::cout << "CDERR_LOADRESFAILURE\n";  break;
		case CDERR_LOADSTRFAILURE: std::cout << "CDERR_LOADSTRFAILURE\n";  break;
		case CDERR_LOCKRESFAILURE: std::cout << "CDERR_LOCKRESFAILURE\n";  break;
		case CDERR_MEMALLOCFAILURE: std::cout << "CDERR_MEMALLOCFAILURE\n"; break;
		case CDERR_MEMLOCKFAILURE: std::cout << "CDERR_MEMLOCKFAILURE\n";  break;
		case CDERR_NOHINSTANCE: std::cout << "CDERR_NOHINSTANCE\n";     break;
		case CDERR_NOHOOK: std::cout << "CDERR_NOHOOK\n";          break;
		case CDERR_NOTEMPLATE: std::cout << "CDERR_NOTEMPLATE\n";      break;
		case CDERR_STRUCTSIZE: std::cout << "CDERR_STRUCTSIZE\n";      break;
		case FNERR_BUFFERTOOSMALL: std::cout << "FNERR_BUFFERTOOSMALL\n";  break;
		case FNERR_INVALIDFILENAME: std::cout << "FNERR_INVALIDFILENAME\n"; break;
		case FNERR_SUBCLASSFAILURE: std::cout << "FNERR_SUBCLASSFAILURE\n"; break;
		default: std::cout << "You cancelled.\n";
		}
	}
	return filename;
}