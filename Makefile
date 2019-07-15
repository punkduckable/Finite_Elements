# Variables
Comp = g++ -std=c++0x
Flags = -c -Wall -Wsign-compare -Wextra -O3


# Core Rules
All: Array.o Main.o Node.o Core.o Ke.o Fe.o Setup_Class.o Element_Tests.o Matrix_Tests.o Node_Tests.o Compress_K.o ./Matrix/Matrix.cc ./Matrix/Matrix.h
	$(Comp) -o FEM Array.o Main.o Node.o Core.o Ke.o Fe.o Setup_Class.o Element_Tests.o Matrix_Tests.o Node_Tests.o Compress_K.o

Array.o: Array.cc Array.h
	$(Comp) $(Flags) Array.cc

Main.o: Main.cc ./Tests/Tests.h
	$(Comp) $(Flags) Main.cc

Node.o: ./Node/Node.cc ./Node/Node.h Array.h Errors.h
	$(Comp) $(Flags) ./Node/Node.cc



# Rules to make the Element class
Core.o: ./Element/Core.cc ./Element/Element.h ./Node/Node.h Errors.h ./Matrix/Matrix.h
	$(Comp) $(Flags) ./Element/Core.cc

Ke.o: ./Element/Ke.cc ./Element/Element.h ./Node/Node.h Errors.h ./Matrix/Matrix.h
	$(Comp) $(Flags) ./Element/Ke.cc

Fe.o: ./Element/Fe.cc ./Element/Element.h ./Node/Node.h Errors.h ./Matrix/Matrix.h
	$(Comp) $(Flags) ./Element/Fe.cc

Setup_Class.o: ./Element/Setup_Class.cc ./Element/Element.h ./Node/Node.h Errors.h ./Matrix/Matrix.h
	$(Comp) $(Flags) ./Element/Setup_Class.cc



# Rules to make the test object files.
Element_Tests.o: ./Tests/Element_Tests.cc ./Tests/Tests.h ./Node/Node.h ./Matrix/Matrix.h ./Element/Element.h Errors.h
	$(Comp) $(Flags) ./Tests/Element_Tests.cc

Matrix_Tests.o: ./Tests/Matrix_Tests.cc ./Tests/Tests.h ./Node/Node.h ./Matrix/Matrix.h ./Element/Element.h Errors.h
	$(Comp) $(Flags) ./Tests/Matrix_Tests.cc

Node_Tests.o: ./Tests/Node_Tests.cc ./Tests/Tests.h ./Node/Node.h ./Matrix/Matrix.h ./Element/Element.h Errors.h
	$(Comp) $(Flags) ./Tests/Node_Tests.cc


# Rules for the Pardiso directory
Compress_K.o: ./Pardiso/Compress_K.cc ./Pardiso/Compress_K.h ./Matrix/Matrix.h
	$(Comp) $(Flags) ./Pardiso/Compress_K.cc

Clean:
	rm *.o FEM K.txt F.txt
