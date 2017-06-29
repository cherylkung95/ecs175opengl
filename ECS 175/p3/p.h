#ifndef P_H
#define P_H

#include <iostream>
using namespace std;

class NormV
{
public:
	double x;
	double y;
	double z;

	NormV();
	NormV(double x1, double y1, double z1){
		x = x1;
		y = y1;
		z = z1;
	};
	~NormV(){
	};
};

class Pixel
{
public:
	double x;
	double y;
	double z;
	int polyNum;
	Pixel* next;
	NormV* norm;
	double intensity;


	Pixel(double x1, double y1, double z1);
	Pixel(double x1, double y1);
	Pixel();
	~Pixel();
	void print() {
		cout << x << " " << y << " " << z << endl;
	};
	int ptNum;

	bool operator==(const Pixel& rhs){
		if( x == rhs.x &&
			y ==rhs.y &&
			polyNum==rhs.polyNum
		)
			return true;
		else 
			return false;
	}

	void calcI();
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
	void drawLine();
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
	void callRast();
};

class PolyTri
{
public:
	PolyTri();
	~PolyTri();

	Polygon **tri=NULL;
	int numTri;
	
};

#endif