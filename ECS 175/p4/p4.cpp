#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <GL/glut.h>
#include <math.h>
#include "p4.h"
using namespace std;

GLsizei winWidth = 600, winHeight = 600;

GLfloat xwcMin = -50.0, xwcMax=50.0;
GLfloat ywcMin = -50.0, ywcMax = 50.0;

int window_1;
int numBez;
int numdeB;
int curvePts;
Bezier **bez;
DeBoor **deB;

Bezier::Bezier()
{

}
Bezier::~Bezier()
{

}
void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);

}

void plotLine(wcPt3D bezprevPt, wcPt3D bezCurvePt)
{
	glBegin(GL_LINES);
		glVertex2f(bezprevPt.x, bezprevPt.y);
		glVertex2f(bezCurvePt.x, bezCurvePt.y);
	glEnd();
}

void binomialCoeffs(GLint n, GLint *C)
{
	GLint k, j; 

	for (k = 0; k <= n; k++)
	{
		C[k] = 1;
		for (j = n; j >= k + 1; j--)
			C[k] *= j;
		for (j = n - k; j >= 2; j--)
			C[k] /= j;
	}
}

void computeBezPt(GLfloat u, wcPt3D * bezPt, GLint nCtrlPts, wcPt3D * ctrlPts, GLint * C)
{
	GLint k, n = nCtrlPts - 1;
	GLfloat bezBlendFcn;

	bezPt->x = bezPt->y = bezPt->z = 0.0;

	for (k = 0; k < nCtrlPts; k++)
	{
		bezBlendFcn = C[k] * pow(u, k) * pow(1 - u, n - k);
		bezPt->x += ctrlPts[k].x * bezBlendFcn;
		bezPt->y += ctrlPts[k].y * bezBlendFcn;
		bezPt->z += ctrlPts[k].z * bezBlendFcn;
	}

}

void bezier(wcPt3D * ctrlPts, GLint nCtrlPts, GLint nBezCurvePts)
{
	wcPt3D bezCurvePt, bezPrevPt;
	GLfloat u;
	GLint *C, k;

	C = new GLint[nCtrlPts];

	binomialCoeffs(nCtrlPts - 1, C);
	
	bezPrevPt = ctrlPts[0];
	for (k = 1; k <= nBezCurvePts; k++)
	{
		u = GLfloat(k) / GLfloat(nBezCurvePts);
		computeBezPt(u, &bezCurvePt, nCtrlPts, ctrlPts, C);
		plotLine(bezPrevPt,bezCurvePt);
		bezPrevPt = bezCurvePt; 
			
	}
}

GLint findInterval(GLdouble u, GLdouble* knots, GLint nknots)
{
	GLint i;
	for (i = 0; i < nknots; i++)
	{
		if (u < knots[i])
			break;
	}
	return i-1;
}
wcPt3D deBoor(GLint I, GLdouble u, GLint k, GLdouble * knots, wcPt3D * ctrlPts, GLint nCtrlPts)
{
	wcPt3D* d= new wcPt3D[10000];
	GLint r, i;

	for (int t=0; t < nCtrlPts; t++)
	{
		d[t] = ctrlPts[t];
	}

	for (r = 1; r <= k - 1; r++)
	{
		for (i = I - (k - 1); i <= I - r; i++)
		{

			d[r*(k-1)+i] = d[(r-1)*(k-1) + i]*((knots[i + k] - u) / (knots[i + k] - knots[i + r]))
				+ d[(r-1)*(k-1) + i+1]*((u - knots[i + r]) / (knots[i + k] - knots[i + r]));
			//std::cout << "deboor: " << d[r*(k - 1) + i].x << " " << d[r*(k - 1) + i].y << " " << d[r*(k - 1) + i].z << std::endl;
		}
	}

	//wcPt3D dF = d[(k - 1) *(I - (k - 1))];
	wcPt3D dF = d[(r-1)*(k-1) +(i-1)];
	delete[] d;
	//std::cout << "dF" << dF.x << " " << dF.y << " " << dF.z << std::endl;
	return dF;
}
void inputDeBoor(GLint nCtrlPts, wcPt3D* ctrlPts, GLint nK, GLdouble *knots, GLint k, GLint curvePts)
{
	GLint I;
	wcPt3D prev;

	for (GLdouble i = 0; i < curvePts; i++)
	{
		//GLdouble knots[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
		GLdouble u = ((knots[nCtrlPts] - knots[k - 1])* (i / GLdouble(curvePts))) + knots[k - 1];

		I = findInterval(u, knots, nK);


		wcPt3D d = deBoor(I, u, k, knots, ctrlPts, nCtrlPts);
		//std::cout << "u: " << u << " " << d.x << " " << d.y << " " << d.z << " I: " << I << std::endl;

		if (i == 0)
		{
			prev = d;
		}
		else
		{
			plotLine(prev, d);
			prev = d;
		}
	}
}
void read()
{
	ifstream file("data.txt");
	file >> numBez;
	bez = new Bezier*[numBez];

	for (int i = 0; i < numBez; i++)
	{
		bez[i] = new Bezier();
		file >> bez[i]->nCtrlPts;
		bez[i]->ctrlPts = new wcPt3D[bez[i]->nCtrlPts];

		for (int j = 0; j < bez[i]->nCtrlPts; j++)
		{
			file >> bez[i]->ctrlPts[j].x >> bez[i]->ctrlPts[j].y;
			bez[i]->ctrlPts[j].z = 0;
		}//each control point
	}//each Bezier curve

	file >> numdeB;
	deB = new DeBoor*[numdeB];

	for (int i = 0; i < numdeB; i++)
	{
		deB[i] = new DeBoor();
		file >> deB[i]->nCtrlPts;
		deB[i]->ctrlPts = new wcPt3D[deB[i]->nCtrlPts];

		for (int j = 0; j < deB[i]->nCtrlPts; j++)
		{
			file >> deB[i]->ctrlPts[j].x >> deB[i]->ctrlPts[j].y;
			deB[i]->ctrlPts[j].z = 0;
		}//each control point

		file >> deB[i]->nKnots;
		deB[i]->knots = new GLdouble[deB[i]->nKnots];
		
		for (int k = 0; k < deB[i]->nKnots; k++)
		{
			file >> deB[i]->knots[k];
		}//each knot

		file >> deB[i]->k;
		file >> curvePts;

	}//each de Boor curve
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	GLint nCtrlPts = 4;

	//wcPt3D ctrlPts[4] = { { -40.0, -40.0, 0.0 }, { -10.0, 200.0, 0.0 }, { 10.0, -200.0, 0.0 }, { 40.0, 40.0, 0.0 } };
	//wcPt3D ctrlPts[5] = { { -20.0, -10.0, 0.0 }, { -15.0, 10.0, 0.0 }, { 10.0, 15.0, 0.0 }, { 20.0, -10.0, 0.0 }, {0.0, -15.0, 0.0 } };


	glLineWidth(4);
	glColor3f(1.0, 0.0, 0.0);
	
	for (int m = 0; m < numdeB; m++)
	{
		inputDeBoor(deB[m]->nCtrlPts, deB[m]->ctrlPts, deB[m]->nKnots, deB[m]->knots, deB[m]->k, curvePts);
	}
	/*GLint I, k=4, nk=nCtrlPts+k;
	wcPt3D prev;

	for (GLdouble i = 0; i < nBezCurvePts; i++)
	{
		GLdouble knots[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
		GLdouble u = ((knots[nCtrlPts] - knots[k-1])* (i / GLdouble(nBezCurvePts))) + knots[k-1];

		I = findInterval(u, knots, nk);
		

		wcPt3D d = deBoor(I, u, k, knots, ctrlPts,nCtrlPts);
		std::cout << "u: "<< u << " " << d.x << " " << d.y << " " << d.z << " I: " << I << std::endl;

		if (i == 0)
		{
			prev = d;
		}
		else
		{
			plotLine(prev, d);
			prev = d;
		}
	}*/

	glLineWidth(4);
	glColor3f(0.0, 0.0, 1.0);
	for (int k = 0; k < numBez; k++)
	{
		bezier(bez[k]->ctrlPts, bez[k]->nCtrlPts, curvePts);
	}
	//bezier(ctrlPts, nCtrlPts, curvePts);
	glFlush();
	glutSwapBuffers();
}

void winReshapeFcn(GLint newWidth, GLint newHeight)
{
	glViewport(0, 0, newHeight, newHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluOrtho2D(xwcMin, xwcMax, ywcMin, ywcMax);

	glClear(GL_COLOR_BUFFER_BIT);
}

void modBez()
{
	int nBez;
	cout << "Which Bezier Curve to modify? (1-" << numBez << " ";
	cin >> nBez;

	nBez--;
	cout << "number of control points: ";
	cin >> bez[nBez]->nCtrlPts;

	delete[] bez[nBez]->ctrlPts;
	bez[nBez]->ctrlPts = new wcPt3D[bez[nBez]->nCtrlPts];

	cout << "Enter control points: ";
	for (int i = 0; i < bez[nBez]->nCtrlPts; i++)
	{
		cout << "x: ";
		cin >> bez[nBez]->ctrlPts[i].x;
		cout << "y: ";
		cin	>> bez[nBez]->ctrlPts[i].y;
		bez[nBez]->ctrlPts[i].z=0;

	}//each control point
}

void modRes()
{
	cout << "Number of curve points: ";
	cin >> curvePts;
}

void modDeB()
{
	int nDeB;
	cout << "Which B-spline curve to modify? (1-" << numdeB << " ";
	cin >> nDeB;

	nDeB--;
		
	delete deB[nDeB];
		deB[nDeB] = new DeBoor();
		cout << "number of control points: ";
		cin >> deB[nDeB]->nCtrlPts;
		deB[nDeB]->ctrlPts = new wcPt3D[deB[nDeB]->nCtrlPts];
		cout << "Enter control points: " << endl;
		for (int j = 0; j < deB[nDeB]->nCtrlPts; j++)
		{
			cout << "x: ";
			cin >> deB[nDeB]->ctrlPts[j].x;
			cout << "y: ";
			cin >> deB[nDeB]->ctrlPts[j].y;
			deB[nDeB]->ctrlPts[j].z = 0;
		}//each control point

		cout << "number of knots: ";
		cin >> deB[nDeB]->nKnots;
		deB[nDeB]->knots = new GLdouble[deB[nDeB]->nKnots];
		cout << "Enter knots: " << endl;;
		for (int k = 0; k < deB[nDeB]->nKnots; k++)
		{
			cout << "knot " << k + 1 << ": ";
			cin >> deB[nDeB]->knots[k];
		}//each knot

		cout << "Enter order: ";
		cin >> deB[nDeB]->k;

}
static void TimeEvent(int te)
{
	glutSetWindow(window_1);
	glutPostRedisplay();  // Update screen with new rotation data
	glutTimerFunc(100, TimeEvent, 1);  // Reset our timmer.
}
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'r': 
		modRes();
		break;
	case 'b':
		modBez();
		break;
	case 'd':
		modDeB();
		break;
	default:
		break;
	}

}
void main(int argc, char** argv)
{
	curvePts = 100;//remove
	read();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE| GLUT_RGB);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(winWidth, winHeight);
	glutTimerFunc(10, TimeEvent, 1);
	window_1=glutCreateWindow("Bezier and B-spline Curves");

	init();
	glutDisplayFunc(displayFcn);
	glutReshapeFunc(winReshapeFcn);
	glutKeyboardFunc(keyboard);

	glutMainLoop();
}
