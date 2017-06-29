#ifndef P4_H
#define P4_H

#include <stdlib.h>
#include <iostream>
#include <GL/glut.h>
#include <math.h>

class wcPt3D
{
public:
	GLfloat x, y, z;
	wcPt3D operator* (const double rhs){
		wcPt3D lhs;
		lhs.x = this->x * rhs;
		lhs.y = this->y * rhs;
		lhs.z = this->z * rhs;
		return lhs;
	}
	wcPt3D operator+(const wcPt3D& rhs)
	{
		wcPt3D lhs;
		lhs.x = this->x + rhs.x;
		lhs.y = this->y + rhs.y;
		lhs.z = this->z + rhs.z;
		return lhs;
	}
};

class Bezier
{
public:
	GLint nCtrlPts;
	wcPt3D* ctrlPts;

	Bezier();
	~Bezier();


};

class DeBoor
{
public:
	GLint nCtrlPts;
	wcPt3D* ctrlPts;
	
	GLint nKnots;
	GLdouble* knots;

	GLint k;

	DeBoor(){};
	~DeBoor(){};
};
#endif