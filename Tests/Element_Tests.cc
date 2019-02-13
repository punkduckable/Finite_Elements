#if !defined(ELEMENT_TESTS_SOURCE)
#define ELEMENT_TESTS_SOURCE

#include "Tests.h"
#include <stdio.h>

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
