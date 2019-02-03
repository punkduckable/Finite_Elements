# Variables
Comp = g++ -std=c++11
Flags = -c -Wall -O3


# Rules
All: Element.o Array.o Main.o Node.o Tests.o
	$(Comp) -o FEM Element.o Main.o Node.o Tests.o

Array.o: Array.h Array.cc
	$(Comp) $(Flags) Array.cc

Element.o: Element.cc Element.h Node.h
	$(Comp) $(Flags) Element.cc

Main.o: Main.cc Tests.h
	$(Comp) $(Flags) Main.cc

Node.o: Node.cc Node.h Array.h Forward_Declarations.h
	$(Comp) $(Flags) Node.cc

Tests.o: Tests.cc Tests.h Node.h
	$(Comp) $(Flags) Tests.cc


Clean:
		rm *.o FEM
