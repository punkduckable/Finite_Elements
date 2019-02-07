# Variables
Comp = g++ -std=c++11
Flags = -c -Wall -O3


# Rules
All: Array.o Element.o Errors.o Main.o Matrix.o Node.o Tests.o
	$(Comp) -o FEM Array.o Element.o Errors.o Main.o Matrix.o Node.o Tests.o

Array.o: Array.cc Array.h
	$(Comp) $(Flags) Array.cc

Element.o: Element.cc Element.h Node.h Errors.h Matrix.h
	$(Comp) $(Flags) Element.cc

Errors.o: Errors.cc Errors.h
	$(Comp) $(Flags) Errors.cc

Main.o: Main.cc Tests.h
	$(Comp) $(Flags) Main.cc

Matrix.o: Matrix.cc Matrix.h
	$(Comp) $(Flags) Matrix.cc

Node.o: Node.cc Node.h Array.h Errors.h
	$(Comp) $(Flags) Node.cc

Tests.o: Tests.cc Tests.h Node.h Matrix.h
	$(Comp) $(Flags) Tests.cc

Clean:
		rm *.o FEM
