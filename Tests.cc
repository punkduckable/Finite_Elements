#if !defined(TESTS_SOURCE)
#define TESTS_SOURCE

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
  printf("Trying to get Fixed_Componsnt information...\n");
  bool Is_Fixed;
  Er = Node.Get_Is_Fixed(3, Is_Fixed);
  Node_Errors::Handle_Error(Er);

  printf("Trying to get Original position...\n");
  Array_3<double> Ar;
  Er = Node.Get_Original_Position(Ar);
  Node_Errors::Handle_Error(Er);

  printf("Trying to get Current position...\n");
  Er = Node.Get_Current_Position(Ar);
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
  Er = Node.Update_Position(0, 37);
  Node_Errors::Handle_Error(Er);

  // Print new Node information
  printf("After setting 0 component of Node's position:\n");
  Node.Print();
  printf("\n");

  // Try modifying a protected Component of the position
  Er = Node.Update_Position(1, 37);
  Node_Errors::Handle_Error(Er);

  // Try setting an out of bounds component of position
  printf("Trying to set out of bounds component of position...\n");
  Er = Node.Update_Position(3,37);
  Node_Errors::Handle_Error(Er);

  // Try getting out of bounds Fixed_Component information
  printf("Trying to get out of bounds Fixed_Component info\n");
  Er = Node.Get_Is_Fixed( 5, Is_Fixed );
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



void Test::Matrix_Tests(void) {
  printf("Row dominant test:\n");
  unsigned Num_Rows = 3;
  unsigned Num_Cols = 4;

  // First, declare the ROW_MAJOR matrix
  Matrix<int> M1;
  M1.Set_Up(Num_Rows, Num_Cols, Memory::ROW_MAJOR);

  // Populate M1
  printf("Populating matrix... ");
  for(int i = 0; i < Num_Rows; i++)
    for(int j = 0; j < Num_Cols; j++)
      M1(i,j) = i + j;
  printf("Done!\n");

  // Now, print out M1
  for(int i = 0; i < Num_Rows; i++) {
    // Start each row with a "|"
    printf("|");

    for(int j = 0; j < Num_Cols; j++)
      printf(" %3d ", M1(i,j));

    // End each row with a "|"
    printf(" |\n");
  } // for(int i = 0; i < Num_Rows; i++) {



  printf("\n\nColumn dominant test:\n");
  Num_Rows = 5;
  Num_Cols = 2;

  // First, declare the COLUMN_MAJOR matrix.
  Matrix<double> M2;
  M2.Set_Up(Num_Rows, Num_Cols, Memory::COLUMN_MAJOR);

  // Populate M2
  printf("Populating Matrix... ");
  for(int j = 0; j < Num_Cols; j++)
    for(int i = 0; i < Num_Rows; i++)
      M2(i,j) = 1./( (double)(i + j + 1));
  printf("Done!\n");

  // Now, print out M2
  for(int i = 0; i < Num_Rows; i++) {
    // Start each row with a "|"
    printf("|");

    for(int j = 0; j < Num_Cols; j++)
      printf(" %6.3lf ", M2(i,j));

    // End each row with a "|"
    printf(" |\n");
  } // for(int i = 0; i < Num_Rows; i++) {
} // void Test::Matrix_Tests(void) {




void Test::Element(void) {
  // First, lets create some elements.
  class Element El[4];
  Element_Errors::Errors El_Err;

  //////////////////////////////////////////////////////////////////////////////
  // Let's check that the "ELEMENT_NOT_SET_UP" Error is handled correctly

  printf("Trying to get Node ID\n");
  unsigned ID_Out = 0;
  El_Err = El[0].Node_ID(3, ID_Out);
  Element_Errors::Handle_Error(El_Err);

  printf("\nTrying to set nodes\n");
  El_Err = El[1].Set_Nodes(0,1,2,3,4,5,6,7);
  Element_Errors::Handle_Error(El_Err);

  printf("\nTruing to populate Ke\n");
  El_Err = El[2].Populate_Ke();
  Element_Errors::Handle_Error(El_Err);

  printf("\nTrying to move Ke to K\n");
  El_Err = El[3].Move_Ke_To_K();
  Element_Errors::Handle_Error(El_Err);


  //////////////////////////////////////////////////////////////////////////////
  // Now set Static members

  /* Before we can actually set the static members, we need to set
  Up an ID array, K matrix, Node Array, and F */
  const unsigned Nx = 3;
  const unsigned Ny = 3;
  const unsigned Nz = 3;
  const unsigned Num_Nodes = Nx*Ny*Nz;
  const double IPS = .1;

  class Node Nodes[Num_Nodes];

  class Matrix<int> ID(Num_Nodes, 3, Memory::ROW_MAJOR);
  unsigned Num_Global_Eq = 0;

  for(int i = 0; i < Nx; i++) {
    for(int j = 0; j < Ny; j++) {
      for(int k = 0; k < Nz; k++) {
        unsigned Node_Index = k + j*Nz + i*Nz*Ny;
        // Set the Node's original position + BC's
        if(k == 0)
          Nodes[Node_Index].Set_Original_Position({IPS*i, IPS*j, IPS*k}, {false, false, false});
        else
          Nodes[Node_Index].Set_Original_Position({IPS*i, IPS*j, IPS*k}, {false, false, false});

        /* Now cycle through the components of the current node. Use this info
        to populate ID. If the current component is not being used, fill that
        cell of ID with a -1 */
        for(int Comp = 0; Comp < 3; Comp++) {
          bool Is_Fixed;
          Nodes[Node_Index].Get_Is_Fixed(Comp, Is_Fixed);

          if(Is_Fixed == false) {
            ID(Node_Index, Comp) = Num_Global_Eq;

            // Increment number of equations by 1.
            Num_Global_Eq++;
          } // for(int Comp = 0; Comp < 3; Comp++) {
          else
            ID(Node_Index, Comp) = -1;
        } // for(int Comp = 0; Comp < 3; Comp++) {
      } // for(int k = 0; k < Nz; k++) {
    } // for(int j = 0; j < Ny; j++) {
  } // for(int i = 0; i < Nx; i++) {

  // Print out the ID array (make sure it was set up correctly)
  printf("\nID array: \n");
  for(int i = 0; i < Num_Nodes; i++) {
    printf("| ");
    for(int j = 0; j < 3; j++)
      printf(" %3u ", ID(i,j));
    printf("|\n");
  } // for(int i = 0; i < Num_Nodes; i++) {

  // Now that we know the # of Global equations, allocate K
  class Matrix<double> K(Num_Global_Eq, Num_Global_Eq, Memory::COLUMN_MAJOR);

  // Set the elements of K to zero
  for(int i = 0; i < Num_Global_Eq; i++)
    for(int j = 0; j < Num_Global_Eq; j++)
      K(i,j) = 0;

  // We are now ready to set the static members of the Element class
  printf("Setting static members of the Element class\n");
  Set_Element_Static_Members(&ID, &K, Simulation::F);

  // Now, create an array of elements.
  class Element Elements[(Nx-1)*(Ny-1)*(Nz-1)];

  /* Set up Node list, Popuatle Ke, and then move Ke to K for each element. */
  for(int i = 0; i < Nx-1; i++) {
    for(int j = 0; j < Ny-1; j++) {
      for(int k = 0; k < Nz-1; k++) {
        /* Note: The node list is set in such a way that the ID's are in ascendeing
        order. This should improve memory access */
        Elements[i + (Nx-1)*j + (Nx-1)*(Ny-1)*k].Set_Nodes(i + Nx*j + Nx*Ny*k,
                                                           i+1 + Nx*j + Nx*Ny*k,
                                                           i + Nx*(j+1) + Nx*Ny*k,
                                                           i+1 + Nx*(j+1) + Nx*Ny*k,
                                                           i + Nx*j + Nx*Ny*(k+1),
                                                           i+1 + Nx*j + Nx*Ny*(k+1),
                                                           i + Nx*(j+1) + Nx*Ny*(k+1),
                                                           i+1 + Nx*(j+1) + Nx*Ny*(k+1));

        Elements[i + (Nx-1)*j + (Nx-1)*(Ny-1)*k].Populate_Ke();
        Elements[i + (Nx-1)*j + (Nx-1)*(Ny-1)*k].Move_Ke_To_K();
      } // for(int k = 0; k < Nz-1; k++) {
    } // for(int j = 0; j < Ny-1; j++) {
  } // for(int i = 0; i < Nx-1; i++) {

  // In theory, K should now be set. Let's check. Print K to a file
  Simulation::Print_K_To_File(K);
} // void Test::Element(void) {



double Simulation::F(unsigned Na, unsigned ei, unsigned Nb, unsigned ej) {
  return 1.0;
} // double Simulation::F(unsigned Na, unsigned ei, unsigned Nb, unsigned ej) {



void Simulation::Print_K_To_File(const Matrix<double> & K) {
  // First, open a new file
  FILE * File = fopen("K.txt","w");

  // Get the number of Rows, Columns for K
  const unsigned Num_Rows = K.Get_Num_Rows();
  const unsigned Num_Cols = K.Get_Num_Cols();

  /* Note, this is not a very fast way to print K, since K is a column major
  matrix and we're printing it it in row major order. Oh well, this only runs
  once anyway */
  for(int i = 0; i < Num_Rows; i++) {
    fprintf(File,"| ");
    for(int j = 0; j < Num_Cols; j++) {
      fprintf(File,"%2.0lf ", K(i,j));
    } // for(int j = 0; j < Num_Cols; j++) {

    fprintf(File,"|\n");
  } // for(int i = 0; i < Num_Rows; i++) {
} // void Simulation::Print_K_To_File(const Matrix<double> & K) {

#endif
