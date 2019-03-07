#if !defined(ELEMENT_SETUP_SOURCE)
#define ELEMENT_SETUP_SOURCE

/* File description:
This file stores all of the functions that help set up the Element class.
These functions are friends of the Element class and set the Element class'
static members. */

#include "Element.h"
#include <stdio.h>
#define SETUP_MONITOR

using namespace Element_Errors;



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
  for(int Point = 0; Point < 8; Point++) {
    for(int Node = 0; Node < 8; Node++) {
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
    } // for(int Node = 0; Node < 8; Node++) {
  } // for(int Point = 0; Point < 8; Point++) {

  #if defined(SETUP_MONITOR)
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



Errors Set_Element_Material(const double E, const double v) {
  /* Function description:
  This function is designed to set up the D matrix for the element class. This
  matrix is a reduced form of the Elasticity tensor C. D is constructed from C
  using the symmetry of C. See my lecture notes section 2.7.4 for a proper
  derivation of D (the book sucks at deriving D!). We construct D assuming that
  the material is isotropic and homogeneous.

  The input parameter E is the Young's modulus for the material
  The input parameter v is the Poisson's ratio for the material */

  /* Assumption 1:
  We assume that the Element material has not already been set */
  if(Element::Material_Set == true) {
    printf("Error in Set_Element_Material\n");
    return MATERIAL_ALREADY_SET;
  } // if(Element::Material_Set == true) {

  // First, let's calculate lambda and mu.
  const double l = v*E/((1 + v)*(1 - 2*v));
  const double m = E/(2*(1 + v));

  /* Now let's populate D. To make this code easier to read, I decided to
  first declare D as a regular C array (so that I can initialize it) and then
  copy this Array to D. This is not the most efficient way to do this, but
  since this function only runs once, it shouldn't really matter. */
  const double D_Array[36] = { l+2*m,   l  ,   l  ,   0  ,   0  ,   0  ,
                                 l  , l+2*m,   l  ,   0  ,   0  ,   0  ,
                                 l  ,   l  , l+2*m,   0  ,   0  ,   0  ,
                                 0  ,   0  ,   0  ,   m  ,   0  ,   0  ,
                                 0  ,   0  ,   0  ,   0  ,   m  ,   0  ,
                                 0  ,   0  ,   0  ,   0  ,   0  ,   m   };

  // Copy D_Array to D
  for(int i = 0; i < 6; i++)
    for(int j = 0; j < 6; j++)
      Element::D(i,j) = D_Array[i*6 + j];

  // Material has now been set
  Element::Material_Set = true;

  #if defined(SETUP_MONITOR)
    printf("D:\n");
    for(int i = 0; i < 6; i++) {
      printf("| ");
      for(int j = 0; j < 6; j++)
        printf("%9.3e ", Element::D(i,j));
      printf("|\n");
    } // for(int i = 0; i < 6; i++) {
  #endif

  return SUCCESS;
} // Errors Set_Element_Material(const double E, const double v) {

#endif
