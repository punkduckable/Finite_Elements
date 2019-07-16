# Variables
Comp = g++-9
Flags = -c -Wall -Wsign-compare -Wextra -O2 -std=c++11
LIBS =   -L. -lpardiso600-MACOS-X86-64 \
         -L/usr/local/Cellar/lapack/3.8.0_2/lib -llapack.3.8.0 \
         -L/usr/local/Cellar/lapack/3.8.0_2/lib -lblas.3.8.0 \
         -L/opt/intel/compilers_and_libraries_2019.4.233/mac/compiler/lib/ -liomp5

OBJS =    Main.o \
					Array.o \
					Matrix_Tests.o \
          Node.o Node_Tests.o \
					Core.o Ke.o Fe.o Setup_Class.o Element_Tests.o \
	        Compress_K.o Pardiso_Solve.o Pardiso_Tests.o
PATH_OBJS = $(patsubst %,obj/%,$(OBJS))
VPATH = ./bin ./obj ./Matrix ./Node ./Element ./Pardiso



# All
All: $(PATH_OBJS) bin/FEM


# Core rules
bin/FEM: $(PATH_OBJS)
	$(Comp) $(LIBS) $(PATH_OBJS) -o $@

obj/Main.o: Main.cc Element_Tests.h Matrix_Tests.h Node_Tests.h
	$(Comp) $(Flags) $< -o $@



# Rules to make the Array class.
obj/Array.o: Array.cc Array.h
	$(Comp) $(Flags) $< -o $@



# Rules for the Node class.
obj/Node.o: Node.cc Node.h Array.h Errors.h
	$(Comp) $(Flags) $< -o $@

obj/Node_Tests.o: Node_Tests.cc Node_Tests.h Node.h Errors.h
	$(Comp) $(Flags) $< -o $@



# Rules for the Element class
obj/Core.o: Core.cc Element.h Node.h Errors.h Matrix.h
	$(Comp) $(Flags) $< -o $@

obj/Ke.o: Ke.cc Element.h Node.h Errors.h Matrix.h
	$(Comp) $(Flags) $< -o $@

obj/Fe.o: Fe.cc Element.h Node.h Errors.h Matrix.h
	$(Comp) $(Flags) $< -o $@

obj/Setup_Class.o: Setup_Class.cc Element.h Node.h Errors.h Matrix.h
	$(Comp) $(Flags) $< -o $@

obj/Element_Tests.o: Element_Tests.cc Element_Tests.h Element.h Errors.h
	$(Comp) $(Flags) $< -o $@



# Rules for the matrix class.
obj/Matrix_Tests.o: Matrix_Tests.cc Matrix_Tests.h Matrix.h Errors.h
	$(Comp) $(Flags) $< -o $@



# Rules for the Pardiso directory
obj/Compress_K.o: Compress_K.cc Compress_K.h Matrix.h
	$(Comp) $(Flags) $< -o $@

obj/Pardiso_Solve.o: Pardiso_Solve.cc Pardiso_Solve.h Matrix.h Compress_K.h
	$(Comp) $(Flags) $< -o $@

obj/Pardiso_Tests.o: Pardiso_Tests.cc Pardiso_Tests.h Matrix.h
	$(Comp) $(Flags) $< -o $@



# Clean up!
Clean:
	rm ./obj/*.o ./bin/FEM K.txt F.txt
