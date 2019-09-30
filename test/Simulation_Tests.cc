#if !defined(SIMULATION_TESTS_SOURCE)
#define SIMULATION_TESTS_SOURCE

#include "Simulation_Tests.h"

void Test::Mrudang_Test(void) {
  /* First, read in the inp file. */
  std::list<Array<double, 3>> Node_Positions;
  std::list<Array<unsigned, 8>> Element_Node_Lists;
  std::list<IO::Read::inp_boundary_data> Boundary_List;
  std::string File_Name = "Job-1.inp";

  IO::Read::inp(File_Name, Node_Positions, Element_Node_Lists, Boundary_List);

  printf("Read in %u nodes\n",    (unsigned)Node_Positions.size());
  printf("Read in %u elements\n", (unsigned)Element_Node_Lists.size());


  //////////////////////////////////////////////////////////////////////////////
  /* Next, let's process the Node_Positions and Boundary lists into a Nodes
  array */
  const unsigned Num_Nodes = (unsigned)Node_Positions.size();
  class Node* Nodes = Simulation::Process_Node_Lists(Node_Positions, Boundary_List, Num_Nodes);


  //////////////////////////////////////////////////////////////////////////////
  /* Read in each node set and apply the corresponding BC's */

  std::list<unsigned> Node_Set_List;

  /* Node set 1:
  x and y coordinates fixed. */
  IO::Read::node_set(File_Name, Node_Set_List, "NSET1");

  class IO::Read::nset_BC NSET1_BC{};
  NSET1_BC.Set_x_BC(0);
  NSET1_BC.Set_y_BC(0);
  Simulation::Set_nset_BCs(Nodes, Node_Set_List, NSET1_BC);


  /* Node set 2:
  x coordinate fixed. */
  IO::Read::node_set(File_Name, Node_Set_List, "NSET2");

  class IO::Read::nset_BC NSET2_BC{};
  NSET2_BC.Set_x_BC(0);
  Simulation::Set_nset_BCs(Nodes, Node_Set_List, NSET2_BC);


  /* Node set 3:
  y coordinate fixed. */
  IO::Read::node_set(File_Name, Node_Set_List, "NSET3");

  class IO::Read::nset_BC NSET3_BC{};
  NSET3_BC.Set_y_BC(0);
  Simulation::Set_nset_BCs(Nodes, Node_Set_List, NSET3_BC);


  /* Node set 4:
  x, y, and z coordinates fixed. */
  IO::Read::node_set(File_Name, Node_Set_List, "NSET4");

  class IO::Read::nset_BC NSET4_BC{};
  NSET4_BC.Set_x_BC(0);
  NSET4_BC.Set_y_BC(0);
  NSET4_BC.Set_z_BC(0);
  Simulation::Set_nset_BCs(Nodes, Node_Set_List, NSET4_BC);


  /* Node set 5:
  x and z coordinates fixed. */
  IO::Read::node_set(File_Name, Node_Set_List, "NSET5");

  class IO::Read::nset_BC NSET5_BC{};
  NSET5_BC.Set_x_BC(0);
  NSET5_BC.Set_z_BC(0);
  Simulation::Set_nset_BCs(Nodes, Node_Set_List, NSET5_BC);


  /* Node set 6:
  y and z coordinates fixed. */
  IO::Read::node_set(File_Name, Node_Set_List, "NSET6");

  class IO::Read::nset_BC NSET6_BC{};
  NSET6_BC.Set_y_BC(0);
  NSET6_BC.Set_z_BC(0);
  Simulation::Set_nset_BCs(Nodes, Node_Set_List, NSET6_BC);


  /* Node set 7:
  z coordinate fixed. */
  IO::Read::node_set(File_Name, Node_Set_List, "NSET7");

  class IO::Read::nset_BC NSET7_BC{};
  NSET7_BC.Set_z_BC(0);
  Simulation::Set_nset_BCs(Nodes, Node_Set_List, NSET7_BC);


  /* Node set 8:
  x and y coordinates fixed, z displaced by 5 */
  IO::Read::node_set(File_Name, Node_Set_List, "NSET8");

  class IO::Read::nset_BC NSET8_BC{};
  NSET8_BC.Set_x_BC(0);
  NSET8_BC.Set_y_BC(0);
  NSET8_BC.Set_z_BC(5);
  Simulation::Set_nset_BCs(Nodes, Node_Set_List, NSET8_BC);


  /* Node set 9:
  x coordinate fixed, z displaced by 5 */
  IO::Read::node_set(File_Name, Node_Set_List, "NSET9");

  class IO::Read::nset_BC NSET9_BC{};
  NSET9_BC.Set_x_BC(0);
  NSET9_BC.Set_z_BC(5);
  Simulation::Set_nset_BCs(Nodes, Node_Set_List, NSET9_BC);


  /* Node set 10:
  y coordinate fixed, z displaced by 5 */
  IO::Read::node_set(File_Name, Node_Set_List, "NSET10");

  class IO::Read::nset_BC NSET10_BC{};
  NSET10_BC.Set_y_BC(0);
  NSET10_BC.Set_z_BC(5);
  Simulation::Set_nset_BCs(Nodes, Node_Set_List, NSET10_BC);


  /* Node set 11:
  z displaced by 5 */
  IO::Read::node_set(File_Name, Node_Set_List, "NSET11");

  class IO::Read::nset_BC NSET11_BC{};
  NSET11_BC.Set_z_BC(5);
  Simulation::Set_nset_BCs(Nodes, Node_Set_List, NSET11_BC);


  //////////////////////////////////////////////////////////////////////////////
  /* Now populate the ID array and find the number of global equations */
  class Matrix<int> ID{Num_Nodes, 3, Memory::ROW_MAJOR};
  unsigned Num_Global_Eq = Simulation::SetUp_ID_Num_Global_Eq(ID, Nodes, Num_Nodes);


  //////////////////////////////////////////////////////////////////////////////
  /* With this information, we can now allocate K F, and x */
  class Matrix<double> K{Num_Global_Eq, Num_Global_Eq, Memory::COLUMN_MAJOR};
  double* F = new double[Num_Global_Eq];
  double* x = new double[Num_Global_Eq];

  // Zero initialize K and F
  K.Zero();
  for(unsigned i = 0; i < Num_Global_Eq; i++) { F[i] = 0; }


  //////////////////////////////////////////////////////////////////////////////
  /* Now that we have K, F, and Nodes, we can set up the element class static
  members */

  try {
    Set_Element_Static_Members(&ID, &K, F, Nodes);
    Set_Element_Material(Simulation::E, Simulation::v);
  } // try {
  catch (const Element_Exception & Er) {
    printf("%s\n",Er.what());
    return;
  } // catch ((const Element_Exception & Er) {


  //////////////////////////////////////////////////////////////////////////////
  /* Allocate the elements Array.
  Note: This will populate Ke and Fe for each element */

  const unsigned Num_Elements = (unsigned)Element_Node_Lists.size();
  class Element* Elements = Simulation::Process_Element_List(Element_Node_Lists, Num_Elements);


  //////////////////////////////////////////////////////////////////////////////
  /* Now, find F and K
  Note: we could have done this when we processed the Element's list. I choose
  to do it afterward becuase I think it makes more sense that way. */

  try {
    for(unsigned Element_Index = 0; Element_Index < Num_Elements; Element_Index++) {
      Elements[Element_Index].Move_Ke_To_K();
      Elements[Element_Index].Move_Fe_To_F();
    } // for(unsigned Element_Index = 0; Element_Index < Num_Elements; Element_Index++) {
  } // try {
  catch (const Element_Exception & Er) {
    printf("%s\n",Er.what());
    throw;
  } // catch (const Element_Exception & Er) {


  //////////////////////////////////////////////////////////////////////////////
  /* Solve for x in Kx = F. */
  Pardiso_Solve(K, x, F);

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
  /* Output/display results. */

  IO::Write::vtk(Nodes, Num_Nodes, Elements, Num_Elements);

  return;
} // void Test::Mrudang_Test(void) {

#endif
