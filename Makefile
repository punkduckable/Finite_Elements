# Variables
COMPILER :=    g++-9
CFLAGS := 	  -c -Wall -Wsign-compare -Wextra -O2 -std=c++11

LIBS :=       -L/usr/local/Cellar/lapack/3.8.0_2/lib \
              -lblas.3.8.0 \
              -llapack.3.8.0 \
              -L/opt/intel/compilers_and_libraries_2019.4.233/mac/compiler/lib/ \
              -liomp5 \
             ./libpardiso600-MACOS-X86-64.dylib

R_PATH :=     -Wl,-rpath,$$ORIGIN -Wl,-rpath,/opt/intel/compilers_and_libraries_2019.4.233/mac/compiler/lib/

OBJS :=        Main.o \
					     Matrix_Tests.o \
               Node.o Node_Tests.o \
					     Core.o Ke.o Fe.o Setup_Class.o Element_Tests.o \
	             Compress_K.o Pardiso_Solve.o Pardiso_Tests.o Pardiso_Error.o \
							 inp_reader.o
PATH_OBJS := $(patsubst %,obj/%,$(OBJS))
VPATH :=     ./bin ./obj ./source ./source/Node ./source/Element ./source/Pardiso ./source/IO ./test



# All
All: $(PATH_OBJS) bin/FEM



# Core rules
bin/FEM: $(PATH_OBJS)
	$(COMPILER) $(PATH_OBJS) $(R_PATH) $(LIBS) -o $@

obj/Main.o: Main.cc Element_Tests.h Matrix_Tests.h Node_Tests.h
	$(COMPILER) $(CFLAGS) $< -o $@



# Rules for the Node class.
obj/Node.o: Node.cc Node.h Array.h Errors.h
	$(COMPILER) $(CFLAGS) $< -o $@

obj/Node_Tests.o: Node_Tests.cc Node_Tests.h Node.h Errors.h
	$(COMPILER) $(CFLAGS) $< -o $@



# Rules for the Element class
obj/Core.o: Core.cc Element.h Node.h Errors.h Matrix.h
	$(COMPILER) $(CFLAGS) $< -o $@

obj/Ke.o: Ke.cc Element.h Node.h Errors.h Matrix.h
	$(COMPILER) $(CFLAGS) $< -o $@

obj/Fe.o: Fe.cc Element.h Node.h Errors.h Matrix.h
	$(COMPILER) $(CFLAGS) $< -o $@

obj/Setup_Class.o: Setup_Class.cc Element.h Node.h Errors.h Matrix.h
	$(COMPILER) $(CFLAGS) $< -o $@

obj/Element_Tests.o: Element_Tests.cc Element_Tests.h Element.h Errors.h Pardiso_Solve.h
	$(COMPILER) $(CFLAGS) $< -o $@



# Rules for the matrix class.
obj/Matrix_Tests.o: Matrix_Tests.cc Matrix_Tests.h Matrix.h Errors.h
	$(COMPILER) $(CFLAGS) $< -o $@



# Rules for the Pardiso directory
obj/Compress_K.o: Compress_K.cc Compress_K.h Matrix.h
	$(COMPILER) $(CFLAGS) $< -o $@

obj/Pardiso_Solve.o: Pardiso_Solve.cc Pardiso_Solve.h Matrix.h Compress_K.h Pardiso.h
	$(COMPILER) $(CFLAGS) $< -o $@

obj/Pardiso_Tests.o: Pardiso_Tests.cc Pardiso_Tests.h Matrix.h
	$(COMPILER) $(CFLAGS) $< -o $@

obj/Pardiso_Error.o: Pardiso_Error.cc Pardiso.h
	$(COMPILER) $(CFLAGS) $< -o $@



# Rules for IO
obj/inp_reader.o: inp_reader.cc inp_reader.h Errors.h
	$(COMPILER) $(CFLAGS) $< -o $@



# Clean up!
Clean:
	rm ./obj/*.o ./bin/FEM ./IO/*.txt
