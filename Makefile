# Variables
Comp = g++ -std=c++0x
Flags = -c -Wall -Wsign-compare -Wextra -O3



# Core Rules
All: Array.o Main.o Node.o Core.o Ke.o Fe.o Setup_Class.o Element_Tests.o Matrix_Tests.o Node_Tests.o Compress_K.o Pardiso_Tests.o ./Matrix/Matrix.h
	$(Comp) -o FEM Array.o Main.o Node.o Core.o Ke.o Fe.o Setup_Class.o Element_Tests.o Matrix_Tests.o Node_Tests.o Compress_K.o Pardiso_Tests.o

Array.o: Array.cc Array.h
	$(Comp) $(Flags) Array.cc

Main.o: Main.cc ./Element/Element_Tests.h ./Matrix/Matrix_Tests.h ./Node/Node_Tests.h
	$(Comp) $(Flags) Main.cc



# Rules for the Node class.
Node.o: ./Node/Node.cc ./Node/Node.h Array.h Errors.h
	$(Comp) $(Flags) ./Node/Node.cc

Node_Tests.o: ./Node/Node_Tests.cc ./Node/Node_Tests.h ./Node/Node.h Errors.h
	$(Comp) $(Flags) ./Node/Node_Tests.cc



# Rules for the Element class
Core.o: ./Element/Core.cc ./Element/Element.h ./Node/Node.h Errors.h ./Matrix/Matrix.h
	$(Comp) $(Flags) ./Element/Core.cc

Ke.o: ./Element/Ke.cc ./Element/Element.h ./Node/Node.h Errors.h ./Matrix/Matrix.h
	$(Comp) $(Flags) ./Element/Ke.cc

Fe.o: ./Element/Fe.cc ./Element/Element.h ./Node/Node.h Errors.h ./Matrix/Matrix.h
	$(Comp) $(Flags) ./Element/Fe.cc

Setup_Class.o: ./Element/Setup_Class.cc ./Element/Element.h ./Node/Node.h Errors.h ./Matrix/Matrix.h
	$(Comp) $(Flags) ./Element/Setup_Class.cc

Element_Tests.o: ./Element/Element_Tests.cc ./Element/Element_Tests.h ./Element/Element.h Errors.h
	$(Comp) $(Flags) ./Element/Element_Tests.cc



# Rules for the matrix class.
Matrix_Tests.o: ./Matrix/Matrix_Tests.cc ./Matrix/Matrix_Tests.h ./Matrix/Matrix.h Errors.h
	$(Comp) $(Flags) ./Matrix/Matrix_Tests.cc



# Rules for the Pardiso directory
Compress_K.o: ./Pardiso/Compress_K.cc ./Pardiso/Compress_K.h ./Matrix/Matrix.h
	$(Comp) $(Flags) ./Pardiso/Compress_K.cc

Pardiso_Tests.o: ./Pardiso/Pardiso_Tests.cc ./Pardiso/Pardiso_Tests.h ./Matrix/Matrix.h
	$(Comp) $(Flags) ./Pardiso/Pardiso_Tests.cc



# Clean up!
Clean:
	rm *.o FEM K.txt F.txt
