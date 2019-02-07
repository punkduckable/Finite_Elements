# Variables
Comp = g++ -std=c++11
Flags = -c -Wall -O3


# Rules
All: Element.o Errors.o Array.o Main.o Matrix.o Node.o Tests.o
	$(Comp) -o FEM Element.o Main.o Node.o Tests.o Errors.o

Array.o: Array.cc Array.h
	$(Comp) $(Flags) Array.cc

Element.o: Element.cc Element.h Node.h Errors.h
	$(Comp) $(Flags) Element.cc

Errors.o: Errors.cc Errors.h
	$(Comp) $(Flags) Errors.cc

Main.o: Main.cc Tests.h
	$(Comp) $(Flags) Main.cc

Matrix.o: Matrix.cc Matrix.h
	$(Comp) $(Flags) Array.cc

Node.o: Node.cc Node.h Array.h Errors.h
	$(Comp) $(Flags) Node.cc

Tests.o: Tests.cc Tests.h Node.h Matrix.h
	$(Comp) $(Flags) Tests.cc

Clean:
		rm *.o FEM
