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
  class Node Node;

  // Declare an Node_Error type variable
  Node_Errors::Errors Er;

  // Try to update the Node before it has been set up
  printf("Trying to update Node position before setting the node up...\n");
  Er = Node.Update_Position(37, 1);
  Node_Errors::Handle_Error(Er);

  // Set the Node's BC's, and original position
  printf("Setting node Poisition, BC's: ");
  Er = Node.Set_Original_Position({0,0,0},{false, true, false});
  Node_Errors::Handle_Error(Er);

  // Print out Node information
  printf("After set up:\n");
  Node.Print();
  printf("\n");


  // Try setting BC's and Original Position again.
  printf("Trying to set Original Position a second time...\n");
  Er = Node.Set_Original_Position({1,2,3},{true, false, true});
  Node_Errors::Handle_Error(Er);

  // Print out current Node information
  printf("After trying to set Original position twice:\n");
  Node.Print();
  printf("\n");



  // Modifiying current position
  printf("Updating current position: ");
  Er = Node.Update_Position(37, 0);
  Node_Errors::Handle_Error(Er);

  // Print new Node information
  printf("After setting 0 component of Node's position:\n");
  Node.Print();
  printf("\n");

  // Try modifying a protected Component of the position
  Er = Node.Update_Position(37, 1);
  Node_Errors::Handle_Error(Er);

  // Try setting an out of bounds component of position
  printf("Trying to set out of bounds component of position...\n");
  Er = Node.Update_Position(37, 3);
  Node_Errors::Handle_Error(Er);

  // Print Node's information
  printf("After trying to modify current position:\n");
  Node.Print();
  printf("\n");

  printf("Test complete\n");
} // void Test::Node_Errors(void) {



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
  for(int k = 0; k < Nz; k++) {
    double z_pos = k*Intra_Nodal_Spacing;

    // Cycle throughh the columns at a given z coordinate
    for(int i = 0; i < Nx; i++) {
      double x_pos = i*Intra_Nodal_Spacing;

      // Cycle through the nodes in a column.
      for(int j = 0; j < Ny; j++) {
        double y_pos = j*Intra_Nodal_Spacing;

        if(j == 0 )
          Nodes[j + i*Ny + k*Nx*Ny].Set_Original_Position({x_pos, .5, z_pos}, {false, true, false});
        else
          Nodes[j + i*Ny + k*Nx*Ny].Set_Original_Position({x_pos, y_pos, z_pos});
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
