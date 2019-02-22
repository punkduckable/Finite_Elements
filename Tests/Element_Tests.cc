#if !defined(ELEMENT_TESTS_SOURCE)
#define ELEMENT_TESTS_SOURCE

#include "Tests.h"
#include <stdio.h>

void Test::Element_Errors(void) {
  // First, lets create some elements.
  class Element El[4];
  Element_Errors::Errors El_Err;

  //////////////////////////////////////////////////////////////////////////////
  // Let's check that the "ELEMENT_NOT_SET_UP" Error is handled correctly

  printf("\nTrying to set nodes\n");
  El_Err = El[1].Set_Nodes(0,1,2,3,4,5,6,7);
  Element_Errors::Handle_Error(El_Err);

  printf("\nTrying to populate Ke\n");
  El_Err = El[2].Populate_Ke();
  Element_Errors::Handle_Error(El_Err);

  printf("\nTrying to move Ke to K\n");
  El_Err = El[3].Move_Ke_To_K();
  Element_Errors::Handle_Error(El_Err);


  //////////////////////////////////////////////////////////////////////////////
  // Set up Nodes, ID array.

  /* Before we can actually set the static members, we need to set the nodes,
  the ID array, and K.*/
  printf("\nSetting up Nodes, ID array...");

  // Parameters
  const unsigned Nx = 3;
  const unsigned Ny = 3;
  const unsigned Nz = 3;
  const unsigned Num_Nodes = Nx*Ny*Nz;
  const double IPS = .1;

  // Node array, ID array
  class Node Nodes[Num_Nodes];
  class Matrix<unsigned> ID(Num_Nodes, 3, Memory::ROW_MAJOR);

  // Set up Nodes + ID array
  unsigned Num_Global_Eq = 0;
  unsigned Node_Index = 0;
  for(int i = 0; i < Nx; i++) {
    for(int j = 0; j < Ny; j++) {
      for(int k = 0; k < Nz; k++) {
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
            ID(Node_Index, Comp) = -1;           // Note: ID is an unsigned matrix. (unsigned)-1 is the largest possible unsigned integer
        } // for(int Comp = 0; Comp < 3; Comp++) {

        Node_Index++;
      } // for(int k = 0; k < Nz; k++) {
    } // for(int j = 0; j < Ny; j++) {
  } // for(int i = 0; i < Nx; i++) {
  printf("Done!\n");

  // Print out the ID array (make sure it was set up correctly)
  printf("\nID array: \n");
  for(int i = 0; i < Num_Nodes; i++) {
    printf("| ");
    for(int j = 0; j < 3; j++)
      /* Note, even though ID is a matrix of unsigned integers, I print ID(i,j)
      as a signed integer so that any components set to -1 show up as -1 rather
      than some nonsense large number. */
      printf(" %3d ", ID(i,j));
    printf("|\n");
  } // for(int i = 0; i < Num_Nodes; i++) {

  //////////////////////////////////////////////////////////////////////////////
  // Create K, set Element static members.

  // Now that we know the # of Global equations, allocate K
  class Matrix<double> K(Num_Global_Eq, Num_Global_Eq, Memory::COLUMN_MAJOR);

  // Set the elements of K to zero
  for(int i = 0; i < Num_Global_Eq; i++)
    for(int j = 0; j < Num_Global_Eq; j++)
      K(i,j) = 0;

  // We are now ready to set the static members of the Element class
  printf("\nSetting static members of the Element class\n");
  El_Err = Set_Element_Static_Members(&ID, &K);
  Element_Errors::Handle_Error(El_Err);

  printf("Attempting to set the static members a second time\n");
  El_Err = Set_Element_Static_Members(&ID, &K);
  Element_Errors::Handle_Error(El_Err);

  // Now, create an array of elements.
  const unsigned Num_Elements = (Nx-1)*(Ny-1)*(Nz-1);
  class Element Elements[Num_Elements];


  ///////////////////////////////////////////////////////////////////////////////
  // Try using element methods before setting nodes.

  printf("Attempting to Populate_Ke before setting node list\n");
  El_Err = Elements[0].Populate_Ke();
  Element_Errors::Handle_Error(El_Err);

  printf("Attempting to Fill_Ke_With_1s before setting node list\n");
  El_Err = Elements[0].Fill_Ke_With_1s();
  Element_Errors::Handle_Error(El_Err);

  printf("Attempting to move Ke to K before setting node list\n");
  El_Err = Elements[0].Move_Ke_To_K();
  Element_Errors::Handle_Error(El_Err);


  ///////////////////////////////////////////////////////////////////////////////
  // Now, set each element's node list

  /* Set up Node list, Popuatle Ke, and then move Ke to K for each element. */
  printf("\nSetting Node Lists... ");
  unsigned Element_Index = 0;
  for(int i = 0; i < Nx-1; i++) {
    for(int j = 0; j < Ny-1; j++) {
      for(int k = 0; k < Nz-1; k++) {
        Elements[Element_Index].Set_Nodes(i + Nx*j + Nx*Ny*k,
                                          i+1 + Nx*j + Nx*Ny*k,
                                          i + Nx*(j+1) + Nx*Ny*k,
                                          i+1 + Nx*(j+1) + Nx*Ny*k,
                                          i + Nx*j + Nx*Ny*(k+1),
                                          i+1 + Nx*j + Nx*Ny*(k+1),
                                          i + Nx*(j+1) + Nx*Ny*(k+1),
                                          i+1 + Nx*(j+1) + Nx*Ny*(k+1));
        Element_Index++;
      } // for(int k = 0; k < Nz-1; k++) {
    } // for(int j = 0; j < Ny-1; j++) {
  } // for(int i = 0; i < Nx-1; i++) {
  printf("Done!\n");

  printf("Attempting to move Ke to K\n");
  El_Err = Elements[0].Move_Ke_To_K();
  Element_Errors::Handle_Error(El_Err);


  ///////////////////////////////////////////////////////////////////////////////
  // Fill each Ke with 1's

  printf("\nFilling every Elemnet's Ke with 1's...");
  Element_Index = 0;
  for(int i = 0; i < Nx-1; i++) {
    for(int j = 0; j < Ny-1; j++) {
      for(int k = 0; k < Nz-1; k++) {
        Elements[Element_Index].Fill_Ke_With_1s();
        Element_Index++;
      } // for(int k = 0; k < Nz-1; k++) {
    } // for(int j = 0; j < Ny-1; j++) {
  } // for(int i = 0; i < Nx-1; i++) {
  printf("Done!\n");

  printf("Attempting to populate Element[0]'s (it's already set)\n");
  El_Err = Elements[0].Populate_Ke();
  Element_Errors::Handle_Error(El_Err);

  printf("Attempting to fill Element[0]'s Ke with 1's\n");
  El_Err = Elements[0].Fill_Ke_With_1s();
  Element_Errors::Handle_Error(El_Err);


  ///////////////////////////////////////////////////////////////////////////////
  // Move Ke to K

  Element_Index = 0;
  printf("\nConstructing K...");
  for(int i = 0; i < Nx-1; i++) {
    for(int j = 0; j < Ny-1; j++) {
      for(int k = 0; k < Nz-1; k++) {
        Elements[Element_Index].Move_Ke_To_K();
        Element_Index++;
      } // for(int k = 0; k < Nz-1; k++) {
    } // for(int j = 0; j < Ny-1; j++) {
  } // for(int i = 0; i < Nx-1; i++) {
  printf("Done!\n");

  // In theory, K should now be set. Let's check. Print K to a file
  Simulation::Print_K_To_File(K);
} // void Test::Element_Errors(void) {



void Test::Element(void) {
  //////////////////////////////////////////////////////////////////////////////
  // Specicy dimension

  // First, specify the dimensions of the object that we're creating
  const unsigned Nx = 4;
  const unsigned Ny = 4;
  const unsigned Nz = 4;
  const unsigned Num_Nodes = Nx*Ny*Nz;
  const double INS = .1;                         // Inter-nodal spacing         Units: M


  //////////////////////////////////////////////////////////////////////////////
  // Set up Nodes.

  // Create the  array of nodes
  class Node Nodes[Num_Nodes];

  // Create the ID Array
  class Matrix<unsigned> ID(Num_Nodes, 3, Memory::ROW_MAJOR);

  // Set up the Node positions + Populate the ID array
  unsigned Num_Global_Eq = 0;
  unsigned Node_Index = 0;
  for(int i = 0; i < Nx; i++) {
    for(int j = 0; j < Ny; j++) {
      for(int k = 0; k < Nz; k++) {
        // Set the Node's original position + BC's
        if(k == 0)
          Nodes[Node_Index].Set_Original_Position({INS*i, INS*j, INS*k}, {false, false, false});
        else
          Nodes[Node_Index].Set_Original_Position({INS*i, INS*j, INS*k}, {false, false, false});

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
            ID(Node_Index, Comp) = -1;           // Note: ID is an unsigned matrix. (unsigned)-1 is the largest unsigned integer
        } // for(int Comp = 0; Comp < 3; Comp++) {

        Node_Index++;
      } // for(int k = 0; k < Nz; k++) {
    } // for(int j = 0; j < Ny; j++) {
  } // for(int i = 0; i < Nx; i++) {

  // Print out the ID array (make sure it was set up correctly)
  printf("\nID array: \n");
  for(int i = 0; i < Num_Nodes; i++) {
    printf("| ");
    for(int j = 0; j < 3; j++)
      /* Note, even though ID is a matrix of unsigned integers, I print ID(i,j)
      as a signed integer so that any components set to -1 show up as -1 rather
      than some nonsense large number. */
      printf(" %3d ", ID(i,j));
    printf("|\n");
  } // for(int i = 0; i < Num_Nodes; i++) {


  //////////////////////////////////////////////////////////////////////////////
  // Set up K
  class Matrix<double> K(Num_Global_Eq, Num_Global_Eq, Memory::COLUMN_MAJOR);

  // Set the elements of K to zero
  for(int i = 0; i < Num_Global_Eq; i++)
    for(int j = 0; j < Num_Global_Eq; j++)
      K(i,j) = 0;


  //////////////////////////////////////////////////////////////////////////////
  // Make some elements

  // Set up the element class
  Set_Element_Static_Members(&ID, &K);

  // Create some elements
  const unsigned Num_Elements = (Nx-1)*(Ny-1)*(Nz-1);
  class Element Elements[Num_Elements];


  //////////////////////////////////////////////////////////////////////////////
  // Create ordered Node lists.

  // Make a matrix to store all the node lists
  class Matrix<unsigned> Node_Lists = Matrix<unsigned>(Num_Elements, 8, Memory::ROW_MAJOR);

  /* Now, set each Element's Node list. Note, the order that we set these nodes
  is very specific. This is done so that the orientation of the nodes in the
  element matches that on page 124 of Hughes' book. */
  unsigned Element_Index = 0;
  for(int i = 0; i < Nx-1; i++) {
    for(int j = 0; j < Ny-1; j++) {
      for(int k = 0; k < Nz-1; k++) {
        Elements[Element_Index].Set_Nodes(i + Nx*j + Nx*Ny*k,
                                          i+1 + Nx*j + Nx*Ny*k,
                                          i + Nx*(j+1) + Nx*Ny*k,
                                          i+1 + Nx*(j+1) + Nx*Ny*k,
                                          i + Nx*j + Nx*Ny*(k+1),
                                          i+1 + Nx*j + Nx*Ny*(k+1),
                                          i + Nx*(j+1) + Nx*Ny*(k+1),
                                          i+1 + Nx*(j+1) + Nx*Ny*(k+1));

        Elements[Element_Index].Fill_Ke_With_1s();
        Elements[Element_Index].Move_Ke_To_K();

        Element_Index++;
      } // for(int k = 0; k < Nz-1; k++) {
    } // for(int j = 0; j < Ny-1; j++)
  } // for(int i = 0; i < Nx-1; i++)

  Simulation::Print_K_To_File(K);
} // void Test::Element_Errors(void) {



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
      fprintf(File,"%1.0lf ", K(i,j));
    } // for(int j = 0; j < Num_Cols; j++) {

    fprintf(File,"|\n");
  } // for(int i = 0; i < Num_Rows; i++) {
} // void Simulation::Print_K_To_File(const Matrix<double> & K) {

#endif
