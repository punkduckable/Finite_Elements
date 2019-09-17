#if !defined(SIMULATION_TESTS_SOURCE)
#define SIMULATION_TESTS_SOURCE

#include "Simulation_Tests.h"

void Test::Mrudang_Test(void) {
  /* First, read in the inp file. */
  std::list<Array<double, 3>> Node_Positions;
  std::list<Array<unsigned, 8>> Element_Node_Lists;
  std::list<IO::Read::inp_boundary_data> Boundary_List;
  std::list<unsigned> Node_Set_List;

  std::string File_Name = "Job-1.inp";

  IO::Read::inp(File_Name, Node_Positions, Element_Node_Lists, Boundary_List);
  IO::Read::node_set(File_Name, Node_Set_List, "NSET4");

  printf("Read in %u nodes\n",    (unsigned)Node_Positions.size());
  printf("Read in %u elements\n", (unsigned)Element_Node_Lists.size());

  unsigned counter = 0;
  while(Node_Set_List.empty() == false) {
    printf("%3d", Node_Set_List.front());
    Node_Set_List.pop_front();
    if(counter == 15) {
      printf("\n");
      counter = 0;
    } // if(counter == 16) {
    else {
      printf(", ");
      counter++;
    } // else {
  } // while(Node_Set_List.empty() == false) {

  return;
} // void Test::Mrudang_Test(void) {

#endif
