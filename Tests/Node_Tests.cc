#if !defined(NODE_TESTS_SOURCE)
#define NODE_TESTS_SOURCE

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

  // Try Using the getter methods before the node has been set up.
  printf("Trying to get Has_BC information...\n");
  bool Has_BC;
  Er = Node.Get_Has_BC(3, Has_BC);
  Node_Errors::Handle_Error(Er);

  printf("Trying to get Original Position...\n");
  Array_3<double> Ar;
  Er = Node.Get_Original_Position(Ar);
  Node_Errors::Handle_Error(Er);


  // Set the Node Original Position, BC's
  printf("\nSetting Node Original Position:\n ");
  Er = Node.Set_Original_Position({0,0,0});
  Node_Errors::Handle_Error(Er);

  printf("Setting Node BCs:\n ");
  Er = Node.Set_BC(1,.5);
  Node_Errors::Handle_Error(Er);


  // Print out Node information
  printf("After set up:\n");
  Node.Print();
  printf("\n");


  // Try setting BC's and Original Position again.
  printf("\nTrying to set Position a second time...\n");
  Er = Node.Set_Original_Position({1,2,3});
  Node_Errors::Handle_Error(Er);

  // Print out current Node information
  printf("After trying to set Original position twice:\n");
  Node.Print();
  printf("\n");



  // Modifiying node position
  printf("\nUpdating node position: ");
  Er = Node.Update_Position(0, 37);
  Node_Errors::Handle_Error(Er);

  // Print new Node information
  printf("After setting 0 component of Node's position:\n");
  Node.Print();
  printf("\n");

  // Try setting an out of bounds component of position
  printf("\nTrying to set out of bounds component of position...\n");
  Er = Node.Update_Position(3,37);
  Node_Errors::Handle_Error(Er);

  // Try getting out of bounds Fixed_Component information
  printf("Trying to get out of bounds Fixed_Component info\n");
  Er = Node.Get_Has_BC( 5, Has_BC );
  Node_Errors::Handle_Error(Er);

  // Print Node's information
  printf("After trying to modify current position:\n");
  Node.Print();
  printf("\n");

  printf("\nTest complete\n");
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

        Nodes[j + i*Ny + k*Nx*Ny].Set_Original_Position({x_pos, y_pos, z_pos});

        if(j == 0)
          Nodes[j + i*Ny + k*Nx*Ny].Set_BC(1,.5);
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
