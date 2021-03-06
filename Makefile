# Variables
COMPILER :=    g++-9
CFLAGS := 	  -c -Wall -Wsign-compare -Wextra -O2 -std=c++11

LIBS :=       -L/usr/local/Cellar/lapack/3.8.0_2/lib \
              -lblas.3.8.0 \
              -llapack.3.8.0 \
              -L/opt/intel/compilers_and_libraries_2019.4.233/mac/compiler/lib/ \
              -liomp5 \
             ./libpardiso600-MACOS-X86-64.dylib

INC_PATH :=   -iquote ./source \
              -iquote ./test

R_PATH :=     -Wl,-rpath,$$ORIGIN -Wl,-rpath,/opt/intel/compilers_and_libraries_2019.4.233/mac/compiler/lib/

OBJS :=        Main.o \
					     Matrix_Tests.o \
               Node.o Node_Tests.o \
					     Core.o Ke.o Fe.o Setup_Class.o Element_Tests.o \
	             Compress_K.o Pardiso_Solve.o Pardiso_Tests.o Pardiso_Error.o \
							 inp_Reader.o KFX_Writer.o vtk_Writer.o IO_Tests.o String_Ops.o \
							 Simulation.o Simulation_Tests.o
PATH_OBJS := $(patsubst %,obj/%,$(OBJS))
VPATH :=     ./bin ./obj ./source \
             ./source/Node ./source/Element ./source/Pardiso ./source/IO ./source/Simulation \
						 ./test



# All
All: $(PATH_OBJS) bin/FEM



# Core rules
bin/FEM: $(PATH_OBJS)
	$(COMPILER) $(PATH_OBJS) $(R_PATH) $(LIBS) -o $@

obj/Main.o: Main.cc Element_Tests.h Matrix_Tests.h Node_Tests.h IO_Tests.h
	$(COMPILER) $(CFLAGS) $(INC_PATH) $< -o $@



# Rules for the Node class.
obj/Node.o: Node.cc Node.h Array.h Errors.h
	$(COMPILER) $(CFLAGS) $(INC_PATH) $< -o $@

obj/Node_Tests.o: Node_Tests.cc Node_Tests.h Node.h Errors.h
	$(COMPILER) $(CFLAGS) $(INC_PATH) $< -o $@



# Rules for the Element class
obj/Core.o: Core.cc Element.h Node.h Errors.h Matrix.h
	$(COMPILER) $(CFLAGS) $(INC_PATH) $< -o $@

obj/Ke.o: Ke.cc Element.h Node.h Errors.h Matrix.h
	$(COMPILER) $(CFLAGS) $(INC_PATH) $< -o $@

obj/Fe.o: Fe.cc Element.h Node.h Errors.h Matrix.h
	$(COMPILER) $(CFLAGS) $(INC_PATH) $< -o $@

obj/Setup_Class.o: Setup_Class.cc Element.h Node.h Errors.h Matrix.h
	$(COMPILER) $(CFLAGS) $(INC_PATH) $< -o $@

obj/Element_Tests.o: Element_Tests.cc Element_Tests.h Element.h Errors.h Pardiso_Solve.h
	$(COMPILER) $(CFLAGS) $(INC_PATH) $< -o $@



# Rules for the matrix class.
obj/Matrix_Tests.o: Matrix_Tests.cc Matrix_Tests.h Matrix.h Errors.h
	$(COMPILER) $(CFLAGS) $(INC_PATH) $< -o $@



# Rules for the Pardiso directory
obj/Compress_K.o: Compress_K.cc Compress_K.h Matrix.h
	$(COMPILER) $(CFLAGS) $(INC_PATH) $< -o $@

obj/Pardiso_Solve.o: Pardiso_Solve.cc Pardiso_Solve.h Matrix.h Compress_K.h Pardiso.h
	$(COMPILER) $(CFLAGS) $(INC_PATH) $< -o $@

obj/Pardiso_Tests.o: Pardiso_Tests.cc Pardiso_Tests.h Matrix.h
	$(COMPILER) $(CFLAGS) $(INC_PATH) $< -o $@

obj/Pardiso_Error.o: Pardiso_Error.cc Pardiso.h
	$(COMPILER) $(CFLAGS) $(INC_PATH) $< -o $@



# Rules for IO
obj/inp_Reader.o: inp_Reader.cc inp_Reader.h Errors.h String_Ops.h
	$(COMPILER) $(CFLAGS) $(INC_PATH) $< -o $@

obj/KFX_Writer.o: KFX_Writer.cc KFX_Writer.h
	$(COMPILER) $(CFLAGS) $(INC_PATH) $< -o $@

obj/vtk_Writer.o: vtk_Writer.cc vtk_Writer.h Errors.h Node.h Element.h
	$(COMPILER) $(CFLAGS) $(INC_PATH) $< -o $@

obj/IO_Tests.o: IO_Tests.cc IO_Tests.h inp_Reader.h
	$(COMPILER) $(CFLAGS) $(INC_PATH) $< -o $@

obj/String_Ops.o: String_Ops.cc String_Ops.h
	$(COMPILER) $(CFLAGS) $(INC_PATH) $< -o $@



# Rules for Simulation
obj/Simulation.o: Simulation.cc Simulation.h Errors.h Matrix.h Array.h Node.h Element.h inp_Reader.h vtk_Writer.h
	$(COMPILER) $(CFLAGS) $(INC_PATH) $< -o $@

obj/Simulation_Tests.o: Simulation_Tests.cc Simulation_Tests.h Simulation.h
	$(COMPILER) $(CFLAGS) $(INC_PATH) $< -o $@



# Clean up!
Clean:
	rm ./obj/*.o ./bin/FEM ./IO/*.txt
