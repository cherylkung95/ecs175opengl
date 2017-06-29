// Glutdualwindow.c
// By Eric Stringer 2002
// Simple examples of OpenGL and Glut usage.
// Keyboard input
// 'v' = view ortho/perspective
// 'l' = lighting on/off


//#include <windows.h>  // This header file will be needed for some windows compilers
//#include <GL/gl.h>   // gl.h and glu.h also maybe needed for some compilers
//#include <GL/glu.h>
#include <stdlib.h>  // standard library (set of standard C functions
#include <GL/glut.h>  // glut (gl utility toolkit) basic windows functions, keyboard, mouse.
#include <stdio.h>    // standard (I/O library)
#include <math.h>     // Math library (Higher math functions )
#include <iostream>
#include <string.h>
#include <fstream>
#include "p.h"
using namespace std;

int numPols;
void getTran3();
void getRot3();
void getScale3();
void write();
void rot3D(double x1, double y1, double z1, double x2, double y2, double z2, double angle);
Polygon **pols;
PolyTri **polyt;
double ka, kd, ks, Ia, Il, n, K;

NormV *xL, *fL, *rL;

unsigned int window3[480][360];


int window_1, window_2, window_3;

static int view_state = 0, light_state = 0;

int spin;

int shape;

// I use this to put text on the screen
void Sprint(int x, int y, char *st)
{
	int l, i;

	l = strlen(st); // see how many characters are in text string.
	glRasterPos2i(x, y); // location to start printing text
	for (i = 0; i < l; i++)  // loop until i is greater then l
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, st[i]); // Print a character on the screen
	}

}


// This creates the spinning of the cube.
static void TimeEvent(int te)
{

	spin++;  // increase cube rotation by 1
	if (spin > 360) spin = 0; // if over 360 degress, start back at zero.

	glutSetWindow(window_1);
	glutPostRedisplay();  // Update screen with new rotation data

	glutSetWindow(window_2);
	glutPostRedisplay();  // Update screen with new rotation data

	glutSetWindow(window_3);
	glutPostRedisplay();

	glutSetWindow(window_1);
	glutPostRedisplay();

	glutTimerFunc(100, TimeEvent, 1);  // Reset our timmer.
}


// Setup our Opengl world, called once at startup.
void init(void)
{

	//stuff I don't know what to do with but leave in just in case it will break my program
	glClearColor(0.0, 0.0, 0.0, 0.0);  // When screen cleared, use black.
	glShadeModel(GL_SMOOTH);  // How the object color will be rendered smooth or flat
	glEnable(GL_DEPTH_TEST);   // Check depth when rendering
	// Lighting is added to scene

	glEnable(GL_LIGHTING);  // Turn on lighting
	glEnable(GL_LIGHT1);    // Turn on light 1


}


// Draw our world
void display_1(void)
{

	//not sure what this code does but if I don't include it, nothing displays
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //Clear the screen

	glMatrixMode(GL_PROJECTION);  // Tell opengl that we are doing project matrix work
	glLoadIdentity();  // Clear the matrix
	glOrtho(-8.0, 8.0, -8.0, 8.0, 0.0, 30.0);  // Setup an Ortho view
	glMatrixMode(GL_MODELVIEW);  // Tell opengl that we are doing model matrix work. (drawing)
	glLoadIdentity(); // Clear the model matrix

	

	// Setup view, and print view state on screen

	glColor3f(0.0, 0.0, 1.0);  // Cube color

	// Lighting on/off
	if (light_state == 1)
	{
		glDisable(GL_LIGHTING);  // Turn off lighting
		glDisable(GL_COLOR_MATERIAL); // Turn off material, which needs lighting to work
	}
	else
	{
		glEnable(GL_LIGHTING); // Turn on lighting
		glEnable(GL_COLOR_MATERIAL); // Turn on material settings
		glColorMaterial(GL_FRONT, GL_AMBIENT);
		glColor4f(0.65, 0.65, 0.65, 0.4);
		glColorMaterial(GL_FRONT, GL_EMISSION);
		glColor4f(0.10, 0.10, 0.10, 0.0);
		glColorMaterial(GL_FRONT, GL_SPECULAR);
		glColor4f(0.5, 0.5, 0.5, 0.4);
		glColorMaterial(GL_FRONT, GL_DIFFUSE);
		glColor4f(0.85, 0.85, 0.85, 0.4);
	}
	
	gluLookAt(0, 0, 20, 0, 0, 0, 0, 1, 0);
	
	//end of unknown code block that lets me input things into the display

	glDrawPixels(480, 360, GL_RED, GL_UNSIGNED_BYTE, window3);

	/*
	for (int i = 0; i < numPols; i++)
	{
		for (int j = 0; j < pols[i]->numEdge;j++)
		{
			double x = pols[i]->lines[j]->first->x, y = pols[i]->lines[j]->first->y, xl = pols[i]->lines[j]->last->x, yl = pols[i]->lines[j]->last->y;
			glLineWidth(1);
			glColor3f(1.0, 0.0, 0.0);
			glBegin(GL_LINES);
			//glVertex3f(pols[i]->lines[i]->first->x, pols[i]->lines[i]->first->y , 0.0);
			//glVertex3f(pols[i]->lines[i]->last->x, pols[i]->lines[i]->last->y, 0);
			glVertex2f(x, y);
			glVertex2f(xl, yl);
			glEnd();
		}
		
	}*/
	//from p2

	//axises
	glLineWidth(1);
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
	glVertex2f(0, -10);
	glVertex2f(0, 10);
	glEnd();
	glLineWidth(1);
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
	glVertex2f(-10, 0);
	glVertex2f(10, 0);
	glEnd();

	glutSwapBuffers();
}

void display_2(void)
{
	
	//not sure what this code does but if I don't include it, nothing displays
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //Clear the screen

	glMatrixMode(GL_PROJECTION);  // Tell opengl that we are doing project matrix work
	glLoadIdentity();  // Clear the matrix
	glOrtho(-8.0, 8.0, -8.0, 8.0, 0.0, 30.0);  // Setup an Ortho view
	glMatrixMode(GL_MODELVIEW);  // Tell opengl that we are doing model matrix work. (drawing)
	glLoadIdentity(); // Clear the model matrix

	

	// Setup view, and print view state on screen
	
	glColor3f(0.0, 0.0, 1.0);  // Cube color

	// Lighting on/off
	if (light_state == 1)
	{
		glDisable(GL_LIGHTING);  // Turn off lighting
		glDisable(GL_COLOR_MATERIAL); // Turn off material, which needs lighting to work
	}
	else
	{
		glEnable(GL_LIGHTING); // Turn on lighting
		glEnable(GL_COLOR_MATERIAL); // Turn on material settings
		glColorMaterial(GL_FRONT, GL_AMBIENT);
		glColor4f(0.65, 0.65, 0.65, 0.4);
		glColorMaterial(GL_FRONT, GL_EMISSION);
		glColor4f(0.10, 0.10, 0.10, 0.0);
		glColorMaterial(GL_FRONT, GL_SPECULAR);
		glColor4f(0.5, 0.5, 0.5, 0.4);
		glColorMaterial(GL_FRONT, GL_DIFFUSE);
		glColor4f(0.85, 0.85, 0.85, 0.4);
	}

	gluLookAt(0, 0, 20, 0, 0, 0, 0, 1, 0);

	//end of unknown code block that lets me input things into the display

	/*
	for (int i = 0; i < numPols; i++)
	{
		for (int j = 0; j < pols[i]->numEdge; j++)
		{

			double x = pols[i]->lines[j]->first->x, z = pols[i]->lines[j]->first->z, xl = pols[i]->lines[j]->last->x, zl = pols[i]->lines[j]->last->z;
			glLineWidth(1);
			glColor3f(1.0, 0.0, 0.0);
			glBegin(GL_LINES);
			//glVertex3f(pols[i]->lines[i]->first->x, pols[i]->lines[i]->first->y , 0.0);
			//glVertex3f(pols[i]->lines[i]->last->x, pols[i]->lines[i]->last->y, 0);
			glVertex2f(x, z);
			glVertex2f(xl, zl);
			glEnd();


		}

	}*/
	//from p2

	//displays axis
	glLineWidth(1);
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
	glVertex2f(0, -10);
	glVertex2f(0, 10);
	glEnd();
	glLineWidth(1);
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
	glVertex2f(-10, 0);
	glVertex2f(10, 0);
	glEnd();
	glutSwapBuffers();
}

void display_3(void)
{
	//not sure what this code does but if I don't include it, nothing displays
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //Clear the screen

	glMatrixMode(GL_PROJECTION);  // Tell opengl that we are doing project matrix work
	glLoadIdentity();  // Clear the matrix
	glOrtho(-8.0, 8.0, -8.0, 8.0, 0.0, 30.0);  // Setup an Ortho view
	glMatrixMode(GL_MODELVIEW);  // Tell opengl that we are doing model matrix work. (drawing)
	glLoadIdentity(); // Clear the model matrix

	// Setup view, and print view state on screen
	
	glColor3f(0.0, 0.0, 1.0);  // Cube color

	// Lighting on/off
	if (light_state == 1)
	{
	glDisable(GL_LIGHTING);  // Turn off lighting
	glDisable(GL_COLOR_MATERIAL); // Turn off material, which needs lighting to work
	}
	else
	{
	glEnable(GL_LIGHTING); // Turn on lighting
	glEnable(GL_COLOR_MATERIAL); // Turn on material settings
	glColorMaterial(GL_FRONT, GL_AMBIENT);
	glColor4f(0.65, 0.65, 0.65, 0.4);
	glColorMaterial(GL_FRONT, GL_EMISSION);
	glColor4f(0.10, 0.10, 0.10, 0.0);
	glColorMaterial(GL_FRONT, GL_SPECULAR);
	glColor4f(0.5, 0.5, 0.5, 0.4);
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glColor4f(0.85, 0.85, 0.85, 0.4);
	}

	gluLookAt(0, 0, 20, 0, 0, 0, 0, 1, 0);
	//end of unknown code block that lets me input things into the display


	
	/*
	for (int i = 0; i < numPols; i++)
	{
		for (int j = 0; j < pols[i]->numEdge; j++)
		{

			double y = pols[i]->lines[j]->first->y, z = pols[i]->lines[j]->first->z, yl = pols[i]->lines[j]->last->y, zl = pols[i]->lines[j]->last->z;
			glLineWidth(1);
			glColor3f(1.0, 0.0, 0.0);
			glBegin(GL_LINES);
			//glVertex3f(pols[i]->lines[i]->first->x, pols[i]->lines[i]->first->y , 0.0);
			//glVertex3f(pols[i]->lines[i]->last->x, pols[i]->lines[i]->last->y, 0);
			glVertex2f(z, y);
			glVertex2f(zl, yl);
			glEnd();
			

		}


	}*/
	//from p2

	glLineWidth(1);
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
	glVertex2f(0, -10);
	glVertex2f(0, 10);
	glEnd();
	glLineWidth(1);
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
	glVertex2f(-10, 0);
	glVertex2f(10, 0);
	glEnd();
}


// This is called when the window has been resized.
void reshape_1(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

// This is called when the window has been resized.
void reshape_2(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

// Read the keyboard
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{

	case 'v':
	case 'V':
		view_state = abs(view_state - 1);
		break;
	case 'l':
	case 'L':
		light_state = abs(light_state - 1);
		break;
	case 's':
	case 'S':
		getScale3();
		break;
	case 27:
		exit(0); // exit program when [ESC] key presseed
		break;
	case 't':
	case 'T':
		getTran3();
		break;
	case 'r':
	case 'R':
		getRot3();
		break;
	case 'w':
	case 'W':
		write();
	default:
		break;
	}

	if (shape > 8) shape = 0;

}

void getScale3()
{
	char ans;
	int pNum;
	double degree;
	cout << "Scale? (y / n): ";
	cin >> ans;

	if (ans == 'y')
	{
		cout << "Which polygon?(number): ";
		cin >> pNum;
		cout << "Degree(number)?: ";
		cin >> degree;
		pols[pNum-1]->scale3D(degree);
	}
}
void Polygon::trans3D(double xshift, double yshift, double zshift)
{
	for (int i = 0; i < numPts; i++)
	{
		points[i]->x += xshift;
		points[i]->y += yshift;
		points[i]->z += zshift;
	}

	for (int j = 0; j < numEdge; j++)
	{
		lines[j]->first = points[lines[j]->firstNum];
		lines[j]->last = points[lines[j]->lastNum];
	}
}
void getTran3()
{
	char ans;
	int pNum;
	double xshift, yshift, zshift;
	cout << "Translate? y/n: ";
	cin >> ans;

	if (ans == 'y')
	{
		cout << "Which polygon?(number): ";
		cin >> pNum;
		cout << "Translation vector: " << endl;
		cout << "x: ";
		cin >> xshift;
		cout << "y: ";
		cin >> yshift;
		cout << "z: ";
		cin >> zshift;
		pols[pNum-1]->trans3D(xshift, yshift, zshift);
	}
}

Pixel *getCenter3(const Polygon *pl)
{
	int xAvg = 0, yAvg = 0, zAvg = 0;
	for (int i = 0; i < pl->numPts; i++)
	{
		xAvg += pl->points[i]->x;
		yAvg += pl->points[i]->y;
		zAvg += pl->points[i]->z;
	}

	xAvg = xAvg / pl->numPts;
	yAvg = yAvg / pl->numPts;
	zAvg = zAvg / pl->numPts;

	//cout << xAvg << " " << yAvg << endl;
	Pixel *pix = new Pixel(xAvg, yAvg, zAvg);
	return pix;
}
void Polygon::scale3D(double degree)
{
	Pixel *pix = getCenter3(this);
	this->trans3D(-(pix->x), -(pix->y), -(pix->z));

	for (int i = 0; i < numPts; i++)
	{
		points[i]->x *= degree;
		points[i]->y *= degree;
		points[i]->z *= degree;
	}

	this->trans3D((pix->x), (pix->y), (pix->z));
}
void Polygon::rot3D(double x1, double y1, double z1, double x2, double y2, double z2, double angle)
{
	double u = x1 - x2;
	double v = y1 - y2;
	double w = z1 - z2;

	double L = (u*u) + (v*v) + (w*w);

	for (int i = 0; i < numPts; i++)
	{
		double x = points[i]->x, y =points[i]->y, z=points[i]->z;
		x = ((points[i]->x)*((u*u) + (((v*v) + (w*w))*cos(angle))) / L) + ((points[i]->y) *((u*v *(1-cos(angle))) - (w*sqrt(L) *sin(angle)))/L) + ((points[i]->z)*((u*w*(1-cos(angle)))+(v*sqrt(L)*sin(angle)))/L) + ((((x1*((v*v)+(w*w))-(u*((y1*v)+(z1*w))))*(1-cos(angle)))+((y1*w)-(z1*v))*sqrt(L)*sin(angle))/L);
		y = ((points[i]->x)*((((u*v)*(1 - cos(angle))) + (w*sqrt(L)*sin(angle))) / L)) + ((points[i]->y) * (((v*v) + (((u*u) + (w*w))*cos(angle)))) / L) + ((points[i]->z)*(((v*w)*(1 - cos(angle))) - (u*sqrt(L)*sin(angle))) / L) + ((((y1*((u*u) + (w*w)) - (v*((x1*u) + (z1*w))))*(1 - cos(angle))) + ((z1*u) - (x1*w))*sqrt(L)*sin(angle)) / L);
		z = ((points[i]->x)*(((u*w)*(1 - cos(angle))) - (v*sqrt(L)*sin(angle))) / L) + ((points[i]->y)* (((v*w)*(1 - cos(angle))) + ((u*sqrt(L))*sin(angle))) / L) + ((points[i]->z)*((w*w) + (((u*u) + (v*v))*cos(angle))) / L) + ((((z1*((u*u) + (v*v)) - (w*((x1*u) + (y1*v))))*(1 - cos(angle))) + ((x1*v) - (y1*u))*sqrt(L)*sin(angle)) / L);

		points[i]->x = x;
		points[i]->y = y;
		points[i]->z = z;

		//points[i]->print();
	}

	for (int j = 0; j < numEdge; j++)
	{
		lines[j]->first = points[lines[j]->firstNum];

		lines[j]->last = points[lines[j]->lastNum];
	}
}
void getRot3()
{
	char ans;
	int pNum;
	double x1, y1, z1, x2, y2, z2,angle;
	cout << "Rotate? y/n: ";
	cin >> ans;

	if (ans == 'y')
	{
		cout << "Which polygon?(number): ";
		cin >> pNum;
		cout << "Rotation point 1: " << endl;
		cout << "x: ";
		cin >> x1;
		cout << "y: ";
		cin >> y1;
		cout << "z: ";
		cin >> z1;
		cout << "Rotation point 2: " << endl;
		cout << "x: ";
		cin >> x2;
		cout << "y: ";
		cin >> y2;
		cout << "z: ";
		cin >> z2;
		cout << "Rotation angle: " << endl;
		cin >> angle;
		pols[pNum - 1]->rot3D(x1, y1, z1, x2, y2, z2, angle);
	}
}
void write()
{
	ofstream file("results.txt");

	cout << "writing to results.txt" << endl;
	
	file << numPols << endl;
	for (int i = 0; i < numPols; i++)
	{
		file << pols[i]->numPts << endl;
		for (int j = 0; j < pols[i]->numPts; j++)
		{
			file << pols[i]->points[j]->x << " " << pols[i]->points[j]->y << " " << pols[i]->points[j]->z << endl;
		}
		
		file << pols[i]->numEdge << endl;
		for (int k = 0; k < pols[i]->numEdge; k++)
		{
			file << pols[i]->lines[k]->firstNum+1 << " " << pols[i]->lines[k]->lastNum+1 << endl;
		}

	}

}
void read()
{
	ifstream file("data.txt");
	Pixel *p;

	int numPoly;
	file >> numPoly;
	numPols = numPoly;
	pols = new Polygon*[numPoly];
	for (int i = 0; i < numPoly; i++)
	{
		int numPts;
		file >> numPts;
		
		pols[i] = new Polygon();
		pols[i]->numPts = numPts;
		pols[i]->polyNum = i;
		pols[i]->points = new Pixel*[numPts];;
		for (int j = 0; j < numPts; j++)
		{
			double x,y,z;
			file >> x >>y>>z;
			Pixel* pt = new Pixel(x, y, z);
			pt->ptNum = j;
			pols[i]->points[j] = pt;
			//pols[i]->points[j]->print();
		}
		
		int numEdge;
		file >> numEdge;
		pols[i]->numEdge=numEdge;
		pols[i]->lines = new Line*[numEdge];
		for (int k = 0; k < numEdge; k++)
		{
			int fst, lst;
			file >> fst >> lst;
			pols[i]->lines[k] = new Line(pols[i]->points[fst-1], pols[i]->points[lst-1]);
			pols[i]->lines[k]->firstNum = fst-1;
			pols[i]->lines[k]->lastNum = lst-1;
			//pols[i]->lines[k]->first->print();
			//pols[i]->lines[k]->last->print();
		}
		
	}
}

NormV normalize(NormV one, Pixel two)
{
	double x, y, z, diff;
	x = one.x - two.x;
	y = one.y - two.y;
	z = one.z - two.z;

	diff = sqrt((x*x) + (y*y) + (z*z));

	x = x / diff;
	y = y / diff;
	z = z / diff;

	NormV temp = NormV(x, y, z);

	return temp;

}

double distP(NormV one, Pixel two)
{
	double x, y, z, dist;
	x = one.x - two.x;
	y = one.y - two.y;
	z = one.z - two.z;

	dist = sqrt((x*x) + (y*y) + (z*z));

	return dist;
}

double dotP(NormV one, NormV two)
{
	double x, y, z;
	x = one.x * two.x;
	y = one.y * two.y; 
	z = one.z * two.z;

	double dot = x + y + z;
	return dot;
}
void Pixel::calcI()
{
	NormV l = normalize(*xL, *this);
	NormV v = normalize(*fL, *this);

	double dist = distP(*fL, *this);

	double ln = dotP(l, *norm);
	double rv = dotP(*rL, v);

	intensity = ka*Ia;
	intensity += (Il / (dist + K)) *((kd*ln) + (kd*pow(rv, n)));

	cout << intensity << endl;
}
void read3()
{
	ifstream file("test.txt");
	
	int numPoly;
	file >> numPoly;
	numPols = numPoly;
	polyt = new PolyTri*[numPoly];
	for (int i = 0; i < numPoly; i++)
	{
		int numTri;
		file >> numTri;
		polyt[i] = new PolyTri();
		polyt[i]->numTri = numTri;
		polyt[i]->tri = new Polygon*[numTri];
		for (int t = 0; t < numTri; t++)
		{
			int numPts;
			file >> numPts;

			polyt[i]->tri[t] = new Polygon();
			polyt[i]->tri[t]->numPts = numPts;
			polyt[i]->tri[t]->polyNum = i;
			polyt[i]->tri[t]->points = new Pixel*[numPts];
			for (int j = 0; j < numPts; j++)
			{
				double x, y, z;
				file >> x >> y >> z;
				Pixel* pt = new Pixel(x*10, y*10, z*10);
				pt->ptNum = j;
				polyt[i]->tri[t]->points[j] = pt;
				//pols[i]->points[j]->print();

				double xN, yN, zN;
				file >> xN;
				file >> yN;
				file >> zN;
				polyt[i]->tri[t]->points[j]->norm = new NormV(xN, yN, zN);
			}//for inputting points

			int numEdge;
			file >> numEdge;
			polyt[i]->tri[t]->numEdge = numEdge;
			polyt[i]->tri[t]->lines = new Line*[numEdge];
			for (int k = 0; k < numEdge; k++)
			{
				int fst, lst;
				file >> fst >> lst;
				polyt[i]->tri[t]->lines[k] = new Line(polyt[i]->tri[t]->points[fst - 1], polyt[i]->tri[t]->points[lst - 1]);
				polyt[i]->tri[t]->lines[k]->firstNum = fst - 1;
				polyt[i]->tri[t]->lines[k]->lastNum = lst - 1;
				//pols[i]->lines[k]->first->print();
				//pols[i]->lines[k]->last->print();
			}//for linking points

			

		}//for each triangles
		
	}//for number of polygons

	//read constants
	file >> ka >> kd >> ks >> Ia >> Il >> n >> K;

	double tempx, tempy, tempz;
	file >> tempx >> tempy >> tempz;
	xL = new NormV(tempx, tempy, tempz);

	file >> tempx >> tempy >> tempz;
	fL = new NormV(tempx, tempy, tempz);

	file >> tempx >> tempy >> tempz;
	rL = new NormV(tempx, tempy, tempz);

}

void calcIPoly()
{
	for (int i = 0; i < numPols; i++)
	{
		for (int j = 0; j < polyt[i]->numTri; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				polyt[i]->tri[j]->points[k]->calcI();
			}
		}
	}
}

void makeLine()
{
	for (int i = 0; i < numPols; i++)
	{
		for (int j = 0; j < polyt[i]->numTri; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				polyt[i]->tri[j]->lines[k]->drawLine();
			}
		}
	}
}
// Main program

int main(int argc, char** argv)
{
	read3();
	calcIPoly();
	makeLine();
	//getTran3();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE);
	glutInitWindowSize(480, 360);
	glutInitWindowPosition(100, 100);
	glutTimerFunc(10, TimeEvent, 1);
	window_1 = glutCreateWindow(argv[0]);
	glutSetWindowTitle("xy");
	init();
	glutDisplayFunc(display_1);
	glutReshapeFunc(reshape_1);
	glutKeyboardFunc(keyboard);

	window_2 = glutCreateWindow(argv[0]);
	glutSetWindowTitle("xz");
	init();
	glutDisplayFunc(display_2);
	glutReshapeFunc(reshape_2);

	window_3 = glutCreateWindow(argv[0]);
	glutSetWindowTitle("zy");
	init();
	glutDisplayFunc(display_3);
	glutReshapeFunc(reshape_2);
	glutMainLoop();
	return 0;
}