Cheryl Kung
User Manual Project #4

Exe: p4.out

Initial curves are read in from data.txt and is structured as so:
number of Bezier curves
number of control points for curve 1
control points x y z
...
number of B-spline curves
number of control points curve 1
control points
...
number of knots
knots
...
order
number of points (display resolution)

The number of curves can only be modified within the data.txt file. 

To modify the curves:
In the window "Bezier and B-spline Curves" ONLY:
	enter 'r' to modify number of points(display resolution)
	enter 'b' to modify Bezier curves
	enter 'd' to modify b-spline curves

In the command line, it will ask for which curve. The curves are distinguished by the order they are entered (ie: first entered curve is 1). Then it will ask for the new parameters.
