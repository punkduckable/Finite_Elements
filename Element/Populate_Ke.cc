#if !defined(ELEMENT_POPULATE_KE)
#define ELEMENT_POPULATE_KE

/* File description:
This file holds all of the functions that are involved in Populating the
Element stiffness matrix. */

#include "Element.h"
#include <stdio.h>

using namespace Element_Errors;

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

#endif
