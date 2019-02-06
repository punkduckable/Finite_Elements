#if !defined(ELEMENT_SOURCE)
#define ELEMENT_SOURCE

#include "Element.h"
#include <stdio.h>

using namespace Element_Errors;

////////////////////////////////////////////////////////////////////////////////
// Set Elements static members

unsigned Element::Num_Elements = 0;
Node * Element::Node_Array = NULL;
unsigned * Element::ID = NULL;
double (*Element::F)(unsigned, unsigned, unsigned, unsigned);
double * Element::K = NULL;



////////////////////////////////////////////////////////////////////////////////
// Constructors, destructor

// Deep copy constructor
Element::Element(const Element & El) {
  // Increment number of elements
  Num_Elements++;

  /* Check if El has been set up. If so, then deep copy Local_Eq_Num_To_Node,
  Local_Eq_Num_To_Global_Eq_Num and Ke */
  if(El.Element_Set_Up == true) {
    // Copy over non-dynamic members
    for(int i = 0; i < 8; i++)
      Node_List[i] = El.Node_List[i];

    Element_Set_Up = true;
    Num_Local_Eq = El.Num_Local_Eq;


    // Deep copy Local_Eq_Num_To_Node.
    const unsigned int Twice_Num_Local_Eq = Num_Local_Eq*2;
    Local_Eq_Num_To_Node = new unsigned[Twice_Num_Local_Eq];
    for(int i = 0; i < Twice_Num_Local_Eq; i++)
      Local_Eq_Num_To_Node[i] = El.Local_Eq_Num_To_Node[i];


    // Deep copy Local_Eq_Num_To_Global_Eq_Num
    Local_Eq_Num_To_Global_Eq_Num = new unsigned[Num_Local_Eq];
    for(int i = 0; i < Num_Local_Eq; i++)
      Local_Eq_Num_To_Global_Eq_Num[i] = El.Local_Eq_Num_To_Global_Eq_Num[i];


    // Deep copy Ke
    const unsigned int Num_Local_Eq_Squared = Num_Local_Eq*Num_Local_Eq;
    Ke = new double[Num_Local_Eq_Squared];
    for(int i = 0; i < Num_Local_Eq_Squared; i++)
      Ke[i] = El.Ke[i];
  } // if(El.Element_Set_Up == true) {
  else
    Element_Set_Up = false;
} // Element::Element(const Element & El) {



// Destructor
Element::~Element(void) {
  /* Check if nodes are set. If so then this element dynamically allocated
  memory for Local_Eq_Num_To_Node, Local_Eq_Num_To_Global_Eq_Num, and Ke. We
  need to free this memory before deleting the object. */
  if(Element_Set_Up) {
    delete [] Local_Eq_Num_To_Node;
    delete [] Local_Eq_Num_To_Global_Eq_Num;
    delete [] Ke;
  } // if(Element_Set_Up) {

  // Decrememnt number of elements
  Num_Elements--;
} // Element::~Element(void) {





////////////////////////////////////////////////////////////////////////////////
// Other Element methods

// Disabled = operator.
Element & Element::operator=(const Element & El) {
  printf("Error in Element & Element::operator=(const Element & El)\n");
  printf("Element equality is not defined! Returning *this\n");
  return *this;
} // Element & Element::operator=(const Element & El) {



// Set nodes
Errors Element::Set_Nodes(const unsigned Node0_ID,
                          const unsigned Node1_ID,
                          const unsigned Node2_ID,
                          const unsigned Node3_ID,
                          const unsigned Node4_ID,
                          const unsigned Node5_ID,
                          const unsigned Node6_ID,
                          const unsigned Node7_ID) {

  /* First, check if the Element class has been set up.
  The element class contains a few static members. One of these, the ID array,
  points to the global ID array. Another, Node_Array, points to the
  array of nodes used in this simulation. We need to have access to both of
  these arrays to be able to set up the Element. Therefore, if these static
  members have not been set and the user is trying to set this Element's nodes
  then we need to thrown an error.

  The Node_Array and ID array members are set to point to NULL when the
  program begins (see the start of this file). Therefore, we can determine
  if the Element class has been set up by checking if either of these static
  members is NULL */
  if(ID == NULL || Node_Array == NULL) {
    printf("Error in void Element::Set_Nodes!\n");
    printf("To set up an element, the ID and Node_Array static members must be set\n");
    printf("Currently, ID = %p and Node_Array = %p.\n", ID, Node_Array);

    return STATIC_MEMBERS_NOT_SET;
  } // if(ID == NULL || Node_Array == NULL) {


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

  // Nodes are now set
  Element_Set_Up = true;


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
  Ke = new double[Num_Local_Eq*Num_Local_Eq];

  //////////////////////////////////////////////////////////////////////////////
  // Set up Local_Eq_Num_To_Node, Local_Eq_Num_To_Global_Eq_Num

  unsigned Eq_Num = 0;
  for(int Node = 0; Node < 8; Node++) {
    const unsigned Global_Node_Number = Node_List[Node];

    for(int Component = 0; Component < 3; Component++) {
      if(Node_Array[Global_Node_Number].Fixed_Pos[Component] == false) {
        // Set Local_Eq_Num_To_Global_Eq_Num
        Local_Eq_Num_To_Global_Eq_Num[Eq_Num] = ID[3*Global_Node_Number + Component];

        // Set Local_Eq_Num_To_Node
        Local_Eq_Num_To_Node[2*Eq_Num + 0] = Global_Node_Number;
        Local_Eq_Num_To_Node[2*Eq_Num + 1] = Component;

        // Increment equation number
        Eq_Num++;
      } // if(Node_Array[Node_List[Node]].Fixed_Pos[Component] == false) {
    } // for(int Component = 0; Component < 3 Component++) {
  } // for(int Node = 0; Node < 8; Node++) {

  return SUCCESS;
} // Errors Element::Set_Nodes(const unsigned Node1_ID,



// Populate Ke
Errors Element::Populate_Ke(void) {
  /* Assumptions:
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

  // Populate the diagional + lower triangular elements of Ke
  for(int Col = 0; Col < Num_Local_Eq; Col++) {
    /* Get ID and Component associated with the Col'th local equation. It should
    be noted that this array is stored in Row major order and that its
    dimensions are Num_Local_Eq x 2. Therefore, the ID for the ith Node is at
    2*i while the component is at 2*i + 1 */
    const unsigned Node_A_ID = Local_Eq_Num_To_Node[2*Col + 0];
    const unsigned Node_A_Component = Local_Eq_Num_To_Node[2*Col + 1];

    for(int Row = Col; Row < Num_Local_Eq; Row++) {
      /* Get ID and Component associated with the Row'th local equation. */
      const unsigned Node_B_ID = Local_Eq_Num_To_Node[2*Row + 0];
      const unsigned Node_B_Component = Local_Eq_Num_To_Node[2*Row + 1];

      // Populate Ke
      Ke[Row + Num_Local_Eq*Col] = F(Node_A_ID, Node_A_Component, Node_B_ID, Node_B_Component);
    } // for(int Row = Col; Row < Num_Local_Eq; Row++) {
  } // for(int Col = 0; Col < Num_Local_Eq; Col++) {

  // Now populate the Upper triangular elements of Ke
  for(int Col = 1; Col < Num_Local_Eq; Col++)
    for(int Row = 0; Row < Col; Row++)
      Ke[Row + Col*Num_Local_Eq] = Ke[Col + Num_Local_Eq*Row];                 // Ke[i,j] = Ke[j,i];

  return SUCCESS;
} // Errors Element::Populate_Ke(void) {



// Get Node ID
Errors Element::Node_ID(const unsigned i, unsigned & ID_Out) const {
  if(Element_Set_Up == false)
    return ELEMENT_NOT_SET_UP;

  // Check that i is within the bounds of the Node_List
  if(i >= 8)
    return INDEX_OUT_OF_BOUNDS;

  ID_Out = Node_List[i];

  return SUCCESS;
} // Errors Element::Node_ID(const unsigned i, unsigned & ID_Out) const {





////////////////////////////////////////////////////////////////////////////////
// Friend functions

Errors Set_Element_Static_Members(Node * Node_Array_Ptr, unsigned * ID_Ptr, double (*Integrating_Function)(unsigned, unsigned, unsigned, unsigned), double * K_Ptr) {
  /* This function is used to set up the Element class. We really only want to be
  able to do this once. (doing so multiple times would lead to disaster).

  The Node_Array and ID array static members are initialized to NULL (see
  the top of this file). After being set, they will not be NULL.

  Therefore, we can use the value of these pointers to determine if the element
  class has been set up already. If it has, then we return an error */
  if(Element::Node_Array != NULL || Element::ID != NULL || Element::K != NULL)
    return STATIC_MEMBERS_ALREADY_SET;

  // Set Static members
  Element::Node_Array = Node_Array_Ptr;
  Element::ID = ID_Ptr;
  Element::F = Integrating_Function;
  Element::K = K_Ptr;

  return SUCCESS;
} // Errors Set_Element_Static_Members(Node * Node_Array_Ptr, unsigned * ID_Ptr, double (*Integrating_Function)(unsigned, unsigned, unsigned, unsigned), double * K_Ptr) {

#endif
