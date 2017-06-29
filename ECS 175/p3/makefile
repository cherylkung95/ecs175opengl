p2.out : p.o p2.o
	g++ -ansi -Wall -g -o p2.out -lglut -lGL -lGLU p.o p2.o
p2.o : p2.cpp
	g++ -ansi -Wall -g -c -lglut -lGL -lGLU p2.cpp
p.o : p.cpp p.h
	g++ -ansi -Wall -g -c -lglut -lGL -lGLU p.cpp

clean :
	rm -f p2.out p.o