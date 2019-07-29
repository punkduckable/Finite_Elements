#if !defined(ELEMENT_TESTS_SOURCE)
#define ELEMENT_TESTS_SOURCE

#include <stdio.h>
#include "Element_Tests.h"



void Test::Element_Error_Tests(void) {
  // First, lets create some elements.
  class Element El[4];

  //////////////////////////////////////////////////////////////////////////////
  // Let's check that the "ELEMENT_NOT_SET_UP" Error is handled correctly

  printf("\nTrying to set nodes\n");
  try { El[1].Set_Nodes(0,1,2,3,4,5,6,7); }
  catch(const Element_Not_Set_Up & Er) { printf("%s\n",Er.what()); }

  printf("\nTrying to populate Ke\n");
  try { El[2].Populate_Ke(); }
  catch(const Element_Not_Set_Up & Er) { printf("%s\n",Er.what()); }

  printf("\nTrying to move Ke to K\n");
  try { El[3].Move_Ke_To_K(); }
  catch(const Element_Not_Set_Up & Er) { printf("%s\n",Er.what()); }


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
  class Matrix<unsigned> ID{Num_Nodes, 3, Memory::ROW_MAJOR};

  // Set up Node positions and BC's
  unsigned Node_Index = 0;
  for(unsigned i = 0; i < Nx; i++) {
    double x_pos = INS*i;
    for(unsigned j = 0; j < Ny; j++) {
      double y_pos = INS*j;
      for(unsigned k = 0; k < Nz; k++) {
        double z_pos = INS*k;

        Nodes[Node_Index].Set_Position_Component(0, x_pos);
        Nodes[Node_Index].Set_Position_Component(1, y_pos);
        Nodes[Node_Index].Set_Position_Component(2, z_pos);

        Node_Index++;
      } // for(unsigned k = 0; k < Nz; k++) {
    } // for(unsigned j = 0; j < Ny; j++) {
  } // for(unsigned i = 0; i < Nx; i++) {

  // Populate ID array
  unsigned Num_Global_Eq = 0;
  Node_Index = 0;
  for(unsigned i = 0; i < Nx; i++) {
    for(unsigned j = 0; j < Ny; j++) {
      for(unsigned k = 0; k < Nz; k++) {
        /* Cycle through the components of the current node. Use this info
        to populate ID. If the current component is not being used, fill that
        cell of ID with a -1 */
        for(unsigned Comp = 0; Comp < 3; Comp++) {
          bool Has_BC;
          Has_BC = Nodes[Node_Index].Get_Has_BC(Comp);

          if(Has_BC == false) {
            ID(Node_Index, Comp) = Num_Global_Eq;

            // Increment number of equations by 1.
            Num_Global_Eq++;
          } // for(unsigned Comp = 0; Comp < 3; Comp++) {
          else
            ID(Node_Index, Comp) = -1;           // Note: ID is an unsigned matrix. (unsigned)-1 is the largest unsigned integer
        } // for(unsigned Comp = 0; Comp < 3; Comp++) {

        Node_Index++;
      } // for(unsigned k = 0; k < Nz; k++) {
    } // for(unsigned j = 0; j < Ny; j++) {
  } // for(unsigned i = 0; i < Nx; i++) {
  printf("Done!\n");

  // Print out the ID array (make sure it was set up correctly)
  printf("\nID array: \n");
  for(unsigned i = 0; i < Num_Nodes; i++) {
    printf("| ");
    for(int j = 0; j < 3; j++)
      /* Note, even though ID is a matrix of unsigned integers, I print ID(i,j)
      as a signed integer so that any components set to -1 show up as -1 rather
      than some nonsense large number. */
      printf(" %3d ", ID(i,j));
    printf("|\n");
  } // for(unsigned i = 0; i < Num_Nodes; i++) {

  //////////////////////////////////////////////////////////////////////////////
  // Create K, set Element static members, Material

  // Now that we know the # of Global equations, allocate K and F
  class Matrix<double> K{Num_Global_Eq, Num_Global_Eq, Memory::COLUMN_MAJOR};
  double * F = new double[Num_Global_Eq];

  // Zero initialize K (Note: K is Column-major)
  for(unsigned j = 0; j < Num_Global_Eq; j++)
    for(unsigned i = 0; i < Num_Global_Eq; i++)
      K(i,j) = 0;

  // zero initialize F
  for(unsigned i = 0; i < Num_Global_Eq; i++)
    F[i] = 0;


  // We are now ready to set the static members of the Element class
  printf("\nSetting static members of the Element class\n");
  try { Set_Element_Static_Members(&ID, &K, F, Nodes); }
  catch(const Element_Exception & Er) { printf("%s\n",Er.what()); }

  printf("Attempting to set the static members a second time\n");
  try { Set_Element_Static_Members(&ID, &K, F, Nodes); }
  catch( const Element_Already_Set_Up & Er) { printf("%s\n",Er.what()); }

  printf("\nSetting Element material\n");
  try { Set_Element_Material(10, .3); }
  catch(const Element_Exception & Er) { printf("%s\n",Er.what()); }

  printf("Attempting to set Element material a second time\n");
  try { Set_Element_Material(10, .3); }
  catch(const Element_Already_Set_Up & Er) { printf("%s\n",Er.what()); }

  // Now, create an array of elements.
  const unsigned Num_Elements = (Nx-1)*(Ny-1)*(Nz-1);
  class Element Elements[Num_Elements];


  ///////////////////////////////////////////////////////////////////////////////
  // Try using element methods before setting nodes.

  printf("\nAttempting to Populate_Ke before setting node list\n");
  try { Elements[0].Populate_Ke(); }
  catch(const Element_Not_Set_Up & Er) { printf("%s\n",Er.what()); }

  printf("Attempting to Fill_Ke_With_1s before setting node list\n");
  try { Elements[0].Fill_Ke_With_1s(); }
  catch(const Element_Not_Set_Up & Er) { printf("%s\n",Er.what()); }

  printf("Attempting to move Ke to K before setting node list\n");
  try { Elements[0].Move_Ke_To_K(); }
  catch(const Element_Not_Set_Up & Er) { printf("%s\n",Er.what()); }

  ///////////////////////////////////////////////////////////////////////////////
  // Now, set each element's node list

  /* Set up Node list, Popuatle Ke, and then move Ke to K for each element. */
  printf("\nSetting Node Lists... ");
  unsigned Element_Index = 0;
  for(unsigned i = 0; i < Nx-1; i++) {
    for(unsigned j = 0; j < Ny-1; j++) {
      for(unsigned k = 0; k < Nz-1; k++) {
        Elements[Element_Index].Set_Nodes(Ny*Nz*i + Nz*j + k,
                                          Ny*Nz*(i+1) + Nz*j + k,
                                          Ny*Nz*(i+1) + Nz*(j+1) + k,
                                          Ny*Nz*i + Nz*(j+1) + k,
                                          Ny*Nz*i + Nz*j + (k+1),
                                          Ny*Nz*(i+1) + Nz*j + (k+1),
                                          Ny*Nz*(i+1) + Nz*(j+1) + (k+1),
                                          Ny*Nz*i + Nz*(j+1) + (k+1));
        Element_Index++;
      } // for(unsigned k = 0; k < Nz-1; k++) {
    } // for(unsigned j = 0; j < Ny-1; j++) {
  } // for(unsigned i = 0; i < Nx-1; i++) {
  printf("Done!\n");

  printf("Attempting to move Ke to K\n");
  try { Elements[0].Move_Ke_To_K(); }
  catch(const Element_Exception & Er) { printf("%s\n",Er.what()); }

  printf("Attempting to move Fe to F\n");
  try { Elements[0].Move_Fe_To_F(); }
  catch(const Element_Exception & Er) { printf("%s\n",Er.what()); }

  printf("Attempting to populate Fe\n");
  try { Elements[0].Populate_Fe(); }
  catch(const Element_Exception & Er) { printf("%s\n",Er.what()); }

  ///////////////////////////////////////////////////////////////////////////////
  // Fill each Ke with 1's

  printf("\nFilling every Elemnet's Ke with 1's...");
  Element_Index = 0;
  for(unsigned i = 0; i < Nx-1; i++) {
    for(unsigned j = 0; j < Ny-1; j++) {
      for(unsigned k = 0; k < Nz-1; k++) {
        Elements[Element_Index].Fill_Ke_With_1s();
        Element_Index++;
      } // for(unsigned k = 0; k < Nz-1; k++) {
    } // for(unsigned j = 0; j < Ny-1; j++) {
  } // for(unsigned i = 0; i < Nx-1; i++) {
  printf("Done!\n");

  printf("Attempting to populate Element[0]'s Ke (it's already set)\n");
  try { Elements[0].Populate_Ke(); }
  catch(const Element_Already_Set_Up & Er) { printf("%s\n",Er.what()); }

  printf("Attempting to fill Element[0]'s Ke with 1's\n");
  try { Elements[0].Fill_Ke_With_1s(); }
  catch(const Element_Already_Set_Up & Er) { printf("%s\n",Er.what());}

  printf("Attempting to move Fe to F\n");
  try { Elements[0].Move_Fe_To_F(); }
  catch(const Element_Exception & Er) { printf("%s\n",Er.what()); }


  ///////////////////////////////////////////////////////////////////////////////
  // Move Ke to K

  Element_Index = 0;
  printf("\nConstructing K...\n");
  for(unsigned i = 0; i < Nx-1; i++) {
    for(unsigned j = 0; j < Ny-1; j++) {
      for(unsigned k = 0; k < Nz-1; k++) {
        Elements[Element_Index].Move_Ke_To_K();
        Elements[Element_Index].Populate_Fe();
        Elements[Element_Index].Move_Fe_To_F();
        Element_Index++;
      } // for(unsigned k = 0; k < Nz-1; k++) {
    } // for(unsigned j = 0; j < Ny-1; j++) {
  } // for(unsigned i = 0; i < Nx-1; i++) {
  printf("Done!\n");

  // In theory, K and F should now be set. Let's check. Print K, F to a file
  Test::Print_K_To_File(K, Test::Printing_Mode::INTEGER);
  Test::Print_F_To_File(F, Num_Global_Eq);
} // void Test::Element_Error_Tests(void) {





void Test::Element(void) {
  //////////////////////////////////////////////////////////////////////////////
  // Specify dimension

  // First, specify the number of nodes in each direction
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
  class Matrix<unsigned> ID{Num_Nodes, 3, Memory::ROW_MAJOR};

  // Set up Node positions and BC's
  unsigned Node_Index = 0;
  for(unsigned i = 0; i < Nx; i++) {
    double x_pos = INS*i;
    for(unsigned j = 0; j < Ny; j++) {
      double y_pos = INS*j;
      for(unsigned k = 0; k < Nz; k++) {
        double z_pos = INS*k;

        // Set the Node's position
        Nodes[Node_Index].Set_Position_Component(0, x_pos);
        Nodes[Node_Index].Set_Position_Component(1, y_pos);
        Nodes[Node_Index].Set_Position_Component(2, z_pos);

        /* Constrain nodes in the yz plane in the x direction */
        if(i == 0) { Nodes[Node_Index].Set_BC_Component(0,0); }

        /* Constrain nodes in the xz plane in the y direction */
        if(j == 0) { Nodes[Node_Index].Set_BC_Component(1,0); }

        /* Constrain nodes in the xy plane in the z direction */
        if(k == 0) { Nodes[Node_Index].Set_BC_Component(2,0); }


        /* Stretch the +x face by 1/2 ISP */
        if(i == Nx-1) { Nodes[Node_Index].Set_BC_Component(0, .5*INS); }

        Node_Index++;
      } // for(unsigned k = 0; k < Nz; k++) {
    } // for(unsigned j = 0; j < Ny; j++) {
  } // for(unsigned i = 0; i < Nx; i++) {

  // Populate ID array
  unsigned Num_Global_Eq = 0;
  Node_Index = 0;
  for(unsigned i = 0; i < Nx; i++) {
    for(unsigned j = 0; j < Ny; j++) {
      for(unsigned k = 0; k < Nz; k++) {
        /* Cycle through the components of the current node. Use this info
        to populate ID. If the current component is not being used, fill that
        cell of ID with a -1 */
        for(unsigned Comp = 0; Comp < 3; Comp++) {
          bool Has_BC;
          Has_BC = Nodes[Node_Index].Get_Has_BC(Comp);

          if(Has_BC == false) {
            ID(Node_Index, Comp) = Num_Global_Eq;

            // Increment number of equations by 1.
            Num_Global_Eq++;
          } // for(unsigned Comp = 0; Comp < 3; Comp++) {
          else
            ID(Node_Index, Comp) = -1;           // Note: ID is an unsigned matrix. (unsigned)-1 is the largest unsigned integer
        } // for(unsigned Comp = 0; Comp < 3; Comp++) {

        Node_Index++;
      } // for(unsigned k = 0; k < Nz; k++) {
    } // for(unsigned j = 0; j < Ny; j++) {
  } // for(unsigned i = 0; i < Nx; i++) {

  // Print out the ID array (make sure it was set up correctly)
  printf("\nID array: \n");
  for(unsigned i = 0; i < Num_Nodes; i++) {
    printf("| ");
    for(unsigned j = 0; j < 3; j++)
      /* Note, even though ID is a matrix of unsigned integers, I print ID(i,j)
      as a signed integer so that any components set to -1 show up as -1 rather
      than some nonsense large number. */
      printf(" %3d ", ID(i,j));
    printf("|\n");
  } // for(unsigned i = 0; i < Num_Nodes; i++) {


  //////////////////////////////////////////////////////////////////////////////
  // Set up K, F, and x (the solution vector)
  class Matrix<double> K{Num_Global_Eq, Num_Global_Eq, Memory::COLUMN_MAJOR};
  double* F = new double[Num_Global_Eq];
  double* x = new double[Num_Global_Eq];

  // Zero initialize K and F
  K.Zero();
  for(unsigned i = 0; i < Num_Global_Eq; i++) { F[i] = 0; }


  //////////////////////////////////////////////////////////////////////////////
  // Set up element class and make some elements

  // Set up the element class
  try { Set_Element_Static_Members(&ID, &K, F, Nodes); }
  catch (const Element_Already_Set_Up & Er) {
    printf("%s\n",Er.what());
    return;
  } // catch (const Element_Already_Set_Up & Er) {

  try { Set_Element_Material(10, .3); }
  catch (const Element_Exception & Er) {
    printf("%s\n",Er.what());
    return;
  } //  catch (const Element_Exception & Er) {

  // Create some elements
  const unsigned Num_Elements = (Nx-1)*(Ny-1)*(Nz-1);
  class Element Elements[Num_Elements];


  //////////////////////////////////////////////////////////////////////////////
  // Find K, F

  // Make a matrix to store all the node lists
  class Matrix<unsigned> Node_Lists{Num_Elements, 8, Memory::ROW_MAJOR};

  /* Cycle through the elements. For each element, supply the nodes, compute Ke (and Fe)
  and then move Ke (and Fe) into K (and F)*/
  unsigned Element_Index = 0;
  for(unsigned i = 0; i < Nx-1; i++) {
    for(unsigned j = 0; j < Ny-1; j++) {
      for(unsigned k = 0; k < Nz-1; k++) {
        /* Set Element Nodes:
        Now, set each Element's Node list. Note, the order that we set these
        nodes is very specific. This is done so that the orientation of the nodes
        in the element matches that on page 124 of Hughes' book. */
        try {
          Elements[Element_Index].Set_Nodes(Ny*Nz*i + Nz*j + k,
                                            Ny*Nz*(i+1) + Nz*j + k,
                                            Ny*Nz*(i+1) + Nz*(j+1) + k,
                                            Ny*Nz*i + Nz*(j+1) + k,
                                            Ny*Nz*i + Nz*j + (k+1),
                                            Ny*Nz*(i+1) + Nz*j + (k+1),
                                            Ny*Nz*(i+1) + Nz*(j+1) + (k+1),
                                            Ny*Nz*i + Nz*(j+1) + (k+1));
        } // try {
        catch (const Element_Exception & Er) {
          printf("%s\n",Er.what());
          return;
        } // catch (const Element_Exception & Er) {


        // Populate Ke and check for errors
        try { Elements[Element_Index].Populate_Ke(); }
        catch (const Element_Exception & Er) {
          printf("%s\n",Er.what());
          return;
        } // catch (const Element_Exception & Er) {


        // Populate Fe and check for errors
        try { Elements[Element_Index].Populate_Fe(); }
        catch (const Element_Exception & Er) {
          printf("%s\n",Er.what());
          return;
        } // // catch (const Element_Exception & Er) {


        // Move Ke to K and check for errors
        try { Elements[Element_Index].Move_Ke_To_K(); }
        catch (const Element_Exception & Er) {
          printf("%s\n",Er.what());
          return;
        } // // catch (const Element_Exception & Er) {


        // Move Fe to F and check for errors
        try { Elements[Element_Index].Move_Fe_To_F(); }
        catch (const Element_Exception & Er) {
          printf("%s\n",Er.what());
          return;
        } // // catch (const Element_Exception & Er) {

        Element_Index++;
      } // for(unsigned k = 0; k < Nz-1; k++) {
    } // for(unsigned j = 0; j < Ny-1; j++)
  } // for(unsigned i = 0; i < Nx-1; i++)

  // Print results to file.
  Test::Print_K_To_File(K);
  Test::Print_F_To_File(F, Num_Global_Eq);


  //////////////////////////////////////////////////////////////////////////////
  // Solve for x in Kx = F.
  Pardiso_Solve(K, x, F);

  printf("x = [");
  for(unsigned i = 0; i < Num_Global_Eq; i++) { printf(" %lf", x[i]); }
  printf(" ]\n");


  //////////////////////////////////////////////////////////////////////////////
  // Report final node positions

  for(unsigned Node_Index = 0; Node_Index < Num_Nodes; Node_Index++) {
    printf("Node %d: [ ", Node_Index);
    for(unsigned Comp = 0; Comp < 3; Comp++) {
      /* If the current position is fixed (has a BC) then display the Node's
      position. Otherwise, display the result from the Pardiso solve. */
      unsigned I = ID(Node_Index, Comp);
      double Final_Position =  Nodes[Node_Index].Get_Position_Component(Comp);
      if(I == (unsigned)-1) { Final_Position += Nodes[Node_Index].Get_Displacement_Component(Comp); }
      else { Final_Position += x[I];  }

      printf("%5.2lf ", Final_Position);
    } // for(unsigned Comp = 0; Comp < 3; Comp++) {
    printf("]\n");
  } // for(unsigned Node_Index = 0; Node_Index < Num_Nodes; Node_Index++) {
} // void Test::Element(void) {





void Test::Print_K_To_File(const Matrix<double> & K, const Printing_Mode Mode) {
  // First, open a new file
  FILE * File = fopen("K.txt","w");

  // Get the number of Rows, Columns for K
  const unsigned Num_Rows = K.Get_Num_Rows();
  const unsigned Num_Cols = K.Get_Num_Cols();

  /* Note, this is not a very fast way to print K, since K is a column major
  matrix and we're printing it it in row major order. Oh well, this only runs
  once anyway */
  for(unsigned i = 0; i < Num_Rows; i++) {
    fprintf(File,"| ");
    for(unsigned j = 0; j < Num_Cols; j++) {
      if(Mode == Printing_Mode::INTEGER)
        fprintf(File,"%1.0lf ", K(i,j));
      else
        fprintf(File,"%8.1e ", K(i,j));
    } // for(unsigned j = 0; j < Num_Cols; j++) {

    fprintf(File,"|\n");
  } // for(unsigned i = 0; i < Num_Rows; i++) {

  // All done. Close the file
  fclose(File);
} // void Test::Print_K_To_File(const Matrix<double> & K) {



void Test::Print_F_To_File(const double * F, const unsigned Num_Global_Eq) {
  // First, open a new file.
  FILE * File = fopen("F.txt","w");

  // Now, print the contents of F to the file.
  fprintf(File, "| ");
  for(unsigned i = 0; i < Num_Global_Eq; i++)
    fprintf(File, "%10.3e ", F[i]);
  fprintf(File, "|");

  // All done, close the file
  fclose(File);
} // void Test::Print_F_To_File(const double * F, const unsigned Num_Global_Eq) {

#endif
