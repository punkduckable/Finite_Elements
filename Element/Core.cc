#if !defined(ELEMENT_SOURCE)
#define ELEMENT_SOURCE

/* File description:
This file includes the core functions of the Element class including the
constructors, destructor, and Node_Set_Up */

#include "Element.h"
#include <stdio.h>
#define ELEMENT_MONITOR                // Prints Node list and Node positions

using namespace Element_Errors;



////////////////////////////////////////////////////////////////////////////////
// Declare Elements static members

bool Element::Static_Members_Set  = false;
Matrix<unsigned> * Element::ID;
Matrix<double> * Element::K;
double * Element::F;
Node * Element::Nodes;

Matrix<double> Element::Na        = Matrix<double>(8, 8, Memory::COLUMN_MAJOR);
Matrix<double> Element::Na_Xi     = Matrix<double>(8, 8, Memory::COLUMN_MAJOR);
Matrix<double> Element::Na_Eta    = Matrix<double>(8, 8, Memory::COLUMN_MAJOR);
Matrix<double> Element::Na_Zeta   = Matrix<double>(8, 8, Memory::COLUMN_MAJOR);

Matrix<double> Element::D         = Matrix<double>(6, 6, Memory::ROW_MAJOR);
bool Element::Material_Set        = false;



////////////////////////////////////////////////////////////////////////////////
// Destructor

Element::~Element(void) {
} // Element::~Element(void) {





////////////////////////////////////////////////////////////////////////////////
// Other Element methods

Errors Element::Set_Nodes(const unsigned Node0_ID,
                          const unsigned Node1_ID,
                          const unsigned Node2_ID,
                          const unsigned Node3_ID,
                          const unsigned Node4_ID,
                          const unsigned Node5_ID,
                          const unsigned Node6_ID,
                          const unsigned Node7_ID) {
  /* Function description:
  This function is used to set set up the Element. The passed Node ID's are
  moved into the Node_List, which then uses the ID array to populate the
  Local_Eq_Num_To_Global_Eq_Num array (which is used for mapping Ke to K) */

  /* Assumption 1:
  This function assumes that the passed nodes are in a particular order.
  In particular, we assume that the nodes are in the order described on page
  123 of Hughes' book.

  Unfortuneatly, there is no way to verrify this assumption. Therefore, we
  simply assume that the user has supplied the nodes in the correct order */


  /* Assumption 2:
  This function assumes that the Element class has been set up.
  The element class contains a few static members. One of these, the ID array,
  points to the global ID array. We need to have access to this array to set
  up the Element. Therefore, if these static members have not been set and the
  user is trying to set this Element's nodes then we need to thrown an error.

  When the static members are set, a flag called "Static_Members_Set" gets
  flipped to true. Therefore, if this flag is true then the assumption is
  satisified.  */
  if(Static_Members_Set == false) {
    printf("Error in Element::Set_Nodes\n");
    return STATIC_MEMBERS_NOT_SET;
  } // if(Static_Members_Set == false) {

  /* Assumptions 3:
  This function also assumes that this specific element has not has its nodes
  set already. */
  if(Element_Set_Up == true) {
    printf("Error in Element::Set_Nodes\n");
    return ELEMENT_ALREADY_SET_UP;
  } // if(Element_Set_Up == true) {

  //////////////////////////////////////////////////////////////////////////////


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
  // Set up Local_Eq_Num_To_Global_Eq_Num, Xa, Ya, and Za

  unsigned Eq_Num = 0;
  for(int Node = 0; Node < 8; Node++) {
    // First, set the X, Y, and Z components of this node's position.
    Xa[Node] = Nodes[Node_List[Node]].Position(0);
    Ya[Node] = Nodes[Node_List[Node]].Position(1);
    Za[Node] = Nodes[Node_List[Node]].Position(2);

    // Now, get the Global node number
    const unsigned Global_Node_Number = Node_List[Node];

    /* Cycle through the components, check which ones are free/which ones are
    fixed using the ID array. Store this information in Local_Eq_Num_To_Global_Eq_Num */
    for(int Component = 0; Component < 3; Component++) {
      /* If a particular component of a node's position is fixed then there is
      no global equation for that component. Therefore, we don't want to map the
      corresponding component of Ke to K.

      The issue is, the Element doesn't have any way of knowing which nodes have
      fixed components and which ones don't based just on the global node ID's
      (which is what was passed to this function). Luckily, however, if a node's
      component is fixed, then that component's corresponding cell in ID array
      will be set to -1. Therefore, we can check if ID(Node, Component) == -1.
      If this is the case, then we set the corresponding cell in
      Local_Eq_Num_To_Global_Eq_Num to the constant FIXED_COMPONENT, which
      indiciates that the corresponding component is fixed (this is important
      for mapping Ke to K)

      If the corresponding global equation is fixed, then we set the Eq_Num'th
      component of Prescribed_Positions to the associated fixed position.
      Otherwise, the Eq_Num'th component of Prescribed_Positions is set to 0*/
      int Global_Eq_Number = (*ID)(Global_Node_Number, Component);
      if(Global_Eq_Number == -1) {
        Local_Eq_Num_To_Global_Eq_Num[Eq_Num] = FIXED_COMPONENT;
        Prescribed_Positions[Eq_Num] = Nodes[Node_List[Node]].Position(Component);
      } // if(Global_Eq_Number == -1) {
      else {
        Local_Eq_Num_To_Global_Eq_Num[Eq_Num] = Global_Eq_Number;
        Prescribed_Positions[Eq_Num] = 0;
      } // else {

      // Increment equation number
      Eq_Num++;
    } // for(int Component = 0; Component < 3 Component++) {
  } // for(int Node = 0; Node < 8; Node++) {

  // The element is now set up
  Element_Set_Up = true;


  #if defined(ELEMENT_MONITOR)
    printf("Node list: ");
    for(int i = 0; i < 8; i++)
      printf("%u ", Node_List[i]);
    printf("\n");



    printf("Node Positions:\n");
    printf("Xa = | ");
    for(int i = 0; i < 8; i++)
      printf("%6.3lf ", Xa[i]);
    printf("|\n");

    printf("Ya = | ");
    for(int i = 0; i < 8; i++)
      printf("%6.3lf ", Ya[i]);
    printf("|\n");

    printf("Za = | ");
    for(int i = 0; i < 8; i++)
      printf("%6.3lf ", Za[i]);
    printf("|\n");



    printf("Local_Eq_Num_To_Global_Eq_Num: ");
    for(int i = 0; i < 24; i++)
      /* Note, even though Local_Eq_Num_To_Global_Eq_Num is an array of unsigned
      integers, I print it as an array of signed integers so that FIXED_COMPONENT
      shows up as -1 and not some nonsense large number. */
      printf("%d ", Local_Eq_Num_To_Global_Eq_Num[i]);
    printf("\n");
  #endif

  return SUCCESS;
} // Errors Element::Set_Nodes(const unsigned Node1_ID,





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
