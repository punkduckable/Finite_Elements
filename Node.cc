#if !defined(NODE_SOURCE)
#define NODE_SOURCE

#include "Node.h"
#include <stdio.h>

using namespace Node_Errors;

////////////////////////////////////////////////////////////////////////////////
// Private methods

void Node::Set_Current_Position(void) {
  for(int i = 0; i < 3; i++) {
    if(Has_BCs[i] == true)
      Current_Position[i] = BCs[i];
    else
      Current_Position[i] = Original_Position[i];
  } // for(int i = 0; i < 3; i++) {
} // void Node::Set_Current_Position(void) {


////////////////////////////////////////////////////////////////////////////////
// Setter methods

Errors Node::Update_Position(const double New_Position_Component, const unsigned int component) {
  // First, check that the component is 0, 1, or 2
  if(component > 2) {
    return INDEX_OUT_OF_BOUNDS;
  } // if(component > 2) {

  // Next, check if this component has a BC
  if(Has_BCs[component] == true) {
    printf("Precribed boundary condition in the e%d direction\n", component+1);
    return BC_FIXED_POS;
  } // if(Has_BCs[component] == true) {

  // If both checks pass then update the current position
  Current_Position[component] = New_Position_Component;
  return SUCCESS;
} // Errors Node::Update_Position(const double New_Position_Component, const unsigned int component) {


Errors Node::Set_Original_Position(const Array_3<double> Original_Position_In) {
  /* Check if the Origional Position has already been set. This should only be
  set once (when the node is first being created */
  if(Original_Position_Has_Been_Set == false) {
    // Set the Original Position using the passed Array
    Original_Position = Original_Position_In;

    // Original Position has been set
    Original_Position_Has_Been_Set = true;

    // Now, check if we're ready to set the Current_Position
    if(Original_Position_Has_Been_Set == true && BCs_Have_Been_Set == true)
      Set_Current_Position();

    return SUCCESS;
  } // if(Original_Position_Has_Been_Set == false) {
  else
    return ORIGINAL_POSITION_SET;
} // Errors Node::Set_Original_Position(const Array_3<double> Original_Position_In) {



Errors Node::Set_BCs(const Array_3<bool> Has_BCs_In, const Array_3<double> BCs_In) {
  /* Check if the Node's Boundary Conditions have already been set. They should
  only be set once when the Node is first being created. */
  if(BCs_Have_Been_Set == false) {
    // Set the Has_BC boolean array and the BC array using the inputs
    Has_BCs = Has_BCs_In;
    BCs = BCs_In;

    // The BC's have been set.
    BCs_Have_Been_Set = true;

    // Now, check if we're ready to set the Current_Position
    if(Original_Position_Has_Been_Set == true && BCs_Have_Been_Set == true)
      Set_Current_Position();

    return SUCCESS;
  } // if(BCs_Have_Been_Set == false) {
  else
    return BC_SET;
} // Errors Node::Set_BCs(const Array_3<bool> Has_BCs_In, const Array_3<double> BCs_In) {

////////////////////////////////////////////////////////////////////////////////
// Getter methods

Array_3<double> Node::Get_Original_Position(void) const {
  return Original_Position;
} // Array_3<double> Node::Get_Original_Position(void) const {



Array_3<double> Node::Get_Current_Position(void) const {
  return Current_Position;
} // Array_3<double> Node::Get_Current_Position(void) const {


////////////////////////////////////////////////////////////////////////////////
// Other methods

void Node::Print(void) {
  printf(         "Original Position:            [%6.3lf, %6.3lf, %6.3lf]\n", Original_Position[0],  Original_Position[1], Original_Position[2]);
  printf(         "Current Position :            [%6.3lf, %6.3lf, %6.3lf]\n", Current_Position[0] ,  Current_Position[1] , Current_Position[2] );
  printf(         "BC's             :            [");
    for(int i = 0; i < 3; i++) {
      // Print this component's BC
      if(Has_BCs[i] == true)
        printf("%6.3lf", BCs[i]);
      else
        printf(" None ");

      // Print comment
      if(i != 2)
        printf(", ");
    }
    printf("]\n");
} // void Node::Print(void) {


#endif
