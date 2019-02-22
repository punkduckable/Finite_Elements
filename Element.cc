#if !defined(ELEMENT_SOURCE)
#define ELEMENT_SOURCE

#include "Element.h"
#include <stdio.h>
//#define ELEMENT_MONITOR

using namespace Element_Errors;

////////////////////////////////////////////////////////////////////////////////
// Set Elements static members

bool Element::Static_Members_Set = false;
Matrix<unsigned> * Element::ID;
Matrix<double> * Element::K;





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
    printf("Error in void Element::Set_Nodes!\n");
    return STATIC_MEMBERS_NOT_SET;
  } // if(Static_Members_Set == false) {

  /* Assumptions 3:
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
  // Set up Local_Eq_Num_To_Node, Local_Eq_Num_To_Global_Eq_Num

  unsigned Eq_Num = 0;
  for(int Node = 0; Node < 8; Node++) {
    const unsigned Global_Node_Number = Node_List[Node];

    for(int Component = 0; Component < 3; Component++) {
      // Set Local_Eq_Num_To_Node
      Local_Eq_Num_To_Node[0 + Eq_Num*2] = Global_Node_Number;
      Local_Eq_Num_To_Node[1 + Eq_Num*2] = Component;

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
      for mapping Ke to K) */
      int Global_Eq_Number = (*ID)(Global_Node_Number, Component);
      if(Global_Eq_Number == -1)
        Local_Eq_Num_To_Global_Eq_Num[Eq_Num] = FIXED_COMPONENT;
      else
        Local_Eq_Num_To_Global_Eq_Num[Eq_Num] = Global_Eq_Number;

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


    printf("Local_Eq_Num_To_Global_Eq_Num: ");
    for(int i = 0; i < 24; i++)
      /* Note, even though Local_Eq_Num_To_Global_Eq_Num is an array of unsigned
      integers, I print it as an array of signed integers so that FIXED_COMPONENT
      shows up as -1 and not some nonsense large number. */
      printf("%d ", Local_Eq_Num_To_Global_Eq_Num[i]);

    printf("\n");


    printf("Local_Eq_Num_To_Node:\n");
    for(int j = 0; j < 2; j++) {
      printf("| ");
      for(int i = 0; i < 24; i++)
        printf("%3u ", Local_Eq_Num_To_Node[i*2 + j]);

      printf("|\n");
    } // for(int j = 0; j < 24; j++) {
  #endif

  return SUCCESS;
} // Errors Element::Set_Nodes(const unsigned Node1_ID,



Errors Element::Populate_Ke(void) {
  /* Function description:
  This method is used to populate Ke, the element stiffness matrix. Once
  this method has run, Ke can be mapped to K and F. */

  /* Assumption 1:
  This function assumes that the nodes in the Node_List are in a particular
  order. Specifically, we assume that the passed nodes are in the same order
  as the figure on page 123 of Hughes' book.

  We have no way of testing and/or verrifying this assumption. Therefore, we
  simply assume that the user set up the node list in the correct order. */

  /* Assumption 2:
  This function assumes that this Element has been set up. More specificially,
  this function assumes that the node list has been set, and that
  Local_Eq_Num_To_Node has been set up. */
  if(Element_Set_Up == false)
    return ELEMENT_NOT_SET_UP;

  /* Assumption 3:
  This function also assumes that Ke has not been set already. */
  if(Ke_Set_Up == true)
    return KE_ALREADY_SET_UP;


  // Ke has now been set
  Ke_Set_Up = true;

  #if defined(ELEMENT_MONITOR)
    printf("Element stiffness matrix:\n");
    for(int i = 0; i < 24; i++) {
      printf("| ");

      for(int j = 0; j < 24; j++)
        printf("%6.2lf ", Ke(i,j));

      printf("|\n");
    } // for(int i = 0; i < 24, i++) {
  #endif

  return SUCCESS;
} // Errors Element::Populate_Ke(void) {



Errors Element::Fill_Ke_With_1s(void) {
  /* Function description:
  This function is used for testing purposes. It sets every element of Ke to 1.
  Once this has been done, Ke can be mapped to K. This is used to test that
  the assembly proceedure is working correctly */

  /* Assumption 1:
  This function assumes that this Element has been set up. More specificially,
  this function assumes that the node list has been set, and that
  Local_Eq_Num_To_Node has been set up. */
  if(Element_Set_Up == false)
    return ELEMENT_NOT_SET_UP;

  /* Assumption 2:
  This function also assumes that Ke has not been set already. */
  if(Ke_Set_Up == true)
    return KE_ALREADY_SET_UP;

  // Fill Ke's with 1's
  for(int i = 0; i < 24; i++)
    for(int j = 0; j < 24; j++)
      Ke(i,j) = 1;

  // Ke has now been set
  Ke_Set_Up = true;

  #if defined(ELEMENT_MONITOR)
    printf("Element stiffness matrix:\n");
    for(int i = 0; i < 24; i++) {
      printf("| ");

      for(int j = 0; j < 24; j++)
        printf("%6.2lf ", Ke(i,j));

      printf("|\n");
    } // for(int i = 0; i < 24, i++) {
  #endif

  return SUCCESS;
} // Errors Element::Fill_Ke_With_1s(void) {




Errors Element::Move_Ke_To_K(void) const {
  /* Function description:
  This fucntion is used to map the element stiffness matrix, Ke, to the global
  stiffness matrix, K. */

  /* Assumption 1:
  This function assumes that the element stiffness matrix, Ke, has been set.

  This function also assumes that the Element class static members, namely K,
  has been set.

  It is not possible, however, to set up Ke without having the Static members
  set. Therefore, if Ke is set then both assumptions must be satisified */
  if(Ke_Set_Up == false)
    return KE_NOT_SET_UP;

  /* First, move the diagional cells of Ke to K. We only move the components
  that correspond to a global equation. Recall that Ke has a row for each
  component of each of the 8 nodes in this Element's Node list, even though
  some of those components may be fixed. We kept track of this with the
  "FIXED_COMPONENT" constant wen we set up Local_Eq_Num_To_Global_Eq_Num */
  for(int i = 0; i < 24; i++) {
    const int I = Local_Eq_Num_To_Global_Eq_Num[i];
    if(I == FIXED_COMPONENT)
      continue;
    else
      (*K)(I, I) += Ke(i,i);
  } // for(int i = 0; i < 24; i++) {

  /* Now, move the off-diagional cells of Ke to K. Again, We only move the
  components that correspond to a global equation (see previous comment) */
  for(int Col = 0; Col < 24; Col++) {
    // Get Global column number, J, associated with the local column number "Col"
    const int J = Local_Eq_Num_To_Global_Eq_Num[Col];

    // Check if J corresponds to a fixed component
    if(J == FIXED_COMPONENT)
      continue;
    else
      for(int Row = Col+1; Row < 24; Row++) {
        // Get Global Row number, I, associated with the local row number "Row"
        const int I = Local_Eq_Num_To_Global_Eq_Num[Row];

        // Check if I corresponds to a fixed component
        if(I == FIXED_COMPONENT)
          continue;

        // If not, move Ke(Row, Col) to the corresponding position in K.
        const double Ke_Row_Col = Ke(Row, Col);
        (*K)(I,J) += Ke_Row_Col;
        (*K)(J,I) += Ke_Row_Col;
      } // for(int Row = Col+1; Row < 24; Row++) {
  } // for(int Col = 0; Col < 24; Col++) {

  return SUCCESS;
} // Errors Element::Move_Ke_To_K(void) const {





////////////////////////////////////////////////////////////////////////////////
// Friend functions

Errors Set_Element_Static_Members(Matrix<unsigned> * ID_Ptr, Matrix<double> * K_Ptr) {
  /* Function description:
  This function is used to set the static members for the Element class. */

  /* Assumption 1:
  This function is used to set up the Element class. We really only want to be
  able to do this once. (doing so multiple times would lead to disaster).

  If the Element static members have already been set then we return an error */
  if(Element::Static_Members_Set == true)
    return STATIC_MEMBERS_ALREADY_SET;

  // Set Static members
  Element::ID = ID_Ptr;
  Element::K = K_Ptr;

  // Static members are now set.
  Element::Static_Members_Set = true;

  return SUCCESS;
} // Errors Set_Element_Static_Members(Matrix<unsigned> * ID_Ptr, Matrix<double> * K_Ptr) {





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
