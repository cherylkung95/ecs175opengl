#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/* Using the GLUT library for the base windowing setup */
#include <GL/glut.h>
#include <iostream>
#include "p.h"
using namespace std;

unsigned int window[480][360];
int windX = 360;
int windY = 480;
Polygon** polys;
int numPl;
int numLine=0;


void drawBren(Line* line);
void translate(double xshift, double yshift, int pNum);
void rotate(double angle, Polygon *poly);
void scale(int degree, Polygon *poly);
void clip();
Pixel* getCenter(Polygon pl);

Pixel::Pixel()
{
	x = 0;
	y = 0;
	z = 0;
	next = NULL;
}
Pixel::Pixel(double x1, double y1)
{
	x = x1;
	y = y1;
	z = 0;
	next = NULL;
}

Pixel::Pixel(double x1, double y1,double z1)
{
	x = x1;
	y = y1;
	z = z1;
	next = NULL;
}
Pixel::~Pixel()
{

}
Line::Line()
{
	num = 2;
}
Line::Line(Pixel* first1, Pixel* last1)
{
	first = first1;
	last = last1;
	first->next = last;
	num = 2;
}
Line::~Line()
{

}
Polygon::Polygon()
{

}
Polygon::~Polygon()
{
	delete[] lines;
	delete[] points;
}
PolyTri::PolyTri()
{

}
PolyTri::~PolyTri()
{

}
void read1()
{
	int numPt;
	cout << "number of polygons: ";
	cin >> numPl;
	polys = new Polygon*[numPl];

	for (int p = 0; p < numPl; p++)
	{
		polys[p] = new Polygon();
		cout << "Polygon "<< p+1 << "\n"<< "number of points: ";
		cin >> numPt;
		Pixel** pix = new Pixel*[numPt];
		polys[p]->lines = new Line*[numPt];

		for (int i = 0; i < numPt; i++)
		{
			int x, y;
			cout << "x: ";
			cin >> x;
			cout << "y: ";
			cin >> y;

			pix[i] = new Pixel(x, y);
		}//gets all the points

		int j;
		for (j = 0; j < numPt - 1; j++)
		{
			Line *line1 = new Line(pix[j], pix[j + 1]);
			polys[p]->lines[j] = line1;
		}
		Line *line1 = new Line(pix[0], pix[j]);
		polys[p]->lines[j] = line1;
		polys[p]->numPts = numPt; //creates lines from points

		polys[p]->points = pix; //sets points
		polys[p]->polyNum = p;
	}//creates each polygon

	
	/*
	Line *line = new Line();
	line->num = numPt;
	Pixel* first = new Pixel();
	line->first = first;
	cout << "x: ";
	cin >> first->x;
	cout << "y: ";
	cin >> first->y;
	for (int i = 0; i < numPl-1; i++)
	{
		Line *line1 = new Line();
		Pixel* last = new Pixel();
		line->last = last;
		cout << "x: ";
		cin >> last->x;
		cout << "y: ";
		cin >> last->y;
		line->first->next = line->last;
		
		
		line->next = line1;
		line1->num = numPt;
		Pixel* first = new Pixel();
		line1->first = first;
		cout << "x: ";
		cin >> first->x;
		cout << "y: ";
		cin >> first->y;
	}

	
	for (int i = 0; i < numPt; i++)
	{
		cout << "x: ";
		cin >> nextP->x;
		cout << "y: ";
		cin >> nextP->y;
		//cout << next->x << " " << next->y << endl;
		if (i != 0)
		{
			nextP->next = new Pixel();
			nextP = nextP->next;
		}
	}*/
}
void getLineAlg(Line* line)
{
	if (numLine == 0)
	{
		cout << "1 for DDA, 2 for Bresenham: ";
		cin >> numLine;
	}
	if (numLine == 1)
	{
		line->drawLine();
	}
	else
	{
		drawBren(line);
	}
	
	
}
void getTranslate()
{
	char ans;
	int pNum;
	double xshift, yshift;
	cout << "Translate? y/n: ";
	cin >> ans;

	if (ans == 'y')
	{
		cout << "Which polygon?(number): ";
		cin >> pNum;
		cout << "x-shift: ";
		cin >> xshift;
		cout << "y-shift: ";
		cin >> yshift;
		translate(xshift, yshift, pNum-1);
	}
}
void getRotate()
{
	char ans;
	int pNum;
	double angle;
	cout << "Rotate? (y / n): ";
	cin >> ans;

	if (ans == 'y')
	{
		cout << "Which polygon?(number): ";
		cin >> pNum;
		cout << "Angle of rotation?: ";
		cin >> angle;
		rotate(angle, polys[pNum-1]);
	}
}
void getScale()
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
		scale(degree, polys[pNum - 1]);
	}
}
void getWindowSize()
{
	char ans;
	cout << "Current Window Dimensions: (" << windX << ", " << windY << ")" << endl;
	cout << "Shrink? (y / n): ";
	cin >> ans;

	if (ans == 'y')
	{
		cout << "Shrink to: " << endl;
		cout << "width (x): ";
		cin >> windX;
		cout << "length (y): ";
		cin >> windY;

		clip();
	}
}
Pixel *getCenter(const Polygon *pl)
{
	int xAvg=0, yAvg=0;
	for (int i = 0; i < pl->numPts; i++)
	{
		xAvg += pl->points[i]->x;
		yAvg += pl->points[i]->y;
	}

	xAvg = xAvg / pl->numPts;
	yAvg = yAvg / pl->numPts;

	//cout << xAvg << " " << yAvg << endl;
	Pixel *pix = new Pixel(xAvg, yAvg);
	return pix;
}
void inputLine(Line* line)
{
	int x = line->first->x;
	int y = line->first->y;
	window[y][x] = 255;

	x = line->last->x;
	y = line->last->y;
	window[y][x] = 255;

	line->toFirst();
	for (int i = 0; i < line->num; i++)
	{
		int x = (line->itr)->x;
		int y = (line->itr)->y;
		window[y][x] = 255;
		line->itr = line->itr->next;
	}
}
void Line::drawLine()
{
	if (last->x < first->x)
	{
		Pixel *temp = last;
		last = first;
		first = temp;
	}
	double difX = (last->x) - (first->x);
	double difY = (last->y) - (first->y);

	//cout << difX << " " << difY << " " << ((difY / difX) >= 1 || (difY / difX) <= -1) << endl;
	if (difY == 0)
	{
		int y = first->y;
		int xi = first->x;
		int xf = last->x;
		this->toFirst();
		for (int i = xi + 1; i < xf; i++)
		{
			Pixel *pixel = new Pixel(i, y);
			pixel->intensity = ((last->x - pixel->x) / (last->x - first->x) * first->intensity + (pixel->x - first->x) / (last->x - first->x) * last->intensity);
			cout << pixel->intensity;
			itr->next = pixel;
			itr = itr->next;
			(num)++;
		}
		itr->next = last;
	}

	else if (difX == 0)
	{
		int x = first->x, yf, yi;

		if ((first->y)>(last->y))
		{
			yf = first->y;
			yi =  last->y;
		}

		else
		{
			yi =  first->y;
			yf =  last->y;
		}

		 this->toFirst();
		for (int i = yi + 1; i < yf; i++)
		{
			Pixel *pixel = new Pixel(x, i);
			pixel->intensity = ((last->x - pixel->x) / (last->x - first->x) * first->intensity + (pixel->x - first->x) / (last->x - first->x) * last->intensity);
			cout << pixel->intensity;
			 itr->next = pixel;
			 itr =  itr->next;
			( num)++;
		}
		 itr->next =  last;
	}

	else if ((difY / difX) < 1 && (difY / difX) > -1)
	{
		double m = difY / difX;
		double b =  first->y;

		 this->toFirst();
		for (int i = 1; i < abs(difX); i++)
		{
			int y = (m*i + b + 0.5);
			Pixel *pixel = new Pixel(( itr->x) + 1, y);
			pixel->intensity = ((last->x - pixel->x) / (last->x - first->x) * first->intensity + (pixel->x - first->x) / (last->x - first->x) * last->intensity);
			cout << pixel->intensity;
			 itr->next = pixel;
			 itr =  itr->next;
			( num)++;
		}
		 itr->next =  last;
	}
	else if ((difY / difX) >= 1)
	{
		if ( last->x <  first->x)
		{
			Pixel *temp =  last;
			 last =  first;
			 first = temp;
		}
		double m = difX / difY;
		double b =  first->x;

		 this->toFirst();
		for (int i = 1; i < abs(difY); i++)
		{
			int x = (m*i + b + 0.5);
			Pixel *pixel = new Pixel(x, ( itr->y) + 1);
			pixel->intensity = ((last->x - pixel->x) / (last->x - first->x) * first->intensity + (pixel->x - first->x) / (last->x - first->x) * last->intensity);
			cout << pixel->intensity;
			 itr->next = pixel;
			 itr =  itr->next;
			( num)++;
		}
		 itr->next =  last;
	}

	else if ((difY / difX) <= -1)
	{
		
		double m = difX / difY;
		double b =  first->x;

		 this->toFirst();
		for (int i = 1; i < abs(difY); i++)
		{
			int x = (abs(m)*i + b + 0.5);
			Pixel *pixel = new Pixel(x, ( itr->y) - 1);
			pixel->intensity = ((last->x - pixel->x) / (last->x - first->x) * first->intensity + (pixel->x - first->x) / (last->x - first->x) * last->intensity);
			cout << pixel->intensity;
			 itr->next = pixel;
			 itr =  itr->next;
			( num)++;
		}
		 itr->next =  last;
	}

	/*
	 toFirst();
	for (int i = 0; i <  num; i++)
	{
		int x = ( itr)->x;
		int y = ( itr)->y;
		window[y][x] = 255;
		 itr =  itr->next;
	}
	*/

}
void drawBren(Line* line)
{
	if (line->last->x < line->first->x)
	{
		Pixel *temp = line->last;
		line->last = line->first;
		line->first = temp;
	}
	double difX = (line->last->x) - (line->first->x);
	double difY = (line->last->y) - (line->first->y);
	int Pi = 2 * difY - difX;

	/*
	int x = line->first->x;
	int y = line->first->y;
	window[y][x] = 255;

	x = line->last->x;
	y = line->last->y;
	window[y][x] = 255;
	*/
	/*
	if (difY == 0)
	{
		int y = line->first->y;
		int xi = line->first->x;
		int xf = line->last->x;
		line->toFirst();
		for (int i = xi + 1; i < xf; i++)
		{
			Pixel *pixel = new Pixel(i, y);
			line->itr->next = pixel;
			line->itr = line->itr->next;
			(line->num)++;
		}
		line->itr->next = line->last;
	}

	else if (difX == 0)
	{
		int x = line->first->x, yf, yi;

		if ((line->first->y)>(line->last->y))
		{
			yf = line->first->y;
			yi = line->last->y;
		}

		else
		{
			yi = line->first->y;
			yf = line->last->y;
		}

		line->toFirst();
		for (int i = yi + 1; i < yf; i++)
		{
			Pixel *pixel = new Pixel(x, i);
			line->itr->next = pixel;
			line->itr = line->itr->next;
			(line->num)++;
		}
		line->itr->next = line->last;
	}*/

	//cout << (difY / difX) << endl;
	if (0 <= (difY / difX) && (difY/difX) < 1)
	{
		int yadd;

		if (Pi > 0)
		{
			yadd = 1;
		}

		else
		{
			yadd = 0;
		}

		line->toFirst();
		int yi = line->first->y;//initial yi

		line->first->next = new Pixel((line->first->x)+1, yi + yadd);//create next Pixel with x+1, yi + either 1 or 0
		line->num++;
		line->itr = line->itr->next;

		for (int i = line->itr->x; i < line->last->x; i++)
		{
			int y = line->itr->y;
			Pi = Pi + (2 * difY) - (2 * difX*(y - yi));

			if (Pi > 0)
			{
				yadd = 1;
			}
			else
			{
				yadd = 0;
			}

			Pixel *pix = new Pixel(i + 1, y + yadd);
			yi = y;
			line->itr->next = pix;
			line->num++;
			line->itr = line->itr->next;
		}

		line->itr->next = line->last;
	}//Breneham

	else if (-1 < (difY/difX) && (difY/difX) < 0)
	{
		int yadd;

		if (Pi < 0)
		{
			yadd = -1;
		}

		else
		{
			yadd = 0;
		}

		line->toFirst();
		int yi = line->first->y;//initial yi

		line->first->next = new Pixel((line->first->x) + 1, yi + yadd);//create next Pixel with x+1, yi + either 1 or 0
		line->num++;
		line->itr = line->itr->next;

		int y = line->itr->y;
		for (int i = line->itr->x; i < line->last->x; i++)
		{
			y = line->itr->y;
			Pi = Pi + (2 * difY) - (2 * difX*(y - yi));

			if (Pi <= 0 && y > line->last->y)
			{
				yadd = -1;
			}
			else if (Pi > 0)
			{
				yadd = 0;
			}

			Pixel *pix = new Pixel(i + 1, y + yadd);
			yi = y;
			line->itr->next = pix;
			line->num++;
			line->itr = line->itr->next;
		}

		line->itr->next = line->last;
	}

	else if (difY / difX <= -1)
	{
		int xadd;

		if (Pi < 0)
		{
			xadd = 1;
		}

		else
		{
			xadd = 0;
		}

		line->toFirst();
		int xi = line->first->x;//initial xi

		line->first->next = new Pixel(xi+xadd,(line->first->y) - 1);//create next Pixel with x+1, yi + either 1 or 0
		line->num++;
		line->itr = line->itr->next;

		for (int i = line->itr->y; i > line->last->y; i--)
		{
			int x = line->itr->x;
			Pi = Pi + (2 * difX) - (2 * difY*(x - xi));

			if (Pi > 0)
			{
				xadd = 1;
			}
			else
			{
				xadd = 0;
			}

			Pixel *pix = new Pixel(x + xadd,i-1);
			xi = x;
			line->itr->next = pix;
			line->num++;
			line->itr = line->itr->next;
		}

		line->itr->next = line->last;
	}

	else if (difY / difX >= 1)
	{
		int xadd;

		if (Pi >= 0)
		{
			xadd = 1;
		}

		else
		{
			xadd = 0;
		}

		line->toFirst();
		int xi = line->first->x;//initial xi

		line->first->next = new Pixel(xi + xadd, (line->first->y) + 1);//create next Pixel with x+1, yi + either 1 or 0
		line->num++;
		line->itr = line->itr->next;

		for (int i = line->itr->y; i < line->last->y; i++)
		{
			int x = line->itr->x;
			Pi = Pi + (2 * difX) - (2 * difY*(x - xi));

			if (Pi >= 0)
			{
				xadd = 1;
			}
			else
			{
				xadd = 0;
			}

			Pixel *pix = new Pixel(x + xadd, i + 1);
			xi = x;
			line->itr->next = pix;
			line->num++;
			line->itr = line->itr->next;
		}

		line->itr->next = line->last;
	}

	/*
	line->toFirst();
	for (int i = 0; i < line->num; i++)
	{
		int x = (line->itr)->x;
		int y = (line->itr)->y;
		window[y][x] = 255;
		line->itr = line->itr->next;
	}*/
}
void rastPoly()
{
	bool in = true;
	for (int i = 0; i < 480; i++)
	{
		Pixel *current = NULL;
		Pixel *prev = NULL;
		Pixel *first = NULL;
		for (int j = 0; j < 360; j++)
		{
			if (window[i][j] == 255)
			{
				Pixel *out = new Pixel(j, i);
				if (current == NULL)
				{
					current = out;
					first = current;
				}
				else 
				{
					prev = current;
					current->next = out;
					current = current->next;
				}
			}
		}
		if (current!= NULL)
		{
			Pixel *itr =first;
			while (itr!=NULL && itr->next != NULL)
			{
				for (int k = 0; k < numPl; k++)
				{
					//cout << "(" << j << ", " << i << ")" << endl;
					for (int l = 0; l < polys[k]->numPts; l++)
					{
						if ((polys[k]->points[l]->x == itr->x && polys[k]->points[l]->y == itr->y) || (polys[k]->points[l]->x == itr->next->x && polys[k]->points[l]->y == itr->next->y))
						{
							itr = itr->next;
							k = numPl;
							break;
							in = false;
						}
					}
				}
				if (in == true && itr->next != NULL)
				{
					Line *line = new Line(itr, itr->next);
					line->drawLine();
					inputLine(line);
					itr = itr->next->next;
				}
			}
		}
		
	}
	//glutPostRedisplay();
	/*
	bool in=true;
	for (int i = 0; i < 480; i++)
	{
		
		for (int j = 0; j < 360; j++)
		{
			if (window[i][j] == 255)
			{
				
				in = true;
				while (in == true && j < 360)
				{
					j++;
					for (int k = 0; k < numPl; k++)
					{
						//cout << "(" << j << ", " << i << ")" << endl;
						for (int l = 0; l < polys[k]->numPts; l++)
						{
							if (polys[k]->points[l]->x == j && polys[k]->points[l]->y == i)
								in = false;
						}
					}

					if (window[i][j] == 255)
						in = false;
					else
						window[i][j] = 255;
				}
			}
		}
	}
	*/
}

void Polygon::callRast()
{
	rastPoly();
}
void drawPoly()
{
	for (int i = 0; i < numPl; i++)
	{
		for (int j = 0; j < polys[i]->numPts; j++)
		{
			getLineAlg(polys[i]->lines[j]);
			inputLine(polys[i]->lines[j]);
		}
		//getCenter(polys[i]);
	}

	glutPostRedisplay();
}
void translate(double xshift, double yshift, int pNum)
{
	for (int i = 0; i < polys[pNum]->numPts; i++)
	{
		polys[pNum]->points[i]->x += xshift;
		polys[pNum]->points[i]->y += yshift;
	}

	int j;
	for (j = 0; j < polys[pNum]->numPts - 1; j++)
	{
		Line *line1 = new Line(polys[pNum]->points[j], polys[pNum]->points[j + 1]);
		polys[pNum]->lines[j] = line1;
	}
	Line *line1 = new Line(polys[pNum]->points[0], polys[pNum]->points[j]);
	polys[pNum]->lines[j] = line1;

}
void rotate(double angle, Polygon *poly)
{
	Pixel *pix = getCenter(poly);
	translate(-(pix->x), -(pix->y), poly->polyNum);

	double cosa = cos(angle);
	double sina = sin(angle);
	for (int i = 0; i < poly->numPts; i++)
	{
		int x = (cosa * (poly->points[i]->x)) + ((-sina) * (poly->points[i]->y));
		int y = (sina * (poly->points[i]->x)) + ((cosa) * (poly->points[i]->y));

		poly->points[i]->x = x;
		poly->points[i]->y = y;
	}

	translate((pix->x), (pix->y), poly->polyNum);

	int j;
	for (j = 0; j < poly->numPts - 1; j++)
	{
		Line *line1 = new Line(poly->points[j], poly->points[j + 1]);
		poly->lines[j] = line1;
	}
	Line *line1 = new Line(poly->points[0], poly->points[j]);
	poly->lines[j] = line1;
}
void scale(int degree, Polygon *poly)
{
	
	Pixel *pix = getCenter(poly);
	translate(-(pix->x), -(pix->y), poly->polyNum);

	for (int i = 0; i < poly->numPts; i++)
	{
		poly->points[i]->x *= 2;
		poly->points[i]->y *= 2;
	}

	translate((pix->x), (pix->y), poly->polyNum);

	int j;
	for (j = 0; j < poly->numPts - 1; j++)
	{
		Line *line1 = new Line(poly->points[j], poly->points[j + 1]);
		poly->lines[j] = line1;
	}
	Line *line1 = new Line(poly->points[0], poly->points[j]);
	poly->lines[j] = line1;
	
}
void clip()
{
	//test west border

	int west = (360 - windX)/2;
	for (int i = 0; i < numPl; i++)
	{
		Polygon *p = new Polygon();
		int pointsItr = 0;
		for (int j = 0; j < polys[i]->numPts; j++)
		{
			getLineAlg(polys[i]->lines[j]);
			p->points = new Pixel*[polys[i]->numPts * 2];
			bool shift = false;
			while (polys[i]->lines[j]->first->x < west && polys[i]->lines[j]->first != NULL)
			{
				polys[i]->lines[j]->first = polys[i]->lines[j]->first->next;
				shift = true;
			}//shift pixel

			if (polys[i]->lines[j]->first != NULL && shift ==true)
			{
				p->points[pointsItr] = polys[i]->lines[j]->first;
				pointsItr++;
			}

			else if (polys[i]->lines[j]->first != NULL)
			{
				p->points[pointsItr] = polys[i]->lines[j]->first;
				pointsItr++;
				p->points[pointsItr] = polys[i]->lines[j]->last;
				pointsItr++;
			}
		}//for each line

		p->numPts = pointsItr;
		polys[i] = p;

		//connect lines
		int j;
		for (j = 0; j < polys[i]->numPts - 1; j++)
		{
			Line *line1 = new Line(polys[i]->points[j], polys[i]->points[j + 1]);
			polys[i]->lines[j] = line1;
		}
		Line *line1 = new Line(polys[i]->points[0], polys[i]->points[j]);
		polys[i]->lines[j] = line1;
		
	}//for each polygon
}
/*
void display(void)
{
	// clear buffer to values set in glClearColor
	glClear(GL_COLOR_BUFFER_BIT);
	//replace current matrix with identity matrix
	glLoadIdentity();

	
	///// Can not use any of the commands in this block for proj 1
	//glRasterPos2d(0, 0);
	
	//window[10][10] = 255;
	//window[1][1] = 510;
	glDrawPixels(480, 360, GL_RGB, GL_UNSIGNED_BYTE, window);
	/////  use glDrawPixels instead
	glColor3f(1.0, 0.0, 1.0);
	
	////end block
	glutSwapBuffers();
	//force execution of glcommands
	glFlush();
}
void init()
{
	//specify all values for the color buffer; (red, green, blue, alpha)
	glClearColor(0.0, 0.0, 0.0, 0.0);

	//can set viewing mode and projection here...not for first project
}
int main(int argc, char* argv[])
{
	// initialize and run program
	//Line *line = new Line();
	
	// GLUT initialization
	glutInit(&argc, argv);
	//set initial display mode, also GLUT_DOUBLE  
	glutInitDisplayMode(GLUT_SINGLE);
	//set window size (width, height)
	glutInitWindowSize(480,360);
	//set position, (x,y)
	glutInitWindowPosition(100, 100);
	//create with name
	glutCreateWindow("Program 1");
	//optional function "init"
	init();
	read();
	getTranslate();
	getRotate();
	getScale();
	getWindowSize();
	drawPoly();
	rastPoly();
	glutDisplayFunc(display);
	
	//set the callback function, called with each redisplay
	//enter processing loop (never ending)
	glutMainLoop();

	return 0;
}*/