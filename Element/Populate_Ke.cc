#if !defined(ELEMENT_POPULATE_KE)
#define ELEMENT_POPULATE_KE

/* File description:
This file holds all of the functions that are involved in Populating the
Element stiffness matrix. */

#include "Element.h"
#include <stdio.h>
//#define POPULATE_KE_MONITOR

using namespace Element_Errors;



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
  means that Xa, Ya, and Za have all been populated. This is done in the
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

  #if defined(POPULATE_KE_MONITOR)
    printf("Coeff:\n");
    for(int i = 0; i < 3; i++) {
      printf("| ");
      for(int j = 0; j < 3; j++)
        printf("%9.3e ", Coeff(i,j));
      printf("|\n");
    } // for(int i = 0; i < 3; i++) {

    printf("J = %6.3lf\n\n", J);
  #endif
} // void Element::Calculate_Coefficient_Matrix(const unsigned Point, Matrix<double> & Coeff, double & J) {



void Element::Add_Ba_To_B(const unsigned Node, const unsigned Point, const Matrix<double> & Coeff, const double J, Matrix<double> & B) {
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
  double Na_x = (Na_Xi(Node, Point)*Coeff(1,1) + Na_Eta(Node, Point)*Coeff(1,2) + Na_Zeta(Node, Point)*Coeff(1,3))/J;
  double Na_y = (Na_Xi(Node, Point)*Coeff(2,1) + Na_Eta(Node, Point)*Coeff(2,2) + Na_Zeta(Node, Point)*Coeff(2,3))/J;
  double Na_z = (Na_Xi(Node, Point)*Coeff(3,1) + Na_Eta(Node, Point)*Coeff(3,2) + Na_Zeta(Node, Point)*Coeff(3,3))/J;

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

  #if defined(POPULATE_KE_MONITOR)
    printf("Ba_T:\n");
    for(int i = 0; i < 3; i++) {
      printf("| ");
      for(int j = 0; j < 6; j++)
        printf("%9.3e ", Ba[i*6 + j];
      printf("|\n");
    } // for(int i = 0; i < 3; i++) {
  #endif
} // void Element::Add_Ba_To_B(const unsigned Node, const unsigned Point, const Matrix<double> & Coeff, const double J, Matrix<double> & B) {



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
  // Cycle through the 8 Integration points

  // First, declare J, Coeff, and jD
  double J;
  class Matrix<double> Coeff = Matrix<double>(3, 3, Memory::ROW_MAJOR);
  class Matrix<double> JD    = Matrix<double>(6, 6, Memory::ROW_MAJOR);

  for(int Point = 0; Point < 8; Point++) {
    // Find coefficient matrix, J.
    Calculate_Coefficient_Matrix(Point, Coeff, J);

    // Make sure that J is not zero.
    if(J == 0) {
      printf("Error in Populate_Ke!\n");
      return ZERO_DETERMINANT;
    } // if(J == 0) {

    // Calculate j*D
    for(int i = 0; i < 6; i++)
      for(int j = 0; j < 6; j++)
        JD(i,j) = J*D(i,j);

    // Declare B
    class Matrix<double> B = Matrix<double>(6, 24, Memory::COLUMN_MAJOR);

    // Now construct B
    for(int Node = 0; Node < 8; Node++)
      Add_Ba_To_B(Node, Point, Coeff, J, B);


  } // for(int Point = 0; Point < 8; Point++) {

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

#endif
