#if !defined(ELEMENT_SOURCE)
#define ELEMENT_SOURCE

/* File description:
This file includes the core functions of the Element class including the
constructors, destructor, and Node_Set_Up */

#include "Element.h"
#include <stdio.h>
#define ELEMENT_MONITOR                // Prints Node list and Node positions



////////////////////////////////////////////////////////////////////////////////
// Declare Elements static members

bool Element::Static_Members_Set  = false;
Matrix<unsigned> * Element::ID;
Matrix<double> * Element::K;
double * Element::F;
Node * Element::Global_Node_Array;

Matrix<double> Element::Na        {8, 8, Memory::COLUMN_MAJOR};
Matrix<double> Element::Na_Xi     {8, 8, Memory::COLUMN_MAJOR};
Matrix<double> Element::Na_Eta    {8, 8, Memory::COLUMN_MAJOR};
Matrix<double> Element::Na_Zeta   {8, 8, Memory::COLUMN_MAJOR};

Matrix<double> Element::D         {6, 6, Memory::ROW_MAJOR};
bool Element::Material_Set        = false;



////////////////////////////////////////////////////////////////////////////////
// Destructor

Element::~Element(void) {
} // Element::~Element(void) {





////////////////////////////////////////////////////////////////////////////////
// Other Element methods

void Element::Set_Nodes(const unsigned Node0_ID,
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
  The element class contains static members. One of these, the ID array,
  points to the global ID array. We need to have access to this array to set
  up the Element. Therefore, if the static members have not been set then we
  throw a "Element_Not_Set_Up" exception. */
  if(Static_Members_Set == false) {
    char Error_Message_Buffer[500];
    sprintf(Error_Message_Buffer,
            "Element Not Set Up Exception: Thrown by Element::Set_Nodes\n"
            "the element class static members (namely the ID array) must be\n"
            "before you can set an element's nodes. This is because Set_Nodes\n"
            "relies on the ID array to set up the element.\n");
    throw Element_Not_Set_Up(Error_Message_Buffer);
  } // if(Static_Members_Set == false) {

  /* Assumptions 3:
  This function also assumes that the nodes for this specific element have not
  been set. If this is not the case then an Element_Already_Set_Up exception is
  thrown. */
  if(Element_Set_Up == true) {
    char Error_Message_Buffer[500];
    sprintf(Error_Message_Buffer,
            "Element Already Set Up Exception: Thrown by Element::Set_Nodes\n"
            "Once an element's nodes have been set, they can not be changed.\n");
    throw Element_Already_Set_Up(Error_Message_Buffer);
  } // if(Element_Set_Up == true) {

  //////////////////////////////////////////////////////////////////////////////


  /* If we've made it this far then the Element class has been set up. We can
  therefore set the nodes.
  To begin, set the Node_List using the passed Node_ID's */
  Element_Nodes[0].ID = Node0_ID;
  Element_Nodes[1].ID = Node1_ID;
  Element_Nodes[2].ID = Node2_ID;
  Element_Nodes[3].ID = Node3_ID;
  Element_Nodes[4].ID = Node4_ID;
  Element_Nodes[5].ID = Node5_ID;
  Element_Nodes[6].ID = Node6_ID;
  Element_Nodes[7].ID = Node7_ID;



  //////////////////////////////////////////////////////////////////////////////
  // Set up Local_Eq_Num_To_Global_Eq_Num, Element_Nodes

  unsigned Eq_Num = 0;
  for(int Node = 0; Node < 8; Node++) {
    // First, set the X, Y, and Z components of this node's position.
    Element_Nodes[Node].Xa = Global_Node_Array[Element_Nodes[Node].ID].Original_Position(0);
    Element_Nodes[Node].Ya = Global_Node_Array[Element_Nodes[Node].ID].Original_Position(1);
    Element_Nodes[Node].Za = Global_Node_Array[Element_Nodes[Node].ID].Original_Position(2);

    // Now, get the Global node number
    const unsigned Global_Node_Number = Element_Nodes[Node].ID;

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
        Prescribed_Positions[Eq_Num] = Global_Node_Array[Element_Nodes[Node].ID].BC(Component);
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
      printf("%u ", Element_Nodes[i].ID);
    printf("\n");



    printf("Node Positions:\n");
    printf("Xa = | ");
    for(int i = 0; i < 8; i++)
      printf("%6.3lf ", Element_Nodes[i].Xa);
    printf("|\n");

    printf("Ya = | ");
    for(int i = 0; i < 8; i++)
      printf("%6.3lf ", Element_Nodes[i].Ya);
    printf("|\n");

    printf("Za = | ");
    for(int i = 0; i < 8; i++)
      printf("%6.3lf ", Element_Nodes[i].Za);
    printf("|\n");



    printf("Local_Eq_Num_To_Global_Eq_Num: ");
    for(int i = 0; i < 24; i++)
      /* Note, even though Local_Eq_Num_To_Global_Eq_Num is an array of unsigned
      integers, I print it as an array of signed integers so that FIXED_COMPONENT
      shows up as -1 and not some nonsense large number. */
      printf("%d ", Local_Eq_Num_To_Global_Eq_Num[i]);
    printf("\n");


    printf("Prescribed Positions: ");
    for(int i = 0; i < 24; i++)
      printf("%5.2lf ", Prescribed_Positions[i]);
    printf("\n");
  #endif
} // void Element::Set_Nodes(const unsigned Node1_ID,

#endif
