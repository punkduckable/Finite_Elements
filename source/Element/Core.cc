#if !defined(ELEMENT_SOURCE)
#define ELEMENT_SOURCE

/* File description:
This file includes the core functions of the Element class including the
constructors, destructor, and Node_Set_Up */

#include "Element.h"
#include <stdio.h>
//#define ELEMENT_MONITOR                // Prints Node list and Node positions



////////////////////////////////////////////////////////////////////////////////
// Declare Elements static members

bool Element::Static_Members_Set  = false;
Matrix<int> * Element::ID;
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
// Set up element (Set_Nodes + Set_Up)

void Element::Set_Up(const unsigned Node0_ID,
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
  Local_Eq_Num_To_Global_Eq_Num array (which is used for mapping Ke to K). This
  function is private and should ONLY be called by one of the two public
  "Set_Node" functions. */

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
    Element_Nodes[Node].Xa = Global_Node_Array[Element_Nodes[Node].ID].Get_Position_Component(0);
    Element_Nodes[Node].Ya = Global_Node_Array[Element_Nodes[Node].ID].Get_Position_Component(1);
    Element_Nodes[Node].Za = Global_Node_Array[Element_Nodes[Node].ID].Get_Position_Component(2);

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
      component of Prescribed_Displacements to the associated fixed position.
      Otherwise, the Eq_Num'th component of Prescribed_Displacements is set to 0*/
      int Global_Eq_Number = (*ID)(Global_Node_Number, Component);
      if(Global_Eq_Number == -1) {
        Local_Eq_Num_To_Global_Eq_Num[Eq_Num] = FIXED_COMPONENT;
        Prescribed_Displacements[Eq_Num] = Global_Node_Array[Element_Nodes[Node].ID].Get_Displacement_Component(Component);
      } // if(Global_Eq_Number == -1) {
      else {
        Local_Eq_Num_To_Global_Eq_Num[Eq_Num] = Global_Eq_Number;
        Prescribed_Displacements[Eq_Num] = 0;
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


    printf("Prescribed Displacements: ");
    for(int i = 0; i < 24; i++)
      printf("%5.2lf ", Prescribed_Displacements[i]);
    printf("\n");
  #endif
} // void Element::Set_Up(const unsigned Node1_ID,



// Brick element set nodes.
void Element::Set_Nodes( const unsigned Node0_ID,
                         const unsigned Node1_ID,
                         const unsigned Node2_ID,
                         const unsigned Node3_ID,
                         const unsigned Node4_ID,
                         const unsigned Node5_ID,
                         const unsigned Node6_ID,
                         const unsigned Node7_ID) {
  /* Function description:
  This function is used to set up Brick type elements. Brick elements have
  8 distinct nodal positions. Each passed node should therefore have a distinct
  position. */
  (*this).Type = Element_Types::BRICK;
  Set_Up(Node0_ID, Node1_ID, Node2_ID, Node3_ID, Node4_ID, Node5_ID, Node6_ID, Node7_ID);
} // void Element::Set_Nodes( const unsigned Node0_ID,



// Wedge element set nodes.
void Element::Set_Nodes( const unsigned Node0_ID,
                         const unsigned Node1_ID,
                         const unsigned Node2_ID,
                         const unsigned Node4_ID,
                         const unsigned Node5_ID,
                         const unsigned Node6_ID) {
  /* Function description:
  This function is used to set up Wedge type elements. Wedge elements have
  have 6 distinct nodal positions. They are formed from brick elements by
  degenerating Nodes 2 and 3 as well as 6 and 7 (0 index). */
  (*this).Type = Element_Types::WEDGE;
  Set_Up(Node0_ID, Node1_ID, Node2_ID, Node2_ID, Node4_ID, Node5_ID, Node6_ID, Node6_ID);
} // void Element::Set_Nodes( const unsigned Node0_ID,



////////////////////////////////////////////////////////////////////////////////
// Getters, setters

unsigned Element::Get_Node_ID(const unsigned Index) const {
  /* Function description:
  This function, as the name implies, is used to get the index of one of the
  nodes in this element's node list. */

  /* Assumption 1:
  This function assumes that Index is 0-7. If this is not the case, then we
  thrown an Array exception. */
  if(Index > 7) {
    char Error_Message_Buffer[500];
    sprintf(Error_Message_Buffer,
            "Array Index Out Of Bounds Exception: Thrown by Element::Get_Node_ID\n"
            "Each element only has 8 nodes in its node list. Valid indicies are therefore 0-7\n"
            "You requested index %d.\n",
            Index);
    throw Array_Index_Out_Of_Bounds(Error_Message_Buffer);
  } // if(Index > 7) {

  /* Return the requested node index. */
  return Element_Nodes[Index].ID;
} // unsigned Element::Get_Node_ID(const unsigned Index) const {


Element_Types Element::Get_Element_Type(void) const {
  /* Function description:
  This function, as the name implies, returns the Element Type */

  /* Assumption 1:
  This function assumes that the element has been set up. This is because the
  element type is actually specified during set up. */
  if((*this).Element_Set_Up == false) {
    char Error_Message_Buffer[500];
    sprintf(Error_Message_Buffer,
            "Element Not Set Up Exception: Thrown by Element::Get_Element_Type\n"
            "The element type is set when the elemetn is set up. Since this element\n"
            "has not been set up, it has no type!\n");
    throw Element_Not_Set_Up(Error_Message_Buffer);
  } // if((*this).Element_Set_Up == false) {

  return (*this).Type;
} // Element_Types Element::Get_Element_Type(void) const {






////////////////////////////////////////////////////////////////////////////////
// Other methods

void Print_Matrix_Of_Doubles(const Matrix<double> & M, unsigned width, unsigned precision) {
  /* Function Description:
  This function prints the given matrix of doubles to the terminal. This
  function should, in general, only be used for debugging and testing purposes.

  Both width and precision are defaulted. You should only supply these
  parameters if the defaulted values are unsatisfactory. witdh is the number of
  characters used to print each element of the matrix. precision is the number
  of characters after the decimal place. By default, width = 8, precision = 1.

  This method is ONLY defined for matricies of doubles. It should be noted that
  there is only one way to print out a matrix to the screen (row-by-row). As
  such, this method will run faster for Row-major matrix than for Column-major
  ones. */

  const unsigned Num_Rows = M.Get_Num_Rows();
  const unsigned Num_Cols = M.Get_Num_Cols();

  /* First, construct the print format (using the passed parameters).
  Note: there is a reason for the triple %. The first two print a % sign (for
  whatever reason, that's how you print a % sign in a string with other
  characters). The third % marks the start of %d, which instructs scanf to place
  the first paramter, width, there. Confusing, but it works. */
  char Format_Buffer[20];
  sprintf(Format_Buffer, "%%%d.%de ", width, precision);

  for(unsigned i = 0; i < Num_Rows; i++) {
    printf("| ");
    for(unsigned j = 0; j < Num_Cols; j++) {
      printf(Format_Buffer, M(i,j));
    } // for(unsigned j = 0; j < Num_Cols; j++) {
    printf("|\n");
  } // for(unsigned i = 0; i < Num_Rows; i++) {
} // void Print_Matrix_Of_Doubles(const Matrix<double> & M, unsigned width, unsigned precision) {

#endif
