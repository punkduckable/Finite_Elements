#if !defined(SIMULATION_SOURCE)
#define SIMULATION_SOURCE

#include "Simulation.h"

void Simulation::From_File(const std::string & File_Name) {
  /* First, read in the inp file. */
  std::list<Array<double, 3>> Node_Positions;
  std::list<Array<unsigned, 8>> Element_Node_Lists;
  std::list<IO::Read::inp_boundary_data> Boundary_List;
  std::list<unsigned> Node_Set_List;

  IO::Read::inp(File_Name, Node_Positions, Element_Node_Lists, Boundary_List, Node_Set_List);

  #ifdef INPUT_MONITOR
    printf("Read in %u nodes\n",    (unsigned)Node_Positions.size());
    printf("Read in %u elements\n", (unsigned)Element_Node_Lists.size());
  #endif


  //////////////////////////////////////////////////////////////////////////////
  /* Next, let's process the Node_Positions and Boundary lists into a Nodes
  array */
  const unsigned Num_Nodes = (unsigned)Node_Positions.size();
  class Node* Nodes = Process_Node_Lists(Node_Positions, Boundary_List, Node_Set_List, Num_Nodes);


  //////////////////////////////////////////////////////////////////////////////
  /* Now populate the ID array and find the number of global equations */
  class Matrix<int> ID{Num_Nodes, 3, Memory::ROW_MAJOR};
  unsigned Num_Global_Eq = SetUp_ID_Num_Global_Eq(ID, Nodes, Num_Nodes);


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
  class Element* Elements = Process_Element_List(Element_Node_Lists, Num_Elements);


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

  #if defined(SIMULATION_MONITOR)
    // Print K, F, x to file
    try {
      IO::Write::K_To_File(K);
      IO::Write::F_To_File(F, Num_Global_Eq);
      IO::Write::x_To_File(x, Num_Global_Eq);
    } // try {
    catch(const Cant_Open_File & Er) { printf("%s\n",Er.what()); }

    for(unsigned Node_Index = 0; Node_Index < Num_Nodes; Node_Index++) {
      printf("Node %d: [ ", Node_Index);
      for(unsigned Comp = 0; Comp < 3; Comp++) {
        /* If the current position is fixed (has a BC) then display the Node's
        position. Otherwise, display the result from the Pardiso solve. */
        int I = ID(Node_Index, Comp);
        double Final_Position =  Nodes[Node_Index].Get_Position_Component(Comp);

        if(I == -1) { Final_Position += Nodes[Node_Index].Get_Displacement_Component(Comp); }
        else { Final_Position += x[I];  }

        printf("%6.3lf ", Final_Position);
      } // for(unsigned Comp = 0; Comp < 3; Comp++) {
      printf("]\n");
    } // for(unsigned Node_Index = 0; Node_Index < Num_Nodes; Node_Index++) {
  #endif
} // void Simulation::From_File(const std::string & File_Name) {





class Node* Simulation::Process_Node_Lists(class std::list<Array<double,3>> & Node_Positions, class std::list<IO::Read::inp_boundary_data> & Boundary_List, class std::list<unsigned> & Node_Set_List, const unsigned Num_Nodes) {
  /* Function description:
  This function uses the Node_Positions and Boundary_List lists to create the
  Nodes array. */

  /* First, allocate the Nodes array */
  Node* Nodes = new Node[Num_Nodes];

  /* Now, one by one, pop the positions off of the Node_List and move them into
  the Nodes array. */
  for(unsigned Node_Index = 0; Node_Index < Num_Nodes; Node_Index++) {
    /* Get the front Node position. Use it to set up the current Node */
    Array<double, 3> Current_Node_Position = Node_Positions.front();

    Nodes[Node_Index].Set_Position_Component(0, Current_Node_Position[0]);
    Nodes[Node_Index].Set_Position_Component(1, Current_Node_Position[1]);
    Nodes[Node_Index].Set_Position_Component(2, Current_Node_Position[2]);

    /* Pop the top Node Position. */
    Node_Positions.pop_front();
  } // for(unsigned Node_Index = 0; Node_Index < Num_Nodes; Node_Index++) {


  /* Now let's apply the BC's */
  unsigned Num_BC = Boundary_List.size();
  for(unsigned i = 0; i < Num_BC; i++) {
    /* Get the front element Boundary_List, use this to implement the current BC */
    struct IO::Read::inp_boundary_data Current_BC = Boundary_List.front();

    /* Convert the Start_DOF and End_DOF from 1-index to 0-index*/
    unsigned Start_DOF = Current_BC.Start_DOF - 1;
    unsigned End_DOF = Current_BC.End_DOF - 1;

    /* Set all DOF's to the specified value*/
    for(unsigned j = Start_DOF; j < End_DOF; j++) {
      Nodes[Current_BC.Node_Number].Set_BC_Component(j, Current_BC.displacement);
    } // for(unsigned j = Start_DOF; j < End_DOF; j++) {

    /* Pop the front element of the Boundary List. */
    Boundary_List.pop_front();
  } // for(unsigned i = 0; i < Num_BC; i++) {


  /* Now, let's fix the nodes in the Node Set. */
  unsigned Num_Nset = Node_Set_List.size();
  for(unsigned i = 0; i < Num_Nset; i++) {
    /* Get the front element of Node_Set_List. */
    unsigned Current_Node = Node_Set_List.front();

    /* Fix the position of this Node.  */
    Nodes[Current_Node].Set_BC_Component(0, 0);
    Nodes[Current_Node].Set_BC_Component(1, 0);
    Nodes[Current_Node].Set_BC_Component(2, 0);

    /* Pop the front element off of the Node_Set_List */
    Node_Set_List.pop_front();
  } // for(unsigned i = 0; i < Num_Nset; i++) {

  return Nodes;
} // class Node* Process_Node_Lists(class std::list<Array<double,3>> & Node_Positions,...





unsigned Simulation::SetUp_ID_Num_Global_Eq(class Matrix<int> & ID, const Node* Nodes, const unsigned Num_Nodes) {
  /* Function description:
  This function is used to set up the ID matrix and to find the number of
  global equations (which is returned) */

  /* Cycle through the nodes in the Nodes array. For each node, cycle through
  its components. For each component, if it's free, add it's ID to the
  corresponding component of the ID array and increment the number of global
  equations by 1. Otherwise, (if it's fixed) set the corresponding component of
  ID to -1. */
  unsigned Num_Global_Eq = 0;
  for(unsigned Node_Index = 0; Node_Index < Num_Nodes; Node_Index++) {
    for(unsigned Comp = 0; Comp < 3; Comp++) {
      bool Has_BC = Nodes[Node_Index].Get_Has_BC(Comp);

      if(Has_BC == false) {
        ID(Node_Index, Comp) = Num_Global_Eq;
        Num_Global_Eq++;
      } // for(unsigned Comp = 0; Comp < 3; Comp++) {
      else { ID(Node_Index, Comp) = -1; }
    } // for(unsigned Comp = 0; Comp < 3; Comp++) {
  } // for(unsigned Node_Index = 0; Node_Index < Num_Nodes; Node_Index++) {

  #if defined(ID_MONITOR)
    // Print out the ID array (make sure it was set up correctly)
    printf("\nID array: \n");
    for(unsigned i = 0; i < Num_Nodes; i++) {
      printf("| ");
      for(unsigned j = 0; j < 3; j++)  { printf(" %3d ", ID(i,j)); }
      printf("|\n");
    } // for(unsigned i = 0; i < Num_Nodes; i++) {
  #endif

  return Num_Global_Eq;
} // unsigned Simulation::SetUp_ID_Num_Global_Eq(class Matrix<int> & ID, const Node* Nodes) {



class Element* Simulation::Process_Element_List(class list<Array<unsigned, 8>> & Element_Node_Lists, const unsigned Num_Elements) {
  /* Function description:
  This function uses the Elemnet_Node_Lists list to create the Element array. */

  /* First, allocate the Elements array */
  Element* Elements = new Element[Num_Elements];

  /* Now, one by one, pop the node lists off of the Element_Node_Lists. Use
  these lists to set each element's node list. */
  try {
    for(unsigned Element_Index = 0; Element_Index < Num_Elements; Element_Index++) {
      /* Get the front Nodes list, use it to set up the current Element. */
      Array<unsigned, 8> Current_Element_Node_List = Element_Node_Lists.front();
      Elements[Element_Index].Set_Nodes(Current_Element_Node_List[0],
                                        Current_Element_Node_List[1],
                                        Current_Element_Node_List[2],
                                        Current_Element_Node_List[3],
                                        Current_Element_Node_List[4],
                                        Current_Element_Node_List[5],
                                        Current_Element_Node_List[6],
                                        Current_Element_Node_List[7]);


      // Populate Ke and Fe.
      Elements[Element_Index].Populate_Ke();
      Elements[Element_Index].Populate_Fe();

      /* Pop the top Element Node List */
      Element_Node_Lists.pop_front();
    } // for(unsigned Element_Index = 0; Element_Index < Num_Elements; Element_Index++) {
  } // try {
  catch (const Element_Exception & Er) {
    printf("%s\n",Er.what());
    throw;
  } // catch (const Element_Exception & Er) {

  return Elements;
} // class Element* Simulation::Process_Element_List(class list<Array<unsigned, 8>> & Element_Node_Lists,...

#endif
