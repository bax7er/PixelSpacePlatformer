#pragma once
#include "Point.h"
#include "Box.h"
#include "nvImage.h"
#include <iostream>

Point* Box::getPoints() {
	return this->points;
}
void Box::drawBox() {
	glPushMatrix();
		glLoadIdentity();
			glBegin(GL_LINE_LOOP);
			for (int i = 0; i < 4; i++) {
				glVertex2f(this->points[i].getX(), this->points[i].getY());
			}
		glEnd();
	glPopMatrix();
}
float Box::getXmin() {
	float xmin = points[0].getX();
	for (int i = 1; i < 4; i++) {
		if (points[i].getX() < xmin) {
			xmin = points[i].getX();
		}
	}
	return xmin;
}
float Box::getXmax() {
	float xMax = points[0].getX();
	for (int i = 1; i < 4; i++) {
		if (points[i].getX() > xMax) {
			xMax = points[i].getX();
		}
	}
	return xMax;
}
float Box::getYmin() {
	float yMin = points[0].getY();
	for (int i = 1; i < 4; i++) {
		if (points[i].getY() < yMin) {
			yMin = points[i].getY();
		}
	}
	return yMin;
}
float Box::getYmax() {
	float yMax = points[0].getY();
	for (int i = 1; i < 4; i++) {
		if (points[i].getY() > yMax) {
			yMax = points[i].getY();
		}
	}
	return yMax;
}
void Box:: move(float xMove, float yMove) {
	for (int i = 0; i < 4; i++) {
		points[i].move(xMove, yMove);
	}
	this->mid.move(xMove, yMove);
}
Box::Box() {
	points[0].set(0, 0);
	points[1].set(0, 0);
	points[2].set(0, 0);
	points[3].set(0, 0);
	mid.set(0, 0);
}
Box::Box(float spawnXpos, float spawnYpos, float xSize, float ySize) {
	points[0].setX(spawnXpos - (xSize / 2));
	points[0].setY(spawnYpos - (ySize / 2));

	points[1].setX(spawnXpos + (xSize / 2));
	points[1].setY(spawnYpos - (ySize / 2));

	points[2].setX(spawnXpos + (xSize / 2));
	points[2].setY(spawnYpos + (ySize / 2));

	points[3].setX(spawnXpos - (xSize / 2));
	points[3].setY(spawnYpos + (ySize / 2));

	this->mid.setX(spawnXpos);
	this->mid.setY(spawnYpos);
	this->xSize = xSize;
	this->ySize = ySize;
}
float Box::getXmid() {
	return mid.getX();
}
float Box::getYmid() {
	return mid.getY();
}
void Box::rotate(float angle) {
	if (angle != 0 ){
		float sinTheta = sin(-angle);
		float cosTheta = cos(-angle);
		float x;
		float y;
		for (int i = 1; i < 4; i++) {
			x = points[i].getX();
			y = points[i].getY();
			x -= points[0].getX();
			y -= points[0].getY();
			points[i].setX((x*cosTheta) - (y*sinTheta));
			points[i].setY((y*cosTheta) + (x*sinTheta));
			points[i].move(points[0].getX(), points[0].getY());
		}
	}
}
void Box::rotate(Point center,float angle) {
		double sinTheta = sin(-angle);
		double cosTheta = cos(-angle);
		float x;
		float y;
		for (int i = 0; i < 4; i++) {
			x = points[i].getX();
			y = points[i].getY();
			x -= center.getX();
			y -= center.getY();
			points[i].setX((x*cosTheta) - (y*sinTheta)+ center.getX());
			points[i].setY((y*cosTheta) + (x*sinTheta)+ center.getY());
	}
}
void Box::set(double xPos, double yPos) {
	points[0].setX(xPos - (xSize / 2));
	points[0].setY(yPos - (ySize / 2));

	points[1].setX(xPos + (xSize / 2));
	points[1].setY(yPos - (ySize / 2));

	points[2].setX(xPos + (xSize / 2));
	points[2].setY(yPos + (ySize / 2));

	points[3].setX(xPos - (xSize / 2));
	points[3].setY(yPos + (ySize / 2));

	this->mid.setX(xPos);
	this->mid.setY(yPos);
}
void Box::copy(Box &box) {
	for (int i = 0; i < 4; i++) {
		points[i].set(box.points[i].getX(), box.points[i].getY());
	}
	this->xSize = box.xSize;
	this->xSize = box.ySize;
	mid.set(box.mid.getX(), box.mid.getY());
}
bool Box::axisAlinedTest(Box &other) {
	if ((this->getXmin() ) < other.getXmax() &&
		(this->getXmax()) > other.getXmin() &&
		(this->getYmin() ) < other.getYmax() &&
		(this->getYmax() ) > other.getYmin())
	{
		return true;
	}
	return false;
}
bool Box::axisAlinedTest(Box &other, float xShift, float yShift) {
	if ((this->getXmin() + xShift) < other.getXmax() &&
		(this->getXmax() + xShift) > other.getXmin() &&
		(this->getYmin() + yShift) < other.getYmax() &&
		(this->getYmax() + yShift) > other.getYmin())
	{
		return true;
	}
	return false;
}
bool Box::SepAxisText(Box &other, float xOffset,float yOffset) { // Returns true if we couldn't find a seperating axis
	Point axis;
	
	double length;
	float projection[4];
	Point* points1;
	if (xOffset != 0 && yOffset != 0) {
		
	}
	else {
		points1 = points;
	}
	//Axis 1 - this box
	axis = points[1] - points[0]; // Vector between two points
	length = axis.Normalise(); //Convert to unit vector and get length
	for (int i = 0; i < 4;i++) {
		projection[i] = (other.points[i].getX() - points[0].getX())*axis.getX() + (other.points[i].getY() - points[0].getY())*axis.getY();
	}
	if (canSeperate(projection, length)) {
		return false;
	}

	//Axis 2 - this box
	axis = points[2] - points[1];
	length = axis.Normalise(); //Convert to unit vector and get length
	for (int i = 0; i < 4; i++) {
		projection[i] = (other.points[i].getX() - points[1].getX())*axis.getX() + (other.points[i].getY() - points[1].getY())*axis.getY();
	}
	if (canSeperate(projection, length)) {
		return false;
	}

	//Axis 3 - other box
	axis = other.points[1] - other.points[0];
	length = axis.Normalise();
	for (int i = 0; i < 4; i++) {
		projection[i] = (points[0].getX()- other.points[i].getX())*axis.getX() + (points[0].getY()- other.points[i].getY())*axis.getY();
	}
	if (canSeperate(projection, length)) {
		return false;
	}
	//Axis 4 - other box
	axis = other.points[2] - other.points[1];
	length = axis.Normalise();
	for (int i = 0; i < 4; i++) {
		projection[i] = (points[1].getX() - other.points[i].getX())*axis.getX() + (points[1].getY() - other.points[i].getY())*axis.getY();
	}
	if (canSeperate(projection, length)) {
		return false;
	}
	return true;//Couldn't find any seperating axis
}
bool Box::canSeperate(float projection[4], float length) {

	float min = projection[0];
	float max = projection[0];
	for (int i = 1; i <= 3; ++i) {  // Find minimum and maximum projections - the other two will lie in between and are of no consequence
		if (projection[i] < min) 
		{
			min = projection[i]; 
		}
		else if (projection[i] > max) {
			max = projection[i];
		}

	}
	// comparison noting that the first point of first poly is at distance 0 and the second point at distance len
	// along the edge of the first poly acting as the axis
	if (min > length || max < 0.0) return true; // found separating axis
	else return false;
}
void Box::resize(float xChange, float yChange) {

	float spawnXpos = mid.getX();
	float spawnYpos = mid.getY();
	xSize = xChange;
	ySize = yChange;
	points[0].setX(spawnXpos - (xSize / 2));
	points[0].setY(spawnYpos - (ySize / 2));

	points[1].setX(spawnXpos + (xSize / 2));
	points[1].setY(spawnYpos - (ySize / 2));

	points[2].setX(spawnXpos + (xSize / 2));
	points[2].setY(spawnYpos + (ySize / 2));

	points[3].setX(spawnXpos - (xSize / 2));
	points[3].setY(spawnYpos + (ySize / 2));
}

void Box::resolveColision(Box & other, Point & movement, bool &headHit)
{
	float xShift = movement.getX();
	float yShift = movement.getY();
	if (axisAlinedTest(other, xShift, 0)) {
		if (axisAlinedTest(other, xShift / 2, 0)) {
			xShift = 0;
		}
		else {
			xShift /= 2;
		}
	}
	else if (axisAlinedTest(other, 0, yShift)) {
		if (axisAlinedTest(other, 0, yShift / 2)) {
			yShift = 0;
			headHit = true;
		}
		else {
			yShift /= 2;
		}

	}
	movement.set(xShift, yShift);
}

void Box::resolveColisionAI(Box & other, Point & movement, bool &headHit)
{
	float xShift = movement.getX();
	float yShift = movement.getY();
	if (axisAlinedTest(other, xShift, 0)) {
		if (axisAlinedTest(other, xShift / 2, 0)) {
			xShift *= -10;
		}
		else {
			xShift /= 2;
		}
	}
	else if (axisAlinedTest(other, 0, yShift)) {
		if (axisAlinedTest(other, 0, yShift / 2)) {
			yShift = 0;
			headHit = true;
		}
		else {
			yShift /= 2;
		}

	}
	movement.set(xShift, yShift);
}
bool Box::checkLineIntersection(Point a, Point b,Point &intersection)
{
	for (int i = 0; i < 4; i++) {
		Point c = points[i];
		Point d;
		if (i == 3) {
			d = points[0];
		}
		else {
			d = points[i + 1];
		}
		float i_x;
		float i_y;
		float p1_x = b.getX();
		float p0_x = a.getX();
		float p1_y = b.getY();
		float p0_y = a.getY();
		float p3_x = d.getX();
		float p2_x = c.getX();
		float p3_y = d.getY();
		float p2_y = c.getY();
		float s1_x, s1_y, s2_x, s2_y;
		s1_x = p1_x - p0_x;
		s1_y = p1_y - p0_y;
		s2_x = p3_x - p2_x;
		s2_y = p3_y - p2_y;

		float s, t;
		s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
		t = (s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

		if (s >= 0 && s <= 1 && t >= 0 && t <= 1) {
			//if (i_x != NULL)
			intersection.setX( p0_x + (t * s1_x));
			//if (i_y != NULL)
			intersection.setY(p0_y + (t * s1_y));
			return true;
		}
	}
			return false;
}
