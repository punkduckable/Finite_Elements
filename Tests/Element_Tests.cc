#if !defined(ELEMENT_TESTS_SOURCE)
#define ELEMENT_TESTS_SOURCE

#include "Tests.h"
#include <stdio.h>

void Test::Element_Error_Tests(void) {
  // First, lets create some elements.
  class Element El[4];
  Element_Errors El_Err;

  //////////////////////////////////////////////////////////////////////////////
  // Let's check that the "ELEMENT_NOT_SET_UP" Error is handled correctly

  printf("\nTrying to set nodes\n");
  El_Err = El[1].Set_Nodes(0,1,2,3,4,5,6,7);
  Handle_Error(El_Err);

  printf("\nTrying to populate Ke\n");
  El_Err = El[2].Populate_Ke();
  Handle_Error(El_Err);

  printf("\nTrying to move Ke to K\n");
  El_Err = El[3].Move_Ke_To_K();
  Handle_Error(El_Err);


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
  const double INS = .1;

  // Node array, ID array
  class Node Nodes[Num_Nodes];
  class Matrix<unsigned> ID(Num_Nodes, 3, Memory::ROW_MAJOR);

  // Set up Node positions and BC's
  unsigned Node_Index = 0;
  for(int i = 0; i < Nx; i++) {
    for(int j = 0; j < Ny; j++) {
      for(int k = 0; k < Nz; k++) {
        // Set the Node's original position + BC's
        Nodes[Node_Index].Set_Original_Position({INS*i, INS*j, INS*k});

        Node_Index++;
      } // for(int k = 0; k < Nz; k++) {
    } // for(int j = 0; j < Ny; j++) {
  } // for(int i = 0; i < Nx; i++) {

  // Populate ID array
  unsigned Num_Global_Eq = 0;
  Node_Index = 0;
  for(int i = 0; i < Nx; i++) {
    for(int j = 0; j < Ny; j++) {
      for(int k = 0; k < Nz; k++) {
        /* Cycle through the components of the current node. Use this info
        to populate ID. If the current component is not being used, fill that
        cell of ID with a -1 */
        for(int Comp = 0; Comp < 3; Comp++) {
          bool Has_BC;
          Nodes[Node_Index].Get_Has_BC(Comp, Has_BC);

          if(Has_BC == false) {
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
  // Create K, set Element static members, Material

  // Now that we know the # of Global equations, allocate K and F
  class Matrix<double> K(Num_Global_Eq, Num_Global_Eq, Memory::COLUMN_MAJOR);
  double * F = new double[Num_Global_Eq];

  // Zero initialize K
  for(int i = 0; i < Num_Global_Eq; i++)
    for(int j = 0; j < Num_Global_Eq; j++)
      K(i,j) = 0;

  // zero initialize F
  for(int i = 0; i < Num_Global_Eq; i++)
    F[i] = 0;


  // We are now ready to set the static members of the Element class
  printf("\nSetting static members of the Element class\n");
  El_Err = Set_Element_Static_Members(&ID, &K, F, Nodes);
  Handle_Error(El_Err);

  printf("Attempting to set the static members a second time\n");
  El_Err = Set_Element_Static_Members(&ID, &K, F, Nodes);
  Handle_Error(El_Err);

  printf("\nSetting Element material\n");
  El_Err = Set_Element_Material(10, .3);
  Handle_Error(El_Err);

  printf("Attempting to set Element material a second time\n");
  El_Err = Set_Element_Material(10, .3);
  Handle_Error(El_Err);

  // Now, create an array of elements.
  const unsigned Num_Elements = (Nx-1)*(Ny-1)*(Nz-1);
  class Element Elements[Num_Elements];


  ///////////////////////////////////////////////////////////////////////////////
  // Try using element methods before setting nodes.

  printf("\nAttempting to Populate_Ke before setting node list\n");
  El_Err = Elements[0].Populate_Ke();
  Handle_Error(El_Err);

  printf("Attempting to Fill_Ke_With_1s before setting node list\n");
  El_Err = Elements[0].Fill_Ke_With_1s();
  Handle_Error(El_Err);

  printf("Attempting to move Ke to K before setting node list\n");
  El_Err = Elements[0].Move_Ke_To_K();
  Handle_Error(El_Err);

  ///////////////////////////////////////////////////////////////////////////////
  // Now, set each element's node list

  /* Set up Node list, Popuatle Ke, and then move Ke to K for each element. */
  printf("\nSetting Node Lists... ");
  unsigned Element_Index = 0;
  for(int i = 0; i < Nx-1; i++) {
    for(int j = 0; j < Ny-1; j++) {
      for(int k = 0; k < Nz-1; k++) {
        Elements[Element_Index].Set_Nodes(Ny*Nz*i + Nz*j + k,
                                          Ny*Nz*(i+1) + Nz*j + k,
                                          Ny*Nz*(i+1) + Nz*(j+1) + k,
                                          Ny*Nz*i + Nz*(j+1) + k,
                                          Ny*Nz*i + Nz*j + (k+1),
                                          Ny*Nz*(i+1) + Nz*j + (k+1),
                                          Ny*Nz*(i+1) + Nz*(j+1) + (k+1),
                                          Ny*Nz*i + Nz*(j+1) + (k+1));
        Element_Index++;
      } // for(int k = 0; k < Nz-1; k++) {
    } // for(int j = 0; j < Ny-1; j++) {
  } // for(int i = 0; i < Nx-1; i++) {
  printf("Done!\n");

  printf("Attempting to move Ke to K\n");
  El_Err = Elements[0].Move_Ke_To_K();
  Handle_Error(El_Err);

  printf("Attempting to move Fe to F\n");
  El_Err = Elements[0].Move_Fe_To_F();
  Handle_Error(El_Err);

  printf("Attempting to populate Fe\n");
  El_Err = Elements[0].Populate_Fe();
  Handle_Error(El_Err);

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
  Handle_Error(El_Err);

  printf("Attempting to fill Element[0]'s Ke with 1's\n");
  El_Err = Elements[0].Fill_Ke_With_1s();
  Handle_Error(El_Err);

  printf("Attempting to move Fe to F\n");
  El_Err = Elements[0].Move_Fe_To_F();
  Handle_Error(El_Err);


  ///////////////////////////////////////////////////////////////////////////////
  // Move Ke to K

  Element_Index = 0;
  printf("\nConstructing K...\n");
  for(int i = 0; i < Nx-1; i++) {
    for(int j = 0; j < Ny-1; j++) {
      for(int k = 0; k < Nz-1; k++) {
        Elements[Element_Index].Move_Ke_To_K();
        Elements[Element_Index].Populate_Fe();
        Elements[Element_Index].Move_Fe_To_F();
        Element_Index++;
      } // for(int k = 0; k < Nz-1; k++) {
    } // for(int j = 0; j < Ny-1; j++) {
  } // for(int i = 0; i < Nx-1; i++) {
  printf("Done!\n");

  // In theory, K and F should now be set. Let's check. Print K, F to a file
  Simulation::Print_K_To_File(K, Simulation::Printing_Mode::INTEGER);
  Simulation::Print_F_To_File(F, Num_Global_Eq);
} // void Test::Element_Error_Tests(void) {



void Test::Element(void) {
  Element_Errors El_Err;

  //////////////////////////////////////////////////////////////////////////////
  // Specify dimension

  // First, specify the dimensions of the object that we're creating
  const unsigned Nx = 2;
  const unsigned Ny = 2;
  const unsigned Nz = 2;
  const unsigned Num_Nodes = Nx*Ny*Nz;
  const double INS = .1;                         // Inter-nodal spacing         Units: M


  //////////////////////////////////////////////////////////////////////////////
  // Set up Nodes.

  // Create the  array of nodes
  class Node Nodes[Num_Nodes];

  // Create the ID Array
  class Matrix<unsigned> ID(Num_Nodes, 3, Memory::ROW_MAJOR);

  // Set up Node positions and BC's
  unsigned Node_Index = 0;
  for(int i = 0; i < Nx; i++) {
    for(int j = 0; j < Ny; j++) {
      for(int k = 0; k < Nz; k++) {
        // Set the Node's original position + BC's
        Nodes[Node_Index].Set_Original_Position({INS*i, INS*j, INS*k});

        if(k == 0)
          Nodes[Node_Index].Set_BC(1,INS*j+1);

        Node_Index++;
      } // for(int k = 0; k < Nz; k++) {
    } // for(int j = 0; j < Ny; j++) {
  } // for(int i = 0; i < Nx; i++) {

  // Populate ID array
  unsigned Num_Global_Eq = 0;
  Node_Index = 0;
  for(int i = 0; i < Nx; i++) {
    for(int j = 0; j < Ny; j++) {
      for(int k = 0; k < Nz; k++) {
        /* Cycle through the components of the current node. Use this info
        to populate ID. If the current component is not being used, fill that
        cell of ID with a -1 */
        for(int Comp = 0; Comp < 3; Comp++) {
          bool Has_BC;
          Nodes[Node_Index].Get_Has_BC(Comp, Has_BC);

          if(Has_BC == false) {
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
  double * F = new double[Num_Global_Eq];

  // Zero initialize K
  for(int i = 0; i < Num_Global_Eq; i++)
    for(int j = 0; j < Num_Global_Eq; j++)
      K(i,j) = 0;

  // zero initialize F
  for(int i = 0; i < Num_Global_Eq; i++)
    F[i] = 0;


  //////////////////////////////////////////////////////////////////////////////
  // Make some elements

  // Set up the element class
  El_Err = Set_Element_Static_Members(&ID, &K, F, Nodes);
  if(El_Err != Element_Errors::SUCCESS) {
    Handle_Error(El_Err);
    return;
  } // if(El_Err != Element_Errors::SUCCESS) {

  El_Err = Set_Element_Material(10, .3);
  if(El_Err != Element_Errors::SUCCESS) {
    Handle_Error(El_Err);
    return;
  } // if(El_Err != Element_Errors::SUCCESS) {

  // Create some elements
  const unsigned Num_Elements = (Nx-1)*(Ny-1)*(Nz-1);
  class Element Elements[Num_Elements];


  //////////////////////////////////////////////////////////////////////////////
  // Find K, F

  // Make a matrix to store all the node lists
  class Matrix<unsigned> Node_Lists = Matrix<unsigned>(Num_Elements, 8, Memory::ROW_MAJOR);

  /* Cycle through the elements. For each element, supply the nodes, compute Ke (and Fe)
  and then move Ke (and Fe) into K (and F)*/
  unsigned Element_Index = 0;
  for(int i = 0; i < Nx-1; i++) {
    for(int j = 0; j < Ny-1; j++) {
      for(int k = 0; k < Nz-1; k++) {
        /* Set Element Nodes:
        Now, set each Element's Node list. Note, the order that we set these
        nodes is very specific. This is done so that the orientation of the nodes
        in the element matches that on page 124 of Hughes' book. */
        El_Err = Elements[Element_Index].Set_Nodes(Ny*Nz*i + Nz*j + k,
                                                   Ny*Nz*(i+1) + Nz*j + k,
                                                   Ny*Nz*(i+1) + Nz*(j+1) + k,
                                                   Ny*Nz*i + Nz*(j+1) + k,
                                                   Ny*Nz*i + Nz*j + (k+1),
                                                   Ny*Nz*(i+1) + Nz*j + (k+1),
                                                   Ny*Nz*(i+1) + Nz*(j+1) + (k+1),
                                                   Ny*Nz*i + Nz*(j+1) + (k+1));
        // Check for error
        if(El_Err != Element_Errors::SUCCESS) {
          Handle_Error(El_Err);
          return;
        } // if(El_Err != Element_Errors::SUCCESS) {


        // Populate Ke and check for errors
        El_Err = Elements[Element_Index].Populate_Ke();
        if(El_Err != Element_Errors::SUCCESS) {
          Handle_Error(El_Err);
          return;
        } // if(El_Err != Element_Errors::SUCCESS) {


        // Populate Fe and check for errors
        El_Err = Elements[Element_Index].Populate_Fe();
        if(El_Err != Element_Errors::SUCCESS) {
          Handle_Error(El_Err);
          return;
        } // if(El_Err != Element_Errors::SUCCESS) {


        // Move Ke to K and check for errors
        El_Err = Elements[Element_Index].Move_Ke_To_K();
        if(El_Err != Element_Errors::SUCCESS) {
          Handle_Error(El_Err);
          return;
        } // if(El_Err != Element_Errors::SUCCESS) {


        // Move Fe to F and check for errors
        El_Err = Elements[Element_Index].Move_Fe_To_F();
        if(El_Err != Element_Errors::SUCCESS) {
          Handle_Error(El_Err);
          return;
        } // if(El_Err != Element_Errors::SUCCESS) {

        Element_Index++;
      } // for(int k = 0; k < Nz-1; k++) {
    } // for(int j = 0; j < Ny-1; j++)
  } // for(int i = 0; i < Nx-1; i++)

  // Print results to file.
  Simulation::Print_K_To_File(K);
  Simulation::Print_F_To_File(F, Num_Global_Eq);
} // void Test::Element_Errors(void) {





void Simulation::Print_K_To_File(const Matrix<double> & K, const Printing_Mode Mode) {
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
      if(Mode == Printing_Mode::INTEGER)
        fprintf(File,"%1.0lf ", K(i,j));
      else
        fprintf(File,"%8.1e ", K(i,j));
    } // for(int j = 0; j < Num_Cols; j++) {

    fprintf(File,"|\n");
  } // for(int i = 0; i < Num_Rows; i++) {

  // All done. Close the file
  fclose(File);
} // void Simulation::Print_K_To_File(const Matrix<double> & K) {



void Simulation::Print_F_To_File(const double * F, const unsigned Num_Global_Eq) {
  // First, open a new file.
  FILE * File = fopen("F.txt","w");

  // Now, print the contents of F to the file.
  fprintf(File, "| ");
  for(int i = 0; i < Num_Global_Eq; i++)
    fprintf(File, "%10.3e ", F[i]);
  fprintf(File, "|");

  // All done, close the file
  fclose(File);
} // void Simulation::Print_F_To_File(const double * F, const unsigned Num_Global_Eq) {

#endif
