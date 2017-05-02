#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "Box.h"
#include "nvImage.h"
GLuint loadPNG(char* name);
class GameObject {
private:
	
	float xRepeat = 1;
	float yRepeat = 1;
	float alpha = 1;
public:
	float texofset = 0;
	GameObject();
	GameObject(float spawnXpos, float spawnYpos, float xSize, float ySize);
	void drawBox();
	void drawTextured();
	inline void setTexture(GLuint i);
	inline void loadThisTexture(char* name) { setTexture(loadPNG(name)); };
	inline void setXRepeat(float i);
	inline void setYRepeat(float i);
	inline void setColour(float r, float g, float b);
	inline void setAlpha(float a) { alpha = a; };
	inline float getAlpha() { return alpha; };
	virtual void move(Point &p);
	Box basicBox;
	bool drawable = true;
	GLuint textureID = 0;
protected:
	
	float red; float green; float blue;
};
inline void GameObject::setColour(float r, float g, float b) {
	this->red = r;
	this->green = g;
	this->blue = b;
}

inline void GameObject::setTexture(GLuint i)
{
	this->textureID = i;
}
inline void GameObject::setXRepeat(float i) {
	this->xRepeat = i;
}
inline void GameObject::setYRepeat(float i) {
	this->yRepeat = i;
}
#endif