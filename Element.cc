#if !defined(ELEMENT_SOURCE)
#define ELEMENT_SOURCE

#include "Element.h"
#include <stdio.h>
#define ELEMENT_MONITOR

using namespace Element_Errors;

////////////////////////////////////////////////////////////////////////////////
// Declare Elements static members

bool Element::Static_Members_Set = false;
Matrix<unsigned> * Element::ID;
Matrix<double> * Element::K;
Node * Element::Nodes;
Matrix<double> Element::Na      = Matrix<double>(8, 8, Memory::ROW_MAJOR);
Matrix<double> Element::Na_Xi   = Matrix<double>(8, 8, Memory::ROW_MAJOR);
Matrix<double> Element::Na_Eta  = Matrix<double>(8, 8, Memory::ROW_MAJOR);
Matrix<double> Element::Na_Zeta = Matrix<double>(8, 8, Memory::ROW_MAJOR);





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



Errors Element::Calculate_Coefficient_Matrix(const unsigned Point, Matrix<double> & Coeff, double & J) {
  /* Function description:
    This function calculates the coefficient matrix and jacobian determinant
    for a specific integration point. */

  /* Assumption 1:
  This function assumes that "Point" is the index of an integration point.
  Therefore, we assume that Point is in the set {0,1,2... 7}. Since Point
  is unsigned, we only need to check if Point >= 8 to verrify this assumption */
  if(Point >= 8) {
    printf("Error in Element::Calculate_Coefficient_Matrix\n");
    return INTEGRATION_POINT_INDEX_OUT_OF_BOUNDS;
  } // if(Point >= 8) {

  /* Assumption 2:
  This function assumes that the spatial position of each Node is known. This
  means that Xa, Ya, and Za have all been populated. This is done in the
  "Set_Up_Element" method. Therefore, we can use the "Element_Set_Up" flag
  to verrify this assumption */
  if(Element_Set_Up == false) {
    printf("Error in Element::Calculate_Coefficient_Matrix\n");
    return ELEMENT_NOT_SET_UP;
  } // if(Element_Set_Up == false) {

  /* Assumption 3:
  This function assumes that the Xi, Eta, and Zeta partial derivatives for
  each shape function in the master element has been calculated. These
  quantities are calculated by the "Set_Element_Static_Members" method.
  Therefore, if the element class has been set up then this this assumption is
  valid */
  if(Static_Members_Set == false) {
    printf("Error in Element::Calculate_Coefficient_Matrix\n");
    return STATIC_MEMBERS_NOT_SET;
  } // if(Static_Members_Set == false) {

  //////////////////////////////////////////////////////////////////////////////

  /* First, calculate the x, y, and z partials (with respect to Xi, Eta, and
  Zeta) at the passed Integration point. */
  double x_Xi = 0, x_Eta = 0, x_Zeta = 0;
  double y_Xi = 0, y_Eta = 0, y_Zeta = 0;
  double z_Xi = 0, z_Eta = 0, z_Zeta = 0;

  for(int Node = 0; Node < 8; Node++) {
    x_Xi   += Na_Xi  (Node, Point)*Xa[Node];
    x_Eta  += Na_Eta (Node, Point)*Xa[Node];
    x_Zeta += Na_Zeta(Node, Point)*Xa[Node];

    y_Xi   += Na_Xi  (Node, Point)*Ya[Node];
    y_Eta  += Na_Eta (Node, Point)*Ya[Node];
    y_Zeta += Na_Zeta(Node, Point)*Ya[Node];

    z_Xi   += Na_Xi  (Node, Point)*Za[Node];
    z_Eta  += Na_Eta (Node, Point)*Za[Node];
    z_Zeta += Na_Zeta(Node, Point)*Za[Node];
  } // for(int Node = 0; Node < 8; Node++) {


  /* Now, compute the components of the coefficient matrix. This is done using
  the steps outlined on page 150 of Hughes' book. */
  Coeff(1,1) = y_Eta*z_Zeta - y_Zeta*z_Eta;
  Coeff(1,2) = y_Zeta*z_Xi  - y_Xi*z_Zeta;
  Coeff(1,3) = y_Xi*z_Eta   - y_Eta*z_Xi;

  Coeff(2,1) = z_Eta*x_Zeta - z_Zeta*x_Eta;
  Coeff(2,2) = x_Xi*z_Zeta  - x_Zeta*z_Xi;
  Coeff(2,3) = z_Xi*x_Eta   - z_Eta*x_Xi;

  Coeff(3,1) = x_Eta*y_Zeta - x_Zeta*y_Eta;
  Coeff(3,2) = x_Zeta*y_Xi  - x_Xi*y_Zeta;
  Coeff(3,3) = x_Xi*y_Eta   - x_Eta*y_Xi;

  /* Finally, calculate J (the jacobian determinant) */
  J = x_Xi*Coeff(1,1) + x_Eta*Coeff(1,2) + x_Zeta*Coeff(1,3);

  return SUCCESS;
} // Errors Element::Calculate_Coefficient_Matrix(const unsigned Point, Matrix<double> & Coeff, double & J) {



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
  this function assumes that the node list and node positions have been set. */
  if(Element_Set_Up == false) {
    printf("Error in Element::Populate_Ke\n");
    return ELEMENT_NOT_SET_UP;
  } // if(Element_Set_Up == false) {

  /* Assumption 3:
  This function also assumes that Ke has not been set already. */
  if(Ke_Set_Up == true) {
    printf("Error in Element::Populate_Ke\n");
    return KE_ALREADY_SET_UP;
  } // if(Ke_Set_Up == true) {

  //////////////////////////////////////////////////////////////////////////////

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
  this function assumes that the node list has been set. */
  if(Element_Set_Up == false) {
    printf("Error in Element::Fill_Ke_With_1s\n");
    return ELEMENT_NOT_SET_UP;
  } // if(Element_Set_Up == false) {

  /* Assumption 2:
  This function also assumes that Ke has not been set already. */
  if(Ke_Set_Up == true) {
    printf("Error in Element::Fill_Ke_With_1s\n");
    return KE_ALREADY_SET_UP;
  } // if(Ke_Set_Up == true) {

  //////////////////////////////////////////////////////////////////////////////

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
  if(Ke_Set_Up == false) {
    printf("Error in Element::Move_Ke_To_K\n");
    return KE_NOT_SET_UP;
  } // if(Ke_Set_Up == false) {



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

Errors Set_Element_Static_Members(Matrix<unsigned> * ID_Ptr, Matrix<double> * K_Ptr, Node * Nodes_Ptr) {
  /* Function description:
  This function is used to set the static members for the Element class. This
  function also calculates the value of the shape functions (for the master
  element) along with their partial derivatives at each integration point. */

  /* Assumption 1:
  This function is used to set up the Element class. We really only want to be
  able to do this once. (doing so multiple times would lead to disaster).

  If the Element static members have already been set then we return an error */
  if(Element::Static_Members_Set == true) {
    printf("Error in Element::Set_Element_Static_Members\n");
    return STATIC_MEMBERS_ALREADY_SET;
  } // if(Element::Static_Members_Set == true) {

  //////////////////////////////////////////////////////////////////////////////
  // Set Static members
  Element::ID = ID_Ptr;
  Element::K = K_Ptr;
  Element::Nodes = Nodes_Ptr;


  //////////////////////////////////////////////////////////////////////////////
  // Set up Na, Na_xi, Na_eta, Na_zeta

  /* First, let's set up node positions in the master element. This is just an
  implementation of the table on page 124 of Hughes' book. The ordering of
  these coordinates is very important since the ordering of the nodes is
  very important.  */
  double Xi_a[8]   = {-1,  1,  1, -1, -1,  1,  1, -1};
  double Eta_a[8]  = {-1, -1,  1,  1, -1, -1,  1,  1};
  double Zeta_a[8] = {-1, -1, -1, -1,  1,  1,  1,  1};

  /* Next, set up the integration points. It should be noted that the order of
  these points is irrelivent.

  Column X1 = Xi coordinate, Column 2 = Eta coordinate, Column 3 = Zeta coordinate */
  double Xi_Int[8];
  double Eta_Int[8];
  double Zeta_Int[8];
  for(int i = 0; i < 8; i++) {
    Xi_Int[i]   = 0.57735026919*Xi_a[i];
    Eta_Int[i]  = 0.57735026919*Eta_a[i];
    Zeta_Int[i] = 0.57735026919*Zeta_a[i];
  } // for(int i = 0; i < 8; i++) {

  /* Now, calculate Na, Na_Xi, Na_Eta, and Na_Zeta at each integration point for
  each node */
  for(int Node = 0; Node < 8; Node++) {
    for(int Point = 0; Point < 8; Point++) {
      Element::Na(Node, Point)      = (1./8.)*(1. + Xi_a[Node]*Xi_Int[Point])*
                                              (1. + Eta_a[Node]*Eta_Int[Point])*
                                              (1. + Zeta_a[Node]*Zeta_Int[Point]);

      Element::Na_Xi(Node, Point)   = (1./8.)*(Xi_a[Node])*
                                              (1. + Eta_a[Node]*Eta_Int[Point])*
                                              (1. + Zeta_a[Node]*Zeta_Int[Point]);

      Element::Na_Eta(Node, Point)  = (1./8.)*(1. + Xi_a[Node]*Xi_Int[Point])*
                                              (Eta_a[Node])*
                                              (1. + Zeta_a[Node]*Zeta_Int[Point]);

      Element::Na_Zeta(Node, Point) = (1./8.)*(1. + Xi_a[Node]*Xi_Int[Point])*
                                              (1. + Eta_a[Node]*Eta_Int[Point])*
                                              (Zeta_a[Node]);
    } // for(int Point = 0; Point < 8; Point++) {
  } // for(int Node = 0; Node < 8; Node++) {

  #if defined(ELEMENT_MONITOR)
    printf("Integration points:\n");
    for(int i = 0; i < 8; i++)
      printf("| %6.3lf %6.3lf %6.3lf |\n", Xi_Int[i], Eta_Int[i], Zeta_Int[i]);



    printf("\nNa:\n");
    for(int i = 0; i < 8; i++) {
      printf("| ");
      for(int j = 0; j < 8; j++) printf("%6.3lf ", Element::Na(i,j));
      printf("|\n");
    } // for(int i = 0; i < 8; i++) {



    printf("\nNa_Xi:\n");
    for(int i = 0; i < 8; i++) {
      printf("| ");
      for(int j = 0; j < 8; j++) printf("%6.3lf ", Element::Na_Xi(i,j));
      printf("|\n");
    } // for(int i = 0; i < 8; i++) {



    printf("\nNa_Eta:\n");
    for(int i = 0; i < 8; i++) {
      printf("| ");
      for(int j = 0; j < 8; j++)
        printf("%6.3lf ", Element::Na_Eta(i,j));
      printf("|\n");
    } // for(int i = 0; i < 8; i++) {



    printf("\nNa_Zeta:\n");
    for(int i = 0; i < 8; i++) {
      printf("| ");
      for(int j = 0; j < 8; j++) printf("%6.3lf ", Element::Na_Zeta(i,j));
      printf("|\n");
    } // for(int i = 0; i < 8; i++) {
  #endif


  // Static members are now set.
  Element::Static_Members_Set = true;

  return SUCCESS;
} // Errors Set_Element_Static_Members(Matrix<unsigned> * ID_Ptr, Matrix<double> * K_Ptr, Node * Nodes_Ptr) {





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
