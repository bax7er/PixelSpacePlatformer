#pragma once
#ifndef POINT_H
#define POINT_H

class Point {
private:
	double x;
	double y;
public:
	//Accessor methods
	inline double getX();
	inline double getY();
	void set(double x, double y);
	inline void setX(double x);
	inline void setY(double y);
	inline void move(double x, double y);
	inline void moveX(double x);
	inline void moveY(double y);
	Point(double x=0, double y=0);
	double Normalise();
	inline Point& operator-(const Point& b);
	inline Point& operator+(const Point& b);
	inline Point& operator*(const int& b);
	inline Point& operator*(const double& b);
	void rotate(Point center, double angle);
};
double DotProduct(Point a, Point b);
double DotProduct(double x,double y, Point b);
inline void Point::set(double x, double y) {
	this->x = x;
	this->y = y;
}

inline void Point::setX(double x) {
	this->x = x;
}
inline void Point::setY(double y) {
	this->y = y;
}
inline void Point::move(double x, double y) {
	this->x += x;
	this->y += y;
}
inline void Point::moveX(double x) {
	this->x += x;
}
inline void Point::moveY(double y) {
	this->y += y;
}
inline Point& Point::operator-(const Point& b) {
	return Point((this->x - b.x), (this->y - b.y));
}
inline Point& Point::operator+(const Point& b) {
	return Point((this->x + b.x), (this->y + b.y));
}
inline Point& Point::operator*(const int& b) {
	return Point((this->x *b), (this->y * b));
}
inline Point& Point::operator*(const double& b) {
	return Point((this->x *b), (this->y * b));
}
#endif