#ifndef P_H
#define P_H

#include <iostream>
using namespace std;

class Pixel
{
public:
	double x;
	double y;
	double z;
	Pixel* next;


	Pixel(double x1, double y1, double z1);
	Pixel(double x1, double y1);
	Pixel();
	~Pixel();
	void print() {
		cout << x << " " << y << " " << z << endl;
	};
	int ptNum;
};

class Line
{
public:
	Line();
	Line(Pixel* first, Pixel* last);
	~Line();
	Pixel* itr;
	Pixel* first;
	Pixel* last;
	void toFirst() { itr = first; };
	int num;
	Line* next;
	int firstNum;
	int lastNum;
};

class Polygon
{
public:
	Polygon();
	~Polygon();
	int numPts;
	int numEdge;
	Line** lines;
	Pixel** points;
	int polyNum;

	void trans3D(double xshift, double yshift, double zshift);
	void rot3D(double x1, double y1, double z1, double x2, double y2, double z2, double angle);
	void scale3D(double degree);
};

#endif