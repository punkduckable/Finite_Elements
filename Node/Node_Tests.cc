#if !defined(NODE_TESTS_SOURCE)
#define NODE_TESTS_SOURCE

#include "Node_Tests.h"
#include <stdio.h>



void Test::Node_Error_Tests(void) {
  ///////////////////////////////////////////////////////////////////////////////
  // Node Error handeling tests

  /* In these tests, we check that the Node class throws the correct errors */
  // First, declare a Node
  class Node Node;

  // Set the Node position, BC's
  printf("\nSetting Node Position:\n ");
  try {
    Node.Set_Position_Component(0, 123.2932);
    Node.Set_Position_Component(1, 5820.4829);
    Node.Set_Position_Component(2, 193.28492);
  } // try {
  catch(const Node_Exception & Er) { printf("%s",Er.what()); }

  printf("Setting y Bc to .5:\n ");
  try { Node.Set_BC_Component(1,.5); }
  catch(const Node_Exception & Er) { printf("%s",Er.what()); }

  printf("Setting Node Force vector to <1, 2, 3>");
  try {
    Node.Set_Force_Component(0, 1);
    Node.Set_Force_Component(1, 2);
    Node.Set_Force_Component(2, 3);
  } // try {
  catch(const Node_Exception & Er) { printf("%s", Er.what()); }

  // Print out Node information
  printf("After set up:\n");
  Node.Print();
  printf("\n");


  // Modifiying node position
  printf("\nUpdating Node Position to <1,2,3>: ");
  try {
    Node.Set_Position_Component(0, 1);
    Node.Set_Position_Component(1, 2);
    Node.Set_Position_Component(2, 3);
  } // try {
  catch(const Node_Exception & Er) { printf("%s",Er.what()); }

  // Print new Node information
  printf("After updating positio:n\n");
  Node.Print();
  printf("\n");

  // Try setting an out of bounds component of position
  printf("\nTrying to set out of bounds component of position...\n");
  try { Node.Set_Position_Component(3,37); }
  catch(const Array_Index_Out_Of_Bounds & Er) { printf("%s",Er.what()); }

  printf("\nTrying to set an out of bounds force component...\n");
  try { Node.Set_Force_Component(19, 2930); }
  catch(const Array_Index_Out_Of_Bounds & Er) { printf("%s",Er.what()); }

  // Try getting out of bounds Fixed_Component information
  printf("Trying to get out of bounds Fixed_Component info\n");
  try {
    bool Has_BC = Node.Get_Has_BC(5);
    printf("Does component 5 has a BC? %u", Has_BC);
  } // try {
  catch(const Array_Index_Out_Of_Bounds & Er) { printf("%s",Er.what()); }

  // Print Node's information
  printf("After trying to set invalid components:\n");
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

        Nodes[j + i*Ny + k*Nx*Ny].Set_Position_Component(0, x_pos);
        Nodes[j + i*Ny + k*Nx*Ny].Set_Position_Component(1, y_pos);
        Nodes[j + i*Ny + k*Nx*Ny].Set_Position_Component(2, z_pos);

        if(j == 0)
          Nodes[j + i*Ny + k*Nx*Ny].Set_BC_Component(1,.5);
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
