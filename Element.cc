#if !defined(ELEMENT_SOURCE)
#define ELEMENT_SOURCE

#include "Element.h"
#include <stdio.h>
#define ELEMENT_MONITOR

using namespace Element_Errors;

////////////////////////////////////////////////////////////////////////////////
// Set Elements static members

bool Element::Static_Members_Set = false;
Node * Element::Node_Array = NULL;
Matrix<int> * Element::ID;
Matrix<double> * Element::K;
double (*Element::F)(unsigned, unsigned, unsigned, unsigned);





////////////////////////////////////////////////////////////////////////////////
// Destructor

Element::~Element(void) {
  /* Check if nodes are set. If so then this element dynamically allocated
  memory for Local_Eq_Num_To_Node, Local_Eq_Num_To_Global_Eq_Num, and Ke. We
  need to free this memory before deleting the object. */
  if(Element_Set_Up) {
    delete [] Local_Eq_Num_To_Node;
    delete [] Local_Eq_Num_To_Global_Eq_Num;
  } // if(Element_Set_Up) {
} // Element::~Element(void) {





////////////////////////////////////////////////////////////////////////////////
// Other Element methods

// Set nodes
Errors Element::Set_Nodes(const unsigned Node0_ID,
                          const unsigned Node1_ID,
                          const unsigned Node2_ID,
                          const unsigned Node3_ID,
                          const unsigned Node4_ID,
                          const unsigned Node5_ID,
                          const unsigned Node6_ID,
                          const unsigned Node7_ID) {

  /* Assumption 1:
  This function assumes that the Element class has been set up.
  The element class contains a few static members. One of these, the ID array,
  points to the global ID array. Another, Node_Array, points to the
  array of nodes used in this simulation. We need to have access to both of
  these arrays to be able to set up the Element. Therefore, if these static
  members have not been set and the user is trying to set this Element's nodes
  then we need to thrown an error.

  When the static members are set, a flag called "Static_Members_Set" gets
  flipped to true. Therefore, if this flag is true then the assumption is
  satisified.  */
  if(Static_Members_Set == false) {
    printf("Error in void Element::Set_Nodes!\n");
    printf("To set up an element, the ID and Node_Array static members must be set\n");
    printf("Currently, ID = %p and Node_Array = %p.\n", ID, Node_Array);

    return STATIC_MEMBERS_NOT_SET;
  } // if(ID == NULL || Node_Array == NULL) {

  /* Assumptions 2:
  This function also assumes that this specific element has not has its nodes
  set already. */
  if(Element_Set_Up == true)
    return ELEMENT_ALREADY_SET_UP;


  /* If we've made it this far then the Element class has been set up. We can
  therefore set the nodes.
  To begin, set the Node_List using the passed Node_ID's */
  Node_List[0] = Node0_ID;
  Node_List[1] = Node1_ID;
  Node_List[2] = Node2_ID;
  Node_List[3] = Node3_ID;
  Node_List[4] = Node4_ID;
  Node_List[5] = Node5_ID;
  Node_List[6] = Node6_ID;
  Node_List[7] = Node7_ID;


  //////////////////////////////////////////////////////////////////////////////
  /* Determine the number of local equations. To do this, we cycle through
  each node in the node list. We access the actual node using the Node_Array
  For each node, we check if it has a fixed component of position */
  Num_Local_Eq = 0;
  for(int Node = 0; Node < 8; Node++) {
    const unsigned Global_Node_Number = Node_List[Node];

    for(int Component = 0; Component < 3; Component++) {
      // Check the current node has a fixed component of position in the Component direction
      if(Node_Array[Global_Node_Number].Fixed_Pos[Component] == false)
        Num_Local_Eq++;
    } // for(int Component = 0; Component < 3; Component++) {
  } // for(int Node = 0; Node < 8; Node++) {


  //////////////////////////////////////////////////////////////////////////////
  // Allocate Local_Eq_Num_To_Node, Local_Eq_Num_To_Global_Eq_Num, and Ke

  Local_Eq_Num_To_Node = new unsigned[2*Num_Local_Eq];
  Local_Eq_Num_To_Global_Eq_Num = new unsigned[Num_Local_Eq];
  Ke.Set_Up(Num_Local_Eq, Num_Local_Eq, Memory::COLUMN_MAJOR);

  //////////////////////////////////////////////////////////////////////////////
  // Set up Local_Eq_Num_To_Node, Local_Eq_Num_To_Global_Eq_Num

  unsigned Eq_Num = 0;
  for(int Node = 0; Node < 8; Node++) {
    const unsigned Global_Node_Number = Node_List[Node];

    for(int Component = 0; Component < 3; Component++) {
      /* If a particular component of a node's position is fixed then that
      component of position is already known. Therefore, there is no equation
      for that component. Therefore, that component should not enter Ke.

      If a node's component is fixed, then its corresponding cell in ID will
      be -1. If not, then we need this component's global equation number
      (which, luckily, is also stored in ID). Therefore, we can use the ID array
      to determine if we should skip this component. */
      int Global_Eq_Number = (*ID)(Global_Node_Number, Component);

      /* if the equation number that we got is -1, then this component is fixed,
      skip it */
      if(Global_Eq_Number != -1) {
        // Set Local_Eq_Num_To_Global_Eq_Num
        Local_Eq_Num_To_Global_Eq_Num[Eq_Num] = Global_Eq_Number;

        // Set Local_Eq_Num_To_Node
        Local_Eq_Num_To_Node[0 + Eq_Num*2] = Global_Node_Number;
        Local_Eq_Num_To_Node[1 + Eq_Num*2] = Component;

        // Increment equation number
        Eq_Num++;
      } // if(Global_Eq_Number != -1) {
    } // for(int Component = 0; Component < 3 Component++) {
  } // for(int Node = 0; Node < 8; Node++) {

  // The element is now set up
  Element_Set_Up = true;


  #if defined(ELEMENT_MONITOR)
    printf("Node list: ");
    for(int i = 0; i < 8; i++)
      printf("%u ", Node_List[i]);

    printf("\n");


    printf("Local_Eq_Num_To_Global_Eq_Num: ");
    for(int i = 0; i < Num_Local_Eq; i++)
      printf("%u ", Local_Eq_Num_To_Global_Eq_Num[i]);

    printf("\n");


    printf("Local_Eq_Num_To_Node:\n");
    for(int j = 0; j < 2; j++) {
      printf("| ");
      for(int i = 0; i < Num_Local_Eq; i++)
        printf("%3u ", Local_Eq_Num_To_Node[i*2 + j]);

      printf("|\n");
    } // for(int j = 0; j < 2; j++) {
  #endif



  return SUCCESS;
} // Errors Element::Set_Nodes(const unsigned Node1_ID,



// Populate Ke
Errors Element::Populate_Ke(void) {
  /* Assumption 1:
  This function assumes that this Element has been set up. More specificially,
  this function assumes that the node list has been set, that Ke has been
  allocated, that Local_Eq_Num_To_Node has been allocated and set up, and that
  Local_Eq_Num_To_Global_Eq_Num has been allocated and set up.

  This function also assumes that the Element class has been set up. More
  specitically, this function assumes that the F (integrating function) has
  been set.

  Importantly, however, the Element can not be set up until the static members
  are set. Therefore, if the Element is set up then both assumptions must be
  valid. */
  if(Element_Set_Up == false)
    return ELEMENT_NOT_SET_UP;

  /* Assumption 2:
  This function also assumes that Ke has not been set already. */
  if(Ke_Set_Up == true)
    return KE_ALREADY_SET_UP;

  // Populate the diagional + lower triangular elements of Ke
  for(int Col = 0; Col < Num_Local_Eq; Col++) {
    /* Get ID and Component associated with the Col'th local equation. It should
    be noted that this array is stored in Row major order and that its
    dimensions are Num_Local_Eq x 2. Therefore, the ID for the ith Node is at
    2*i while the component is at 2*i + 1 */
    const unsigned Node_A_ID = Local_Eq_Num_To_Node[Col*2 + 0];
    const unsigned Node_A_Component = Local_Eq_Num_To_Node[Col*2 + 1];

    for(int Row = Col; Row < Num_Local_Eq; Row++) {
      /* Get ID and Component associated with the Row'th local equation. */
      const unsigned Node_B_ID = Local_Eq_Num_To_Node[Row*2 + 0];
      const unsigned Node_B_Component = Local_Eq_Num_To_Node[Row*2 + 1];

      // Populate Ke
      Ke(Row, Col) = F(Node_A_ID, Node_A_Component, Node_B_ID, Node_B_Component);
    } // for(int Row = Col; Row < Num_Local_Eq; Row++) {
  } // for(int Col = 0; Col < Num_Local_Eq; Col++) {

  // Now populate the Upper triangular elements of Ke
  for(int Col = 1; Col < Num_Local_Eq; Col++)
    for(int Row = 0; Row < Col; Row++)
      Ke(Row, Col) = Ke(Col, Row);

  // Ke has now been set
  Ke_Set_Up = true;

  #if defined(ELEMENT_MONITOR)
    printf("Element stiffness matrix:\n");
    for(int i = 0; i < Num_Local_Eq; i++) {
      printf("| ");

      for(int j = 0; j < Num_Local_Eq; j++)
        printf("%6.2lf ", Ke(i,j));

      printf("|\n");
    } // for(int i = 0; i < Num_Local_Eq, i++) {
  #endif

  return SUCCESS;
} // Errors Element::Populate_Ke(void) {



Errors Element::Move_Ke_To_K(void) const {
  /* Assumption 1:
  This function assumes that the element stiffness matrix, Ke, has been set.

  This function also assumes that the Element class static members, namely K,
  has been set. It is not possible, however, to set up Ke without having the
  Static members set. Therefore, if Ke is set then both assumptions must be
  valid */
  if(Ke_Set_Up == false)
    return KE_NOT_SET_UP;

  // First, move the diagional cells of Ke to K
  for(int i = 0; i < Num_Local_Eq; i++) {
    const int I = Local_Eq_Num_To_Global_Eq_Num[i];
    (*K)(I, I) += Ke(i,i);
  } // for(int i = 0; i < Num_Local_Eq; i++) {

  // Now, move the off diagional cells of Ke to K
  for(int Col = 0; Col < Num_Local_Eq; Col++) {
    // Get Global column number, J, associated with the local column number "Col"
    const int J = Local_Eq_Num_To_Global_Eq_Num[Col];

    for(int Row = Col+1; Row < Num_Local_Eq; Row++) {
      // Get Global Row number, I, associated with the local row number "Row"
      const int I = Local_Eq_Num_To_Global_Eq_Num[Row];


      const double Ke_Row_Col = Ke(Row, Col);
      (*K)(I,J) += Ke_Row_Col;
      (*K)(J,I) += Ke_Row_Col;
    } // for(int Row = Col+1; Row < Num_Local_Eq; Row++) {
  } // for(int Col = 0; Col < Num_Local_Eq; Col++) {

  return SUCCESS;
} // Errors Element::Move_Ke_To_K(void) const {



// Get Node ID
Errors Element::Node_ID(const unsigned i, unsigned & ID_Out) const {
  /* Assumption 1:
  This function assumes that this element has been set up. If it hasn't then
  there is no node ID to report.*/
  if(Element_Set_Up == false)
    return ELEMENT_NOT_SET_UP;

  /* Assumption 2:
  This function also assumes that the requested index is within the bounds of
  the node List. This means that i = 0,1,2... 7*/
  if(i >= 8)
    return NODE_ID_INDEX_OUT_OF_BOUNDS;

  ID_Out = Node_List[i];

  return SUCCESS;
} // Errors Element::Node_ID(const unsigned i, unsigned & ID_Out) const {





////////////////////////////////////////////////////////////////////////////////
// Friend functions

Errors Set_Element_Static_Members(Node * Node_Array_Ptr, Matrix<int> * ID_Ptr, Matrix<double> * K_Ptr, double (*Integrating_Function)(unsigned, unsigned, unsigned, unsigned)) {
  /* Assumption 1:
  This function is used to set up the Element class. We really only want to be
  able to do this once. (doing so multiple times would lead to disaster).

  If the Element static members have already been set then we return an error */
  if(Element::Static_Members_Set == true)
    return STATIC_MEMBERS_ALREADY_SET;

  // Set Static members
  Element::Node_Array = Node_Array_Ptr;
  Element::ID = ID_Ptr;
  Element::F = Integrating_Function;
  Element::K = K_Ptr;

  // Static members are now set.
  Element::Static_Members_Set = true;

  return SUCCESS;
} // Errors Set_Element_Static_Members(Node * Node_Array_Ptr, Matrix<int> * ID_Ptr, Matrix<double> * K_Ptr, double (*Integrating_Function)(unsigned, unsigned, unsigned, unsigned)) {





////////////////////////////////////////////////////////////////////////////////
// Disabled Implicit methods

// Disabled copy constructor
Element::Element(const Element & El) {
  // Yell at the user if they try to use the copy Constructor
  printf("Element Error! You tried using the copy construct for the Element class\n");
  printf("The copy constructor is disabled! \n");
} // Element::Element(const Element & El) {



// Disabled = operator
Element & Element::operator=(const Element & El) {
  printf("Error in Element & Element::operator=(const Element & El)\n");
  printf("Element equality is not defined! Returning *this\n");
  return *this;
} // Element & operator=(const Element & El) {

#endif
