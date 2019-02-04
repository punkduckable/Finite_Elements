#if !defined(NODE_SOURCE)
#define NODE_SOURCE

#include "Node.h"
#include <stdio.h>

using namespace Node_Errors;

////////////////////////////////////////////////////////////////////////////////
// Setter methods

Errors Node::Update_Position(const double New_Position_Component, const unsigned int component) {
  // Make sure that the node has been set up. If not, throw an error
  if(Original_Position_Has_Been_Set == false)
    return NODE_NOT_SET_UP;

  // Check that the component is 0, 1, or 2
  if(component > 2)
    return INDEX_OUT_OF_BOUNDS;

  // Next, check if this component is fixed (prescribed BC)
  if(Fixed_Pos[component] == true) {
    printf("Precribed boundary condition in the %d direction\n", component);
    return FIXED_POSITION_COMPONENT;
  } // if(Fixed_Pos[component] == true) {

  // If both checks pass then update the current position
  Current_Position[component] = New_Position_Component;
  return SUCCESS;
} // Errors Node::Update_Position(const double New_Position_Component, const unsigned int component) {


Errors Node::Set_Original_Position(const Array_3<double> Original_Position_In, const Array_3<bool> Fixed_Pos_In) {
  /* Check if the Origional Position has already been set. This should only be
  set once (when the node is first being created */
  if(Original_Position_Has_Been_Set == true)
    return ORIGINAL_POSITION_SET;

  // Set the Original Position using the passed Array
  Original_Position = Original_Position_In;

  // Set the Fixed Position array using the passed array
  Fixed_Pos = Fixed_Pos_In;

  // Original Position has been set
  Original_Position_Has_Been_Set = true;

  // Now, set the Current position to the original position
  Current_Position = Original_Position;

  return SUCCESS;
} // Errors Node::Set_Original_Position(const Array_3<double> Original_Position_In, const Array_3<bool> Fixed_Pos_In) {



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
  printf(         "Fixed_Pos        :            [");
    for(int i = 0; i < 3; i++) {
      // Print if this component of the position is fixed
      if(Fixed_Pos[i] == true)
        printf(" true ");
      else
        printf("false ");

      // Print comment
      if(i != 2)
        printf(", ");
    }
    printf("]\n");
} // void Node::Print(void) {


#endif
