# Variables
Comp = g++-9
Flags = -c -Wall -Wsign-compare -Wextra -O2 -std=c++11
TARGET = FEM
OBJS = Array.o Main.o Node.o Core.o Ke.o Fe.o Setup_Class.o Element_Tests.o Matrix_Tests.o Node_Tests.o Compress_K.o Pardiso_Tests.o
PATH_OBJS = $(patsubst %,obj/%,$(OBJS))

# All
All: $(PATH_OBJS) $(TARGET)


# Core rules
$(TARGET): $(PATH_OBJS)
	$(Comp) -o FEM $(PATH_OBJS)

obj/Main.o: Main.cc ./Element/Element_Tests.h ./Matrix/Matrix_Tests.h ./Node/Node_Tests.h
	$(Comp) $(Flags) Main.cc -o ./obj/Main.o



# Rules to make the Array class.
obj/Array.o: Array.cc Array.h
	$(Comp) $(Flags) Array.cc -o ./obj/Array.o



# Rules for the Node class.
obj/Node.o: ./Node/Node.cc ./Node/Node.h Array.h Errors.h
	$(Comp) $(Flags) ./Node/Node.cc -o ./obj/Node.o

obj/Node_Tests.o: ./Node/Node_Tests.cc ./Node/Node_Tests.h ./Node/Node.h Errors.h
	$(Comp) $(Flags) ./Node/Node_Tests.cc -o ./obj/Node_Tests.o



# Rules for the Element class
obj/Core.o: ./Element/Core.cc ./Element/Element.h ./Node/Node.h Errors.h ./Matrix/Matrix.h
	$(Comp) $(Flags) ./Element/Core.cc -o ./obj/Core.o

obj/Ke.o: ./Element/Ke.cc ./Element/Element.h ./Node/Node.h Errors.h ./Matrix/Matrix.h
	$(Comp) $(Flags) ./Element/Ke.cc -o ./obj/Ke.o

obj/Fe.o: ./Element/Fe.cc ./Element/Element.h ./Node/Node.h Errors.h ./Matrix/Matrix.h
	$(Comp) $(Flags) ./Element/Fe.cc -o ./obj/Fe.o

obj/Setup_Class.o: ./Element/Setup_Class.cc ./Element/Element.h ./Node/Node.h Errors.h ./Matrix/Matrix.h
	$(Comp) $(Flags) ./Element/Setup_Class.cc -o ./obj/Setup_Class.o

obj/Element_Tests.o: ./Element/Element_Tests.cc ./Element/Element_Tests.h ./Element/Element.h Errors.h
	$(Comp) $(Flags) ./Element/Element_Tests.cc -o ./obj/Element_Tests.o



# Rules for the matrix class.
obj/Matrix_Tests.o: ./Matrix/Matrix_Tests.cc ./Matrix/Matrix_Tests.h ./Matrix/Matrix.h Errors.h
	$(Comp) $(Flags) ./Matrix/Matrix_Tests.cc -o ./obj/Matrix_Tests.o



# Rules for the Pardiso directory
obj/Compress_K.o: ./Pardiso/Compress_K.cc ./Pardiso/Compress_K.h ./Matrix/Matrix.h
	$(Comp) $(Flags) ./Pardiso/Compress_K.cc -o ./obj/Compress_K.o

obj/Pardiso_Tests.o: ./Pardiso/Pardiso_Tests.cc ./Pardiso/Pardiso_Tests.h ./Matrix/Matrix.h
	$(Comp) $(Flags) ./Pardiso/Pardiso_Tests.cc -o ./obj/Pardiso_Tests.o



# Clean up!
Clean:
	rm ./obj/*.o FEM K.txt F.txt
