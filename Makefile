# Variables
Comp = g++ -std=c++11
Flags = -c -Wall -O3


# Rules
All: Array.o Element.o Errors.o Main.o Matrix.o Node.o Element_Tests.o Matrix_Tests.o Node_Tests.o
	$(Comp) -o FEM Array.o Element.o Errors.o Main.o Matrix.o Node.o Element_Tests.o Matrix_tests.o Node_Tests.o

Array.o: Array.cc Array.h
	$(Comp) $(Flags) Array.cc

Element.o: Element.cc Element.h Node.h Errors.h Matrix.h
	$(Comp) $(Flags) Element.cc

Errors.o: Errors.cc Errors.h
	$(Comp) $(Flags) Errors.cc

Main.o: Main.cc ./Tests/Tests.h
	$(Comp) $(Flags) Main.cc

Matrix.o: Matrix.cc Matrix.h
	$(Comp) $(Flags) Matrix.cc

Node.o: Node.cc Node.h Array.h Errors.h
	$(Comp) $(Flags) Node.cc


# Rules to make the test object files.
Element_Tests.o: ./Tests/Element_Tests.cc ./Tests/Tests.h Node.h Matrix.h Element.h Errors.h
	$(Comp) $(Flags) ./Tests/Element_Tests.cc

Matrix_Tests.o: ./Tests/Matrix_Tests.cc ./Tests/Tests.h Node.h Matrix.h Element.h Errors.h
	$(Comp) $(Flags) ./Tests/Matrix_Tests.cc

Node_Tests.o: ./Tests/Node_Tests.cc ./Tests/Tests.h Node.h Matrix.h Element.h Errors.h
	$(Comp) $(Flags) ./Tests/Node_Tests.cc


Clean:
		rm *.o FEM
