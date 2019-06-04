#if !defined(ELEMENT_KE)
#define ELEMENT_KE

/* File description:
This file holds all of the functions that work with the Element Stiffness
matrix. This includes the functions required to Populate Ke, as well as
the functions requried to move Ke to K. */

#include "Element.h"
#include <stdio.h>
//#define COEFFICIENT_MATRIX_MONITOR     // Prints Coeff, J, and Xi, Eta, Zeta partials of x,y,z
//#define BA_MONITOR                     // Prints each Ba (used to construct B)
//#define POPULATE_KE_MONITOR            // Prints JD, B, JD*B and B^T*JD*B
//#define KE_MONITOR                     // Prints Ke



void Element::Calculate_Coefficient_Matrix(const unsigned Point, Matrix<double> & Coeff, double & J) {
  /* Function description:
    This function calculates the coefficient matrix and jacobian determinant
    for a specific integration point. */


  /* Assumption 1:
  This function assumes that "Point" is the index of an integration point.
  Therefore, we assume that Point is in the set {0,1,2... 7}.

  Since this private method is only called by the Populate_Ke method, and that
  method should only call this function with indicies of 0, 1, 2... 7, we will
  assume that this assumption is valid.*/


  /* Assumption 2:
  This function assumes that the spatial position of each Node is known. This
  means that the Element_Node array has been populated. This is done in the
  "Set_Up_Element" method.

  Since this private method is only called by the Populate_Ke method, and
  that method can not run unless the Element is set up, we assume that
  this assumption is valid. */


  /* Assumption 3:
  This function assumes that the Xi, Eta, and Zeta partial derivatives for
  each shape function in the master element has been calculated. These
  quantities are calculated by the "Set_Element_Static_Members" method.

  Since this private method is only called by Populate_Ke, this assumption
  should be valid */

  //////////////////////////////////////////////////////////////////////////////

  /* First, calculate the x, y, and z partials (with respect to Xi, Eta, and
  Zeta) at the passed Integration point. */
  double x_Xi = 0, x_Eta = 0, x_Zeta = 0;
  double y_Xi = 0, y_Eta = 0, y_Zeta = 0;
  double z_Xi = 0, z_Eta = 0, z_Zeta = 0;

  for(int Node = 0; Node < 8; Node++) {
    x_Xi   += Na_Xi  (Node, Point)*Element_Nodes[Node].Xa;
    x_Eta  += Na_Eta (Node, Point)*Element_Nodes[Node].Xa;
    x_Zeta += Na_Zeta(Node, Point)*Element_Nodes[Node].Xa;

    y_Xi   += Na_Xi  (Node, Point)*Element_Nodes[Node].Ya;
    y_Eta  += Na_Eta (Node, Point)*Element_Nodes[Node].Ya;
    y_Zeta += Na_Zeta(Node, Point)*Element_Nodes[Node].Ya;

    z_Xi   += Na_Xi  (Node, Point)*Element_Nodes[Node].Za;
    z_Eta  += Na_Eta (Node, Point)*Element_Nodes[Node].Za;
    z_Zeta += Na_Zeta(Node, Point)*Element_Nodes[Node].Za;
  } // for(int Node = 0; Node < 8; Node++) {


  /* Now, compute the components of the coefficient matrix. This is done using
  the steps outlined on page 150 of Hughes' book. */
  Coeff(0,0) = y_Eta*z_Zeta - y_Zeta*z_Eta;
  Coeff(0,1) = y_Zeta*z_Xi  - y_Xi*z_Zeta;
  Coeff(0,2) = y_Xi*z_Eta   - y_Eta*z_Xi;

  Coeff(1,0) = z_Eta*x_Zeta - z_Zeta*x_Eta;
  Coeff(1,1) = x_Xi*z_Zeta  - x_Zeta*z_Xi;
  Coeff(1,2) = z_Xi*x_Eta   - z_Eta*x_Xi;

  Coeff(2,0) = x_Eta*y_Zeta - x_Zeta*y_Eta;
  Coeff(2,1) = x_Zeta*y_Xi  - x_Xi*y_Zeta;
  Coeff(2,2) = x_Xi*y_Eta   - x_Eta*y_Xi;

  /* Finally, calculate J (the jacobian determinant) */
  J = x_Xi*Coeff(0,0) + x_Eta*Coeff(0,1) + x_Zeta*Coeff(0,2);

  #if defined(COEFFICIENT_MATRIX_MONITOR)
    printf("x_Xi = %6.3lf   x_Eta = %6.3lf   x_Zeta = %6.3lf\n", x_Xi, x_Eta, x_Zeta);
    printf("y_Xi = %6.3lf   y_Eta = %6.3lf   y_Zeta = %6.3lf\n", y_Xi, y_Eta, y_Zeta);
    printf("z_Xi = %6.3lf   z_Eta = %6.3lf   z_Zeta = %6.3lf\n", z_Xi, z_Eta, z_Zeta);

    printf("Coeff:\n");
    for(int i = 0; i < 3; i++) {
      printf("| ");
      for(int j = 0; j < 3; j++)
        printf("%10.3e ", Coeff(i,j));
      printf("|\n");
    } // for(int i = 0; i < 3; i++) {

    printf("J = %10.3e\n\n", J);
  #endif
} // void Element::Calculate_Coefficient_Matrix(const unsigned Point, Matrix<double> & Coeff, double & J) {



void Element::Add_Ba_To_B(const unsigned Node, const unsigned Integration_Point, const Matrix<double> & Coeff, const double J, Matrix<double> & B) {
  /* Function descrpition.
  This function is used to calculate Ba and move Ba into B. This is done using
  the equations on page 150 of Hughes' book and the definition of B, Ba on page
  87 */


  /* Assumption 1:
  This function assumes that the Node index is in {0, 1, 2,... 7}. This is
  because those are the only nodes in the Master element. Anything outside
  of this range would cause an index out of bounds error when reading from
  Na_Xi, Na_Eta, and Na_Zeta.

  Since this private method is only called by the Populate_Ke method, and
  that method has been written to only call this function with Node indicies
  in {0,1,2... 7}, we will assume that this assumption is valid */


  /* Assumption 2:
  This function assumes that B has been set up (right dimensions and such) and
  that the Coefficient matrix has been populated (by the
  Calculate_Coefficient_Matrix method).

  We have no way of testing this assumption. However, since this private method
  is only called by the Populate_Ke method, and that method only calls this
  function once B has been defined and Coeff has been populated, we will assume
  that this assumption is valid. */


  /* Assumption 3:
  This function assumes that Na_Xi, Na_Eta, and Na_Zeta have been set up. This
  happens when the Element static members are set (by Set_Element_Static_Members)

  Since this private method is only called by the Populate_Ke method, and that
  method can't run unless the static members have been set, we should be
  justified in assuming that this assumption is valid. */


  //////////////////////////////////////////////////////////////////////////////
  /* First, calculate Na_x, Na_y, Na_z. This is done using the equations on page
  150 of Hughes' book */
  double Na_x = (Na_Xi(Node, Integration_Point)*Coeff(0,0) + Na_Eta(Node, Integration_Point)*Coeff(0,1) + Na_Zeta(Node, Integration_Point)*Coeff(0,2))/J;
  double Na_y = (Na_Xi(Node, Integration_Point)*Coeff(1,0) + Na_Eta(Node, Integration_Point)*Coeff(1,1) + Na_Zeta(Node, Integration_Point)*Coeff(1,2))/J;
  double Na_z = (Na_Xi(Node, Integration_Point)*Coeff(2,0) + Na_Eta(Node, Integration_Point)*Coeff(2,1) + Na_Zeta(Node, Integration_Point)*Coeff(2,2))/J;

  /* Now, use these to make Ba_T
  We construct Ba_T rather than Ba because B is stored in Column major order.
  As a reault, to improve runtime, we want to move Ba into B in a
  column-by-column manner. The issue is that, to further reduce overhead, Ba
  is implemented as a 1d array rather than a full blown Matrix. Thus, we're
  essentially to use Row_Major ordering (by this, I mean that I can only format
  the initlaization statement for Ba to be in Row_Major order). Therefore, I
  really need to store Ba_T, rather than Ba. This allows me to move a row of
  Ba_T into a column of B, making the best use of cache lines. */
  const double Ba_T[18] = { Na_x, 0   , 0   , 0   , Na_z, Na_y,
                            0   , Na_y, 0   , Na_z, 0   , Na_x,
                            0   , 0   , Na_z, Na_y, Na_x, 0   };

  /* Now, move Ba_T into B. We do this on a column-by-column basis since B is
  stored in Column major order. */
  for(int j = 0; j < 3; j++)
    for(int i = 0; i < 6; i++)
      B(i, j + 3*Node) = Ba_T[j*6 + i];

  #if defined(BA_MONITOR)
    printf("Ba_T:\n");
    for(int i = 0; i < 3; i++) {
      printf("| ");
      for(int j = 0; j < 6; j++)
        printf("%10.2e ", Ba_T[i*6 + j]);
      printf("|\n");
    } // for(int i = 0; i < 3; i++) {
  #endif
} // void Element::Add_Ba_To_B(const unsigned Node, const unsigned Integration_Point, const Matrix<double> & Coeff, const double J, Matrix<double> & B) {



void Element::Populate_Ke(void) {
  /* Function description:
  This method is used to populate Ke, the element stiffness matrix. Once
  this method has run, Ke can be mapped to K (and Fe can be calculated). */


  /* Assumption 1:
  This function assumes that the nodes in the Node_List are in a particular
  order. Specifically, we assume that the passed nodes are in the same order
  as the figure on page 123 of Hughes' book.

  We have no way of testing and/or verrifying this assumption. Therefore, we
  simply assume that the user set up the node list in the correct order. */


  /* Assumption 2:
  This function assumes that this Element has been set up. More specificially,
  this function assumes that the node list and node positions have been set.*/
  if(Element_Set_Up == false) {
    char Error_Message_Buffer[500];
    sprintf(Error_Message_Buffer,
            "Element Not Set Up Exception: Thrown by Element::Populate_Ke\n"
            "it is impossible to calculate Ke if the element's node list has\n"
            "not been set. Set_Nodes must be run BEFORE Populate_Ke.\n");
    throw Element_Not_Set_Up(Error_Message_Buffer);
  } // if(Element_Set_Up == false) {


  /* Assumption 3:
  This function assumes that D has been set. This can be tested with the
  "Material_Set" flag. */
  if(Material_Set == false) {
    char Error_Message_Buffer[500];
    sprintf(Error_Message_Buffer,
            "Element Not Set Up Exception: Thrown by Element::Populate_Ke\n"
            "Ke depends on D. Thus, the element material must be set before\n"
            "calculating Ke.\n");
    throw Element_Not_Set_Up(Error_Message_Buffer);
  } // if(Material_Set == false) {


  /* Assumption 4:
  Finally, This function assumes that Ke has not been set already. */
  if(Ke_Set_Up == true) {
    char Error_Message_Buffer[500];
    sprintf(Error_Message_Buffer,
            "Element Already Set Exception: Thrown by Element::Populate_Ke\n"
            "Once Ke has been calculated, it can not be recalculated.\n");
    throw Element_Already_Set_Up(Error_Message_Buffer);
  } // if(Ke_Set_Up == true) {


  //////////////////////////////////////////////////////////////////////////////

  // First, zero out KE
  Ke.Zero();

  // Now, cycle through the 8 Integration points

  // First, declare J, Coeff, and jD
  double J;
  class Matrix<double> Coeff{3, 3, Memory::ROW_MAJOR};
  class Matrix<double> JD{6, 6, Memory::ROW_MAJOR};

  for(int Point = 0; Point < 8; Point++) {
    // Find coefficient matrix, J.
    Calculate_Coefficient_Matrix(Point, Coeff, J);

    // Make sure that J is not zero. If it is then throw an exception.
    if(J <= 0) {
      char Error_Message_Buffer[500];
      sprintf(Error_Message_Buffer,
              "Element Bad Determinant Exception: Thrown in Element::Populate_Ke\n"
              "The Jacobian determinant, J, must be a strictly positive quantity. However,\n"
              "when calculating J for integration point %d, we got J = %lf.\n",
              Point, J);
      throw Element_Bad_Determinant(Error_Message_Buffer);
    } // if(J == 0) {

    /* Calculate J*D
    Note: D is a row-major matrix, so the product J*D will be stored as a
    Row-major matrix as well (see Matrix class implementation) */
    JD = J*D;


    // Declare B
    class Matrix<double> B{6, 24, Memory::COLUMN_MAJOR};

    // Now construct B
    for(int Node = 0; Node < 8; Node++)
      Add_Ba_To_B(Node, Point, Coeff, J, B);

    // Calculate JD*B
    class Matrix<double> JD_B{6, 24, Memory::COLUMN_MAJOR};

    for(int j = 0; j < 24; j++) {
      for(int i = 0; i < 6; i++) {
        JD_B(i,j) = 0;

        for(int k = 0; k < 6; k++)
          JD_B(i,j) += JD(i,k)*B(k,j);
      } // for(int i = 0; i < 6; i++) {
    } // for(int j = 0; j < 24; j++) {

    /* Now compute B^T*JD*B (this will be added into Ke).
    We expect this matrix to be symmetric. Therefore to minimuze computations,
    we first populate the main diagional of BT_JD_B, and then the off diagional
    parts (by computing the (i,j) cell of BT_JD_B and then moving it into
    the (j,i) cell. */
    class Matrix<double> BT_JD_B{24, 24, Memory::COLUMN_MAJOR};

    // Populate diagional cells of BT_JD_B
    for(int j = 0; j < 24; j++) {
      BT_JD_B(j,j) = 0;
      for(int k = 0; k < 6; k++)
        BT_JD_B(j,j) += B(k,j)*JD_B(k,j);
    } // for(int j = 0; j < 24; j++) {

    // Populate the off diagional cells of BT_JD_B (accounting for symmetry)
    for(int j = 0; j < 24; j++) {
      for(int i = j+1; i < 24; i++) {
        // Calculate the i,j cell.
        BT_JD_B(i,j) = 0;
        for(int k = 0; k < 6; k++)
          BT_JD_B(i,j) += B(k,i)*JD_B(k,j);

        // Now set (j,i) cell using symmetry
        BT_JD_B(j,i) = BT_JD_B(i,j);
      } // for(int i = 0; i < 24; i++) {
    } // for(int j = 0; j < 6; j++) {

    // Now move BT_JD_B to KE.
    for(int j = 0; j < 24; j++)
      for(int i = 0; i < 24; i++)
        Ke(i,j) += BT_JD_B(j,i);



    #if defined(POPULATE_KE_MONITOR)
      printf("JD:\n");
      for(int i = 0; i < 6; i++) {
        printf("| ");
        for(int j = 0; j < 6; j++)
          printf("%5.2lf ", JD(i,j));
        printf("|\n");
      } // for(int i = 0; i < 6; i++) {

      printf("B:\n");
      for(int i = 0; i < 6; i++) {
        printf("| ");
        for(int j = 0; j < 24; j++)
          printf("%5.2lf ", B(i,j));
        printf("|\n");
      } // for(int i = 0; i < 6; i++) {

      printf("JD_B:\n");
      for(int i = 0; i < 6; i++) {
        printf("| ");
        for(int j = 0; j < 24; j++)
          printf("%5.2lf ", JD_B(i,j));
        printf("|\n");
      } // for(int i = 0; i < 6; i++) {

      printf("BT_JD_B:\n");
      for(int i = 0; i < 24; i++) {
        printf("| ");
        for(int j = 0; j < 24; j++)
          printf("%5.2lf ", BT_JD_B(i,j));
        printf("|\n");
      } // for(int i = 0; i < 24; i++) {
    #endif
  } // for(int Point = 0; Point < 8; Point++) {

  // Ke has now been set
  Ke_Set_Up = true;

  #if defined(KE_MONITOR)
    printf("Element stiffness matrix:\n");
    for(int i = 0; i < 24; i++) {
      printf("| ");

      for(int j = 0; j < 24; j++)
        printf("%8.1e ", Ke(i,j));

      printf("|\n");
    } // for(int i = 0; i < 24, i++) {
  #endif
} // void Element::Populate_Ke(void) {



void Element::Fill_Ke_With_1s(void) {
  /* Function description:
  This function is used for testing purposes. It sets every element of Ke to 1.
  Once this has been done, Ke can be mapped to K. This is used to test that
  the assembly proceedure is working correctly */


  /* Assumption 1:
  This function assumes that this Element has been set up. More specificially,
  this function assumes that the node list and node positions have been set.*/
  if(Element_Set_Up == false) {
    char Error_Message_Buffer[500];
    sprintf(Error_Message_Buffer,
            "Element Not Set Up Exception: Thrown by Element::Fill_Ke_With_1s\n"
            "it is impossible to calculate Ke if the element's node list has\n"
            "not been set. Set_Nodes must be run BEFORE Fill_Ke_With_1s.\n");
    throw Element_Not_Set_Up(Error_Message_Buffer);
  } // if(Element_Set_Up == false) {


  /* Assumption 2:
  Finally, This function assumes that Ke has not been set already. */
  if(Ke_Set_Up == true) {
    char Error_Message_Buffer[500];
    sprintf(Error_Message_Buffer,
            "Element Already Set Exception: Thrown by Element::Fill_Ke_With_1s\n"
            "Once Ke has been calculated, it can not be recalculated.\n");
    throw Element_Already_Set_Up(Error_Message_Buffer);
  } // if(Ke_Set_Up == true) {


  //////////////////////////////////////////////////////////////////////////////

  // Fill Ke's with 1's (note: Ke is column major)
  for(int j = 0; j < 24; j++)
    for(int i = 0; i < 24; i++)
      Ke(i,j) = 1;

  // Ke has now been set
  Ke_Set_Up = true;

  #if defined(KE_MONITOR)
    printf("Element stiffness matrix:\n");
    for(int i = 0; i < 24; i++) {
      printf("| ");

      for(int j = 0; j < 24; j++)
        printf("%6.2lf ", Ke(i,j));

      printf("|\n");
    } // for(int i = 0; i < 24, i++) {
  #endif
} // void Element::Fill_Ke_With_1s(void) {



void Element::Move_Ke_To_K(void) const {
  /* Function description:
  This fucntion is used to map the element stiffness matrix, Ke, to the global
  stiffness matrix, K. */

  /* Assumption 1:
  This function assumes that the element stiffness matrix, Ke, has been set.

  This function also assumes that the Element class static members, namely K,
  has been set.

  It is not possible, however, to set up Ke without having the Static members
  set. Therefore, if Ke is set then both assumptions must be satisified. */
  if(Ke_Set_Up == false) {
    char Error_Message_Buffer[500];
    sprintf(Error_Message_Buffer,
            "Element Not Set Up Exception: Thrown by Element::Move_Ke_To_K\n"
            "you have to calculate Ke before mapping Ke to K. Populate_Ke must\n"
            "be run BEFORE Move_Ke_To_K\n");
    throw Element_Not_Set_Up(Error_Message_Buffer);
  } // if(Ke_Set_Up == false) {



  /* First, move the diagional cells of Ke to K. We only move the components
  that correspond to a global equation. Recall that Ke has a row for each
  component of each of the 8 nodes in this Element's Node list, even though
  some of those components may be fixed. We kept track of this with the
  "FIXED_COMPONENT" constant wen we set up Local_Eq_Num_To_Global_Eq_Num */
  for(int i = 0; i < 24; i++) {
    const unsigned I = Local_Eq_Num_To_Global_Eq_Num[i];
    if(I == FIXED_COMPONENT)
      continue;
    else
      (*K)(I, I) += Ke(i,i);
  } // for(int i = 0; i < 24; i++) {

  /* Now, move the off-diagional cells of Ke to K. Again, We only move the
  components that correspond to a global equation (see previous comment) */
  for(int Col = 0; Col < 24; Col++) {
    // Get Global column number, J, associated with the local column number "Col"
    const unsigned J = Local_Eq_Num_To_Global_Eq_Num[Col];

    // Check if J corresponds to a fixed component
    if(J == FIXED_COMPONENT)
      continue;
    else
      for(int Row = Col+1; Row < 24; Row++) {
        // Get Global Row number, I, associated with the local row number "Row"
        const unsigned I = Local_Eq_Num_To_Global_Eq_Num[Row];

        // Check if I corresponds to a fixed component
        if(I == FIXED_COMPONENT)
          continue;

        // If not, move Ke(Row, Col) to the corresponding position in K.
        const double Ke_Row_Col = Ke(Row, Col);
        (*K)(I,J) += Ke_Row_Col;
        (*K)(J,I) += Ke_Row_Col;
      } // for(int Row = Col+1; Row < 24; Row++) {
  } // for(int Col = 0; Col < 24; Col++) {
} // void Element::Move_Ke_To_K(void) const {

#endif
