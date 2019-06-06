#if !defined(NODE_TESTS_SOURCE)
#define NODE_TESTS_SOURCE

#include "Tests.h"
#include <stdio.h>

void Test::Node_Error_Tests(void) {
  ///////////////////////////////////////////////////////////////////////////////
  // Node Error handeling tests

  /* In these tests, we check that the Node class throws the correct errors */
  // First, declare a Node
  class Node Node;

  // Try to update the Node before it has been set up
  printf("Trying to update Node position before setting the node up...\n");
  try { Node.Update_Position(1, 37); }
  catch(const Node_Exception & Er) { printf("%s",Er.what()); }

  // Try Using the getter methods before the node has been set up.
  printf("Trying to get Has_BC information...\n");
  bool Has_BC;
  try { Has_BC = Node.Get_Has_BC(3); }
  catch(const Node_Not_Set_Up & Er) { printf("%s",Er.what()); }
  catch(const Array_Index_Out_Of_Bounds & Er) { printf("%s",Er.what()); }

  printf("Trying to get Original Position...\n");
  Array_3<double> Ar;
  try { Ar = Node.Get_Original_Position(); }
  catch(const Node_Not_Set_Up & Er) { printf("%s",Er.what()); }


  // Set the Node Original Position, BC's
  printf("\nSetting Node Original Position:\n ");
  try { Node.Set_Original_Position({0,0,0}); }
  catch(const Node_Exception & Er) { printf("%s",Er.what()); }

  printf("Setting Node BCs:\n ");
  try { Node.Set_BC(1,.5); }
  catch(const Node_Exception & Er) { printf("%s",Er.what()); }


  // Print out Node information
  printf("After set up:\n");
  Node.Print();
  printf("\n");


  // Try setting BC's and Original Position again.
  printf("\nTrying to set Position a second time...\n");
  try { Node.Set_Original_Position({1,2,3}); }
  catch(const Node_Already_Set_Up & Er) { printf("%s",Er.what()); }

  // Print out current Node information
  printf("After trying to set Original position twice:\n");
  Node.Print();
  printf("\n");



  // Modifiying node position
  printf("\nUpdating node position: ");
  try { Node.Update_Position(0, 37); }
  catch(const Node_Exception & Er) { printf("%s",Er.what()); }

  // Print new Node information
  printf("After setting 0 component of Node's position:\n");
  Node.Print();
  printf("\n");

  // Try setting an out of bounds component of position
  printf("\nTrying to set out of bounds component of position...\n");
  try { Node.Update_Position(3,37); }
  catch(const Array_Index_Out_Of_Bounds & Er) { printf("%s",Er.what()); }

  // Try getting out of bounds Fixed_Component information
  printf("Trying to get out of bounds Fixed_Component info\n");
  try { Has_BC = Node.Get_Has_BC(5); }
  catch(const Array_Index_Out_Of_Bounds & Er) { printf("%s",Er.what()); }

  // Print Node's information
  printf("After trying to modify current position:\n");
  Node.Print();
  printf("\n");

  printf("\nTest complete\n");
} // void Test::Node_Eror_Tests(void) {



void Test::Node(void) {
  //////////////////////////////////////////////////////////////////////////////
  // Node correctness test.

  /* In this test, we create a rectangular prism of nodes, set them up such that
  the bottom row of the nodes has a fixed y coordinate, and then check
  that everything is set up properly */

  // First, let's specify the dimensions of our prism
  const unsigned int Nx = 4;
  const unsigned int Ny = 3;
  const unsigned int Nz = 2;
  const double Intra_Nodal_Spacing = 1;                                        // Units : M


  /* Now, create an array of nodes. We store these nodes such that nodes in the
  same column (y coordinate varies, x and z are fixed) are in adjacent memory
  locations. Further, adjacent columns with the same z coordinate (x varies)
  are in adjacent memory locations */
  class Node Nodes[Nx*Ny*Nz];

  /* Now, let's set up the nodes. We will make it so that the bottom row of
  nodes (those with a y coordinate of zero) have a precribed BC in the y
  direction */
  for(unsigned k = 0; k < Nz; k++) {
    double z_pos = k*Intra_Nodal_Spacing;

    // Cycle throughh the columns at a given z coordinate
    for(unsigned i = 0; i < Nx; i++) {
      double x_pos = i*Intra_Nodal_Spacing;

      // Cycle through the nodes in a column.
      for(unsigned j = 0; j < Ny; j++) {
        double y_pos = j*Intra_Nodal_Spacing;

        Nodes[j + i*Ny + k*Nx*Ny].Set_Original_Position({x_pos, y_pos, z_pos});

        if(j == 0)
          Nodes[j + i*Ny + k*Nx*Ny].Set_BC(1,.5);
      } // for(unsigned j = 0; j < Ny; j++) {
    } // for(unsigned i = 0; i < Nx; i++) {
  } // for(unsigned k = 0; k < Nz; k++) {

  // Now, print the node information
  for(unsigned k = 0; k < Nz; k++) {
    for(unsigned i = 0; i < Nx; i++) {
      for(unsigned j = 0; j < Ny; j++) {
        printf("Node #%d\n", j + i*Ny + k*Nx*Ny);
        Nodes[j + i*Ny + k*Nx*Ny].Print();
        printf("\n");
      } // for(unsigned j = 0; j < Ny; j++) {
    } // for(unsigned i = 0; i < Nx; i++) {
  } // for(unsigned k = 0; k < Nz; k++) {
} // void Test::Node(void) {

#endif
