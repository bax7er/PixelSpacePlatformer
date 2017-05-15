#ifndef BOX_H
#define BOX_H
#include "Point.h"
class Box {
private:
	Point points[4];
	Point mid;
	float xSize;
	float ySize;
	bool canSeperate(float projection[4], float length);
public:
	inline float getXSize();
	inline float getYSize();
	Point* getPoints();
	Box();
	Box(float spawnXpos, float spawnYpos, float xSize, float ySize);
	void copy(Box &box);
	void drawBox();
	float getXmin();
	float getXmax();
	float getYmin();
	float getYmax();
	float getXmid();
	float getYmid();
	void move(float xMove,float yMove);
	void rotate(float angle);
	void rotate(Point center,float angle);
	void set(double xPos, double yPos);
	bool axisAlinedTest(Box &other);
	bool axisAlinedTest(Box &other, float xShift, float yShift);
	bool SepAxisText(Box &other, float xOffset = 0.0, float yOffset = 0.0);
	void resize(float xChange, float yChange);
	void resolveColision(Box &other, Point & movement, bool &headHit);
	void resolveColisionAI(Box &other, Point & movement, bool &headHit);
	bool checkLineIntersection(Point a, Point b,Point &intersection);
	bool findFirstPointLineIntersection(Point a, Point b, Point &intersection);
};
inline float Box::getXSize() {
	return this->xSize;
}
inline float Box::getYSize() {
	return this->ySize;
}
#endif