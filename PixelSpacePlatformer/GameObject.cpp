#pragma once
#include "GameObject.h"
GameObject::GameObject() {
	basicBox = Box();

}
GameObject::GameObject(float spawnXpos, float spawnYpos, float xSize, float ySize) {
	red = blue = green = 1;
	basicBox = Box(spawnXpos, spawnYpos, xSize, ySize);
}
void GameObject::drawBox(){
	glColor3f(red, green, blue);
	this->basicBox.drawBox();
}

void GameObject::drawTextured() {
	if (drawable) {
		Point* points = this->basicBox.getPoints();
		glPushMatrix();
		glColor4f(red, green, blue, alpha);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glBegin(GL_QUADS);
		glTexCoord2d(texofset, 0);					glVertex2d(points[0].getX(), points[0].getY());
		glTexCoord2d(xRepeat + texofset, 0);			glVertex2d(points[1].getX(), points[1].getY());
		glTexCoord2d(xRepeat + texofset, yRepeat);		glVertex2d(points[2].getX(), points[2].getY());
		glTexCoord2d(0 + texofset, yRepeat);			glVertex2d(points[3].getX(), points[3].getY());
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
}
void GameObject::move(Point &p) {
	this->basicBox.move(p.getX(), p.getY());
}

GLuint loadPNG(char* name)
{
	// Texture loading object
	nv::Image img;

	GLuint myTextureID;

	// Return true on success
	if (img.loadImageFromFile(name))
	{
		glGenTextures(1, &myTextureID);
		glBindTexture(GL_TEXTURE_2D, myTextureID);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexImage2D(GL_TEXTURE_2D, 0, img.getInternalFormat(), img.getWidth(), img.getHeight(), 0, img.getFormat(), img.getType(), img.getLevel(0));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
	}

	else {
		std::cout << "Texture Error - unable to load:   " << ";" << name << ";" << std::endl;
		myTextureID = -1;
	}
	return myTextureID;
}