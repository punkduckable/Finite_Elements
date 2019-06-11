#if !defined(ELEMENT_FE)

/* File description:
This file holds all of the functions that work with the element Force vector, Fe.
This includes functions used to populate Fe, as well as a function to move Fe
to F. */

#include "Element.h"
#include <stdio.h>
//#define FE_MONITOR                     // Prints Fe


void Element::Populate_Fe(void) {
  /* Function Description:
  This function is used to populate Fe, the local force vector.*/

  /* Assumption 1:
  This function assumes that Ke has been populated. This can be determined
  by the "Ke_Set_Up" flag. */
  if(Ke_Set_Up == false) {
    char Error_Message_Buffer[500];
    sprintf(Error_Message_Buffer,
            "Element Not Set Up Exception: Thrown by Element::Populate_Fe\n"
            "Ke must be computed before computing Fe. You must run Populate_Ke\n"
            "BEFORE you can run Populate_Fe\n");
    throw Element_Not_Set_Up(Error_Message_Buffer);
  } // if(Ke_Set_Up == false) {


  /* Assumption 2:
  This function assumes that Fe has not already been set up. This can be
  determined with the "Fe_Set_Up" flag */
  if(Fe_Set_Up == true) {
    char Error_Message_Buffer[500];
    sprintf(Error_Message_Buffer,
            "Element Already Set Up Exception: Thrown by Element::Populate_Fe\n"
            "Fe has already been set up and shoud only be set once per iteration.\n");
    throw Element_Already_Set_Up(Error_Message_Buffer);
  } // if(Fe_Set_Up == true) {


  //////////////////////////////////////////////////////////////////////////////
  // Calculate Ke by looping through the 24 local equations
  for(int i = 0; i < 24; i++) {
    // Set Fe to Zero to start
    Fe[i] = 0;

    /* Now, cycle through the 24 equations. If the jth local equation
    corresponds to a fixed position then add its contribution to Fe.
    Note: Fe[i] = Sum(over equations corresponding to prescribed positions of -Ke[i,j]*Prescribed_Positions[j]) */
    for(int j = 0; j < 24; j++) {
      if(Local_Eq_Num_To_Global_Eq_Num[j] == FIXED_COMPONENT)
        Fe[i] -= Ke(i,j)*Prescribed_Positions[j];
    } // for(int j = 0; j < 24; j++) {
  } // for(int i = 0; i < 24; i++) {

  // Fe is now set up
  Fe_Set_Up = true;


  #if defined(FE_MONITOR)
    printf("FE = |");
    for(int i = 0; i < 24; i++)
      printf(" %6.3lf", Fe[i]);
    printf("|\n");
  #endif
} // void Elemnet::Populate_Fe(void) {




void Element::Move_Fe_To_F(void) const {
  /* Function description
  This function maps the local force vector, Fe, to the global force vector, F */

  /* Assumption 1
  This functiona assumes that the local force vector, Fe, has been set.

  This function also also assumes that the Element class has access to the
  global force vector, F. This occurs when the Element Class's static members
  are set. Luckily, it is not possible for Fe to be populated unless the
  static members have been set. Therefore, we only need to check if Ke has been
  set to validate both assumptions */
  if(Fe_Set_Up == false) {
    char Error_Message_Buffer[500];
    sprintf(Error_Message_Buffer,
           "Element Not Set Up Exception: Thrown by Element::Move_Fe_To_F\n"
           "You must compute Fe before you can map Fe to F. Populate_Fe must\n"
           "be run BEFORE Move_Fe_to_F\n");
    throw Element_Not_Set_Up(Error_Message_Buffer);
  } // if(Fe_Set_Up == false) {

  //////////////////////////////////////////////////////////////////////////////
  // Add the local contributions to the force vector (Fe) to F.
  for(int i = 0; i < 24; i++) {
    const unsigned I = Local_Eq_Num_To_Global_Eq_Num[i];
    if(I == FIXED_COMPONENT)
      continue;
    else
      F[I] += Fe[i];
  } // for(int i = 0; i < 24; i++) {
} // void Element::Move_Fe_To_F(void) const {


#endif
