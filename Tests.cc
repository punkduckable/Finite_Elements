#if !defined(TESTS_SOURCE)
#define TESTS_SOURCE

#include "Node.h"
#include "Tests.h"
#include <stdio.h>

void Test::Node_Errors(void) {
  ///////////////////////////////////////////////////////////////////////////////
  // Node Error handeling tests

  /* In these tests, we check that the Node class throws the correct errors */
  // First, declare a Node
  class Node N;

  // Declare an Node_Error type variable
  Node_Errors::Errors Er;

  // Set it's BC's, and original position
  Er = N.Set_Original_Position({0,0,0});
  Node_Errors::Handle_Error(Er);

  Er = N.Set_BCs({0,1,0},{0,5,0});
  Node_Errors::Handle_Error(Er);

  // Print out Node information
  printf("After set up:\n");
  N.Print();



  // Try setting BC's and Original Position again.
  Er = N.Set_Original_Position({0,0,0});
  Node_Errors::Handle_Error(Er);

  Er = N.Set_BCs({1,0,1},{6, 0, 57});
  Node_Errors::Handle_Error(Er);

  // Print out current Node information
  printf("After trying to set BC's, Original Position twice:\n");
  N.Print();



  // Modifiying current position
  Er = N.Update_Position(37, 0);
  Node_Errors::Handle_Error(Er);

  // Print new Node information
  printf("After setting 0 component of Node's position:\n");
  N.Print();

  // Try modifying a protected Component of the position
  Er = N.Update_Position(37, 1);
  Node_Errors::Handle_Error(Er);

  // Try setting an out of bounds component of position
  Er = N.Update_Position(37, 3);
  Node_Errors::Handle_Error(Er);

  // Print Node's information
  printf("After trying to modify current position:\n");
  N.Print();

  printf("Test complete\n");
} // void Test::Node_Errors(void) {



void Test::Node(void) {
  //////////////////////////////////////////////////////////////////////////////
  // Node correctness test.

  /* In this test, we create a rectangular prism of nodes, set them up such that
  the bottom row of the nodes has a fixed y coordinate, and then check
  that everything is set up properly */

  // First, let's specify the dimensions of our prism
  const unsigned int Nx = 5;
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
  for(int k = 0; k < Nz; k++) {
    double z_pos = k*Intra_Nodal_Spacing;

    // Cycle throughh the columns at a given z coordinate
    for(int i = 0; i < Nx; i++) {
      double x_pos = i*Intra_Nodal_Spacing;

      // Cycle through the nodes in a column.
      for(int j = 0; j < Ny; j++) {
        double y_pos = j*Intra_Nodal_Spacing;

        Nodes[j + i*Ny + k*Nx*Ny].Set_Original_Position({x_pos, y_pos, z_pos});

        // Check if we're on the bottom layer. If so then apply BCs
        if(j == 0)
          Nodes[j + i*Ny + k*Nx*Ny].Set_BCs({false, true, false}, {0, .5, 0});
      } // for(int j = 0; j < Ny; j++) {
    } // for(int i = 0; i < Nx; i++) {
  } // for(int k = 0; k < Nz; k++) {

  // Now, print the node information
  for(int k = 0; k < Nz; k++) {
    for(int i = 0; i < Nx; i++) {
      for(int j = 0; j < Ny; j++) {
        printf("Node #%d\n", j + i*Ny + k*Nx*Ny);
        Nodes[j + i*Ny + k*Nx*Ny].Print();
        printf("\n");
      } // for(int j = 0; j < Ny; j++) {
    } // for(int i = 0; i < Nx; i++) {
  } // for(int k = 0; k < Nz; k++) {
} // void Test::Node(void) {

#endif
