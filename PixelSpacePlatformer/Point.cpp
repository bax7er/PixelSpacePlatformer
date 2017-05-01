#pragma once
#include "Point.h"
#include <iostream>

Point::Point(double x, double y) {
	this->x = x;
	this->y = y;
}
inline double Point::getX() {
	return this->x;
}
inline double Point::getY()
{
	return this->y;
}
double Point::Normalise() {
	double length = sqrt(x * x + y * y);
	this->x = x / length;
	this->y = y / length;
	return length;
}
double DotProduct(Point a, Point b) {
	return (a.getX() * b.getX()) + (a.getY() * b.getY());
}
double DotProduct(double x,double y, Point b) {
	return (x * b.getX()) + (y * b.getY());
}

void Point::rotate(Point center, double angle) {
	if (angle != 0) {
		double sinTheta = sin(-angle);
		double cosTheta = cos(-angle);
		double tempx = this->x;
		double tempy = this->y;
		tempx -= center.getX();
		tempy -= center.getY();
		this->x=((tempx*cosTheta) - (tempy*sinTheta))+center.getX();
		this->y=((tempy*cosTheta) + (tempx*sinTheta)) + center.getY();
	}
}
