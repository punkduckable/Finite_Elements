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
  class Matrix<int> ID{Num_Nodes, 3, Memory::ROW_MAJOR};

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
          bool Has_BC = Nodes[Node_Index].Get_Has_BC(Comp);

          if(Has_BC == false) {
            ID(Node_Index, Comp) = Num_Global_Eq;

            // Increment number of equations by 1.
            Num_Global_Eq++;
          } // for(unsigned Comp = 0; Comp < 3; Comp++) {
          else { ID(Node_Index, Comp) = -1; }
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
    for(int j = 0; j < 3; j++) { printf(" %3d ", ID(i,j)); }
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
  try {
    IO::Write::K_To_File(K, IO::Write::Printing_Mode::INTEGER);
    IO::Write::F_To_File(F, Num_Global_Eq);
    IO::Write::x_To_File(F, Num_Global_Eq);
  } // try {
  catch(const Cant_Open_File & Er) { printf("%s\n",Er.what()); }

} // void Test::Element_Error_Tests(void) {





void Test::Brick_Element(void) {
  //////////////////////////////////////////////////////////////////////////////
  // Specify dimension

  // First, specify the number of nodes in each direction
  const unsigned Nx = 3;
  const unsigned Ny = 3;
  const unsigned Nz = 3;
  const unsigned Num_Nodes = Nx*Ny*Nz;
  const double INS = .1;                         // Inter-nodal spacing         Units: M


  //////////////////////////////////////////////////////////////////////////////
  // Set up Nodes.

  // Create the  array of nodes
  class Node Nodes[Num_Nodes];

  // Create the ID Array
  class Matrix<int> ID{Num_Nodes, 3, Memory::ROW_MAJOR};

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

        #ifdef TEST_MONITOR
          printf("Node %3d position [%6.3lf, %6.3lf, %6.3lf]\n", Node_Index, x_pos, y_pos, z_pos);
        #endif

        /* Constrain nodes in the yz plane in the x direction */
        if(i == 0) { Nodes[Node_Index].Set_BC_Component(0,0); }

        /* Constrain nodes in the xz plane in the y direction */
        if(j == 0) { Nodes[Node_Index].Set_BC_Component(1,0); }

        /* Constrain nodes in the xy plane in the z direction */
        if(k == 0) { Nodes[Node_Index].Set_BC_Component(2,0); }

        /* Constrain the +x face in the y and z directions */
        if(i == Nx-1) {
          Nodes[Node_Index].Set_BC_Component(1, 0);
          Nodes[Node_Index].Set_BC_Component(2, 0);
        } // if(i == Nx-1) {

        /* Stretch the +x face by 1/2 ISP */
        if(i == Nx-1) { Nodes[Node_Index].Set_Force_Component(0, .005); }
        if(i == Nx-1) { Nodes[Node_Index].Set_BC_Component(0, -INS); }

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
          bool Has_BC = Nodes[Node_Index].Get_Has_BC(Comp);

          if(Has_BC == false) {
            ID(Node_Index, Comp) = Num_Global_Eq;

            // Increment number of equations by 1.
            Num_Global_Eq++;
          } // for(unsigned Comp = 0; Comp < 3; Comp++) {
          else { ID(Node_Index, Comp) = -1; }
        } // for(unsigned Comp = 0; Comp < 3; Comp++) {

        Node_Index++;
      } // for(unsigned k = 0; k < Nz; k++) {
    } // for(unsigned j = 0; j < Ny; j++) {
  } // for(unsigned i = 0; i < Nx; i++) {

  // Print out the ID array (make sure it was set up correctly)
  #ifdef TEST_MONITOR
    printf("\nID array: \n");
    for(unsigned i = 0; i < Num_Nodes; i++) {
      printf("| ");
      for(unsigned j = 0; j < 3; j++) { printf(" %3d ", ID(i,j)); }
      printf("|\n");
    } // for(unsigned i = 0; i < Num_Nodes; i++) {
  #endif

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

  /* Cycle through the elements. For each element, supply the nodes, compute Ke (and Fe)
  and then move Ke (and Fe) into K (and F). Watch for exceptions.  */
  unsigned Element_Index = 0;
  try {
    for(unsigned i = 0; i < Nx-1; i++) {
      for(unsigned j = 0; j < Ny-1; j++) {
        for(unsigned k = 0; k < Nz-1; k++) {
          /* Set Element Nodes:
          Now, set each Element's Node list. Note, the order that we set these
          nodes is very specific. This is done so that the orientation of the nodes
          in the element matches that on page 124 of Hughes' book. */
          Elements[Element_Index].Set_Nodes(Ny*Nz*i + Nz*j + k,
                                            Ny*Nz*(i+1) + Nz*j + k,
                                            Ny*Nz*(i+1) + Nz*(j+1) + k,
                                            Ny*Nz*i + Nz*(j+1) + k,
                                            Ny*Nz*i + Nz*j + (k+1),
                                            Ny*Nz*(i+1) + Nz*j + (k+1),
                                            Ny*Nz*(i+1) + Nz*(j+1) + (k+1),
                                            Ny*Nz*i + Nz*(j+1) + (k+1));


          #ifdef TEST_MONITOR
            printf("Element %3d Node list [%3d, %3d, %3d, %3d, %3d, %3d, %3d, %3d]\n",
                   Element_Index,
                   Ny*Nz*i + Nz*j + k,
                   Ny*Nz*(i+1) + Nz*j + k,
                   Ny*Nz*(i+1) + Nz*(j+1) + k,
                   Ny*Nz*i + Nz*(j+1) + k,
                   Ny*Nz*i + Nz*j + (k+1),
                   Ny*Nz*(i+1) + Nz*j + (k+1),
                   Ny*Nz*(i+1) + Nz*(j+1) + (k+1),
                   Ny*Nz*i + Nz*(j+1) + (k+1));
          #endif

          // Populate Ke, Fe
          Elements[Element_Index].Populate_Ke();
          Elements[Element_Index].Populate_Fe();


          // Move Ke to K and Fe to F
          Elements[Element_Index].Move_Ke_To_K();
          Elements[Element_Index].Move_Fe_To_F();

          Element_Index++;
        } // for(unsigned k = 0; k < Nz-1; k++) {
      } // for(unsigned j = 0; j < Ny-1; j++)
    } // for(unsigned i = 0; i < Nx-1; i++)
  } // try {
  catch (const Element_Exception & Er) {
    printf("%s\n",Er.what());
    return;
  } // // catch (const Element_Exception & Er) {


  //////////////////////////////////////////////////////////////////////////////
  /* Now, add in the point force contributions to F. */

  for(unsigned Node_Index = 0; Node_Index < Num_Nodes; Node_Index++) {
    for(unsigned component = 0; component < 3; component++) {
      /* Check if this node's component has a force applied to it and is free
      (does not have a prescribed BC). If so then we add that force into the
      corresponding component of F. We determine the corresponding component
      using the ID array. */
      unsigned I = ID(Node_Index, component);
      double Force = Nodes[Node_Index].Get_Force_Component(component);
      if((I != (unsigned)-1) && (Force != 0) ) { F[I] += Force; }
    } // for(unsigned component = 0; component < 3; component++) {
  } // for(unsigned Node_Index = 0; Node_Index < Num_Nodes; Node_Index++) {


  //////////////////////////////////////////////////////////////////////////////
  // Solve for x in Kx = F. Write results to files.

  Pardiso_Solve(K, x, F);

  try {
    IO::Write::K_To_File(K);
    IO::Write::F_To_File(F, Num_Global_Eq);
    IO::Write::x_To_File(x, Num_Global_Eq);
  } // try {
  catch(const Cant_Open_File & Er) { printf("%s\n",Er.what()); }


  //////////////////////////////////////////////////////////////////////////////
  // Assign final displacements to the Nodes.

  /* Loop through the nodes. For each componet that is free (doesn't have a BC),
  set the node's displacement to the corresponding component of the solution x. */
  for(unsigned Node_Index = 0; Node_Index < Num_Nodes; Node_Index++) {
    for(unsigned Comp = 0; Comp < 3; Comp++) {
      int I = ID(Node_Index, Comp);

      /* If this Node's component was free (I != -1) then we assign this
      componnet of this node's displacement to the corresponding component of x */
      if(I != -1) { Nodes[Node_Index].Set_Displacement_Component(Comp, x[I]); }
    } // for(unsigned Comp = 0; Comp < 3; Comp++) {
  } // for(unsigned Node_Index = 0; Node_Index < Num_Nodes; Node_Index++) {


  //////////////////////////////////////////////////////////////////////////////
  // Finally, print the results to a .vtk file.

  IO::Write::vtk(Nodes, Num_Nodes, Elements, Num_Elements);
} // void Test::Brick_Element(void) {



void Test::Wedge_Element(void) {
  /* Function description:
  Thsi function is designed to test the Wedge elements. This is done by making
  a sort of trianglular prism of wedge shaped elements. A cross section of
  this prism looks likes the following (where each verticy is a node),


                       /\
                      /  \
                     /\  /\
    y               /  \/  \
    ^              /\  /\  /\
    |             /  \/  \/  \
    |            /\  /\  /\  /\
    |----> x    /  \/  \/  \/  \

  A force is applied to the top of this prism while the base is clamped. */

  //////////////////////////////////////////////////////////////////////////////
  // Specify dimension.
  // To define the prism, we need to specify the number of layers (depth) and
  // number of nodes in the base.

  const unsigned N_Depth = 5;
  const unsigned N_Base = 10;
  const unsigned Nodes_Per_Depth_Layer = ((N_Base)*(N_Base+1))/2;
  const unsigned Num_Nodes = N_Depth*Nodes_Per_Depth_Layer;

  const double INS = .1;                         // Inter-nodal spacing         Units: M
  /* Note: For simplicity, this is not the literal distance between adjacent
  nodes. Rather, it is the distance between successive layers of nodes in the y,
  x, and z directions */


  //////////////////////////////////////////////////////////////////////////////
  // Set up Nodes.

  // Create the  array of nodes
  class Node Nodes[Num_Nodes];

  // Create the ID Array
  class Matrix<int> ID{Num_Nodes, 3, Memory::ROW_MAJOR};

  // Set up Node positions and BC's
  unsigned Node_Index = 0;
  for(unsigned depth = 0; depth < N_Depth; depth++) {
    double z_pos = INS*depth;
    for(unsigned layer = 0; layer < N_Base; layer++) {
      double y_pos = INS*layer;
      for(unsigned i = 0; i < (N_Base-layer); i++) {
        double x_pos = layer*(INS/2) + INS*i;    // Think about it. See picture above

        // Set the Node's position
        Nodes[Node_Index].Set_Position_Component(0, x_pos);
        Nodes[Node_Index].Set_Position_Component(1, y_pos);
        Nodes[Node_Index].Set_Position_Component(2, z_pos);

        #ifdef TEST_MONITOR
          printf("Node %3d position [%6.3lf, %6.3lf, %6.3lf]\n", Node_Index, x_pos, y_pos, z_pos);
        #endif

        /* Clamp the base. */
        if(layer == 0) {
          Nodes[Node_Index].Set_BC_Component(0,0);
          Nodes[Node_Index].Set_BC_Component(1,0);
          Nodes[Node_Index].Set_BC_Component(2,0);
        } // if(layer == 0) {

        /* Apply a force to the top of the prism. */
        if(layer == (N_Base-1)) {
          double Force = .05;
          if(depth == N_Depth - 1 || depth == 0) { Nodes[Node_Index].Set_Force_Component(1, Force/2.);}
          else { Nodes[Node_Index].Set_Force_Component(1, .05); }
        } // if(layer == (N_Base-1)) {

        Node_Index++;
      } // for(unsigned i = 0; i < (N_Base-layer); i++) {
    } // for(unsigned layer = 0; layer < N_Base; layer++) {
  } // for(unsigned depth = 0; depth < N_Depth; depth++) {

  // Populate ID array
  unsigned Num_Global_Eq = 0;
  for(unsigned Node_Index = 0; Node_Index < Num_Nodes; Node_Index++) {
    /* Cycle through the components of the current node. Use this info
    to populate ID. If the current component is not being used, fill that
    cell of ID with a -1 */
    for(unsigned Comp = 0; Comp < 3; Comp++) {
      bool Has_BC = Nodes[Node_Index].Get_Has_BC(Comp);

      if(Has_BC == false) {
        ID(Node_Index, Comp) = Num_Global_Eq;

        // Increment number of equations by 1.
        Num_Global_Eq++;
      } // for(unsigned Comp = 0; Comp < 3; Comp++) {
      else { ID(Node_Index, Comp) = -1; }
    } // for(unsigned Comp = 0; Comp < 3; Comp++) {
  } // for(unsigned Node_Index = 0; Node_Index < Num_Nodes; Node_Index++) {

  // Print out the ID array (make sure it was set up correctly)
  #ifdef TEST_MONITOR
    printf("\nID array: \n");
    for(unsigned i = 0; i < Num_Nodes; i++) {
      printf("| ");
      for(unsigned j = 0; j < 3; j++) { printf(" %3d ", ID(i,j)); }
      printf("|\n");
    } // for(unsigned i = 0; i < Num_Nodes; i++) {
  #endif

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
  const unsigned Num_Elements = (N_Base-1)*(N_Base-1)*(N_Depth-1);             // Think about it. Number of elements per depth layer turns out to be (N_Base-1)^2
  class Element Elements[Num_Elements];


  //////////////////////////////////////////////////////////////////////////////
  // Find K, F

  /* Cycle through the elements. For each element, supply the nodes, compute Ke (and Fe)
  and then move Ke (and Fe) into K (and F). Watch for exceptions.  */
  unsigned Element_Index = 0;
  try {
    for(unsigned depth = 0; depth < N_Depth-1; depth++) {
      for(unsigned layer = 0; layer < N_Base-1; layer++) {
        unsigned nodes_before_current_layer = 0;
        for(unsigned j = 0; j < layer; j++) {  nodes_before_current_layer += N_Base-j; }
        unsigned nodes_after_current_layer = nodes_before_current_layer + (N_Base - layer);

        for(unsigned i = 0; i < 2*(N_Base-1-layer) - 1; i++) {
          /* Set Element Nodes:
          Now, set each Element's Node list. Note, the order that we set these
          nodes is very specific. This is done so that the orientation of the nodes
          in the element matches that on page 124 of Hughes' book.

          It's tricky to figure out which nodes that correspond to a particular
          element. Just spend some time thinking about it. */

          if(i%2 == 0) {
            unsigned i_low_1  = i/2;
            unsigned i_low_2  = i_low_1+1;
            unsigned i_high_1 = i_low_1;

            Elements[Element_Index].Set_Nodes(Nodes_Per_Depth_Layer*depth     + nodes_before_current_layer + i_low_1,
                                              Nodes_Per_Depth_Layer*depth     + nodes_before_current_layer + i_low_2,
                                              Nodes_Per_Depth_Layer*depth     + nodes_after_current_layer  + i_high_1,
                                              Nodes_Per_Depth_Layer*(depth+1) + nodes_before_current_layer + i_low_1,
                                              Nodes_Per_Depth_Layer*(depth+1) + nodes_before_current_layer + i_low_2,
                                              Nodes_Per_Depth_Layer*(depth+1) + nodes_after_current_layer  + i_high_1);

            #ifdef TEST_MONITOR
              printf("Element %3d Node list [%3d, %3d, %3d, %3d, %3d, %3d]\n",
                     Element_Index,
                     Nodes_Per_Depth_Layer*depth     + nodes_before_current_layer + i_low_1,
                     Nodes_Per_Depth_Layer*depth     + nodes_before_current_layer + i_low_2,
                     Nodes_Per_Depth_Layer*depth     + nodes_after_current_layer  + i_high_1,
                     Nodes_Per_Depth_Layer*(depth+1) + nodes_before_current_layer + i_low_1,
                     Nodes_Per_Depth_Layer*(depth+1) + nodes_before_current_layer + i_low_2,
                     Nodes_Per_Depth_Layer*(depth+1) + nodes_after_current_layer  + i_high_1);
            #endif
          } // if(i%2 == 0) {

          else {
            unsigned i_low_1  = (i+1)/2;
            unsigned i_high_1 = i_low_1 - 1;
            unsigned i_high_2 = i_low_1;

            Elements[Element_Index].Set_Nodes(Nodes_Per_Depth_Layer*depth     + nodes_after_current_layer  + i_high_2,
                                              Nodes_Per_Depth_Layer*depth     + nodes_after_current_layer  + i_high_1,
                                              Nodes_Per_Depth_Layer*depth     + nodes_before_current_layer + i_low_1,
                                              Nodes_Per_Depth_Layer*(depth+1) + nodes_after_current_layer  + i_high_2,
                                              Nodes_Per_Depth_Layer*(depth+1) + nodes_after_current_layer  + i_high_1,
                                              Nodes_Per_Depth_Layer*(depth+1) + nodes_before_current_layer + i_low_1);


            #ifdef TEST_MONITOR
              printf("Element %3d Node list [%3d, %3d, %3d, %3d, %3d, %3d]\n",
                     Element_Index,
                     Nodes_Per_Depth_Layer*depth     + nodes_after_current_layer  + i_high_2,
                     Nodes_Per_Depth_Layer*depth     + nodes_after_current_layer  + i_high_1,
                     Nodes_Per_Depth_Layer*depth     + nodes_before_current_layer + i_low_1,
                     Nodes_Per_Depth_Layer*(depth+1) + nodes_after_current_layer  + i_high_2,
                     Nodes_Per_Depth_Layer*(depth+1) + nodes_after_current_layer  + i_high_1,
                     Nodes_Per_Depth_Layer*(depth+1) + nodes_before_current_layer + i_low_1);
            #endif
          } // else {


          // Populate Ke, Fe
          Elements[Element_Index].Populate_Ke();
          Elements[Element_Index].Populate_Fe();


          // Move Ke to K and Fe to F
          Elements[Element_Index].Move_Ke_To_K();
          Elements[Element_Index].Move_Fe_To_F();

          Element_Index++;
        } // for(unsigned i = 0; i < 2*(N_Base-1-layer) - 1; i++) {
      } // for(unsigned layer = 0; layer < N_Base-1; layer++) {
    } // for(unsigned depth = 0; depth < N_Depth-1; depth++) {
  } // try {
  catch (const Element_Exception & Er) {
    printf("%s\n",Er.what());
    return;
  } // // catch (const Element_Exception & Er) {


  //////////////////////////////////////////////////////////////////////////////
  /* Now, add in the point force contributions to F. */

  for(unsigned Node_Index = 0; Node_Index < Num_Nodes; Node_Index++) {
    for(unsigned component = 0; component < 3; component++) {
      /* Check if this node's component has a force applied to it and is free
      (does not have a prescribed BC). If so then we add that force into the
      corresponding component of F. We determine the corresponding component
      using the ID array. */
      unsigned I = ID(Node_Index, component);
      double Force = Nodes[Node_Index].Get_Force_Component(component);
      if((I != (unsigned)-1) && (Force != 0) ) { F[I] += Force; }
    } // for(unsigned component = 0; component < 3; component++) {
  } // for(unsigned Node_Index = 0; Node_Index < Num_Nodes; Node_Index++) {

  //////////////////////////////////////////////////////////////////////////////
  // Solve for x in Kx = F. Write results to files.

  Pardiso_Solve(K, x, F);

  try {
    IO::Write::K_To_File(K);
    IO::Write::F_To_File(F, Num_Global_Eq);
    IO::Write::x_To_File(x, Num_Global_Eq);
  } // try {
  catch(const Cant_Open_File & Er) { printf("%s\n",Er.what()); }


  //////////////////////////////////////////////////////////////////////////////
  // Assign final displacements to the Nodes.

  /* Loop through the nodes. For each componet that is free (doesn't have a BC),
  set the node's displacement to the corresponding component of the solution x. */
  for(unsigned Node_Index = 0; Node_Index < Num_Nodes; Node_Index++) {
    for(unsigned Comp = 0; Comp < 3; Comp++) {
      int I = ID(Node_Index, Comp);

      /* If this Node's component was free (I != -1) then we assign this
      componnet of this node's displacement to the corresponding component of x */
      if(I != -1) { Nodes[Node_Index].Set_Displacement_Component(Comp, x[I]); }
    } // for(unsigned Comp = 0; Comp < 3; Comp++) {
  } // for(unsigned Node_Index = 0; Node_Index < Num_Nodes; Node_Index++) {


  //////////////////////////////////////////////////////////////////////////////
  // Finally, print the results to a .vtk file.

  IO::Write::vtk(Nodes, Num_Nodes, Elements, Num_Elements);
} // void Test::Wedge_Element(void) {

#endif
