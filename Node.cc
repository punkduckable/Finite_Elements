#if !defined(NODE_SOURCE)
#define NODE_SOURCE

#include "Node.h"
#include <stdio.h>

using namespace Node_Errors;

////////////////////////////////////////////////////////////////////////////////
// Setter methods

Errors Node::Update_Position(const double New_Position_Component, const unsigned int component) {
  /* Assumption 1:

  This function assumes that this specific Node has been set up. */
  if(Node_Set_Up == false)
    return NODE_NOT_SET_UP;

  /* Assumption 2:
  This functions also assumes that the specified index is either 0, 1, or 2
  since the Position array only has 3 components.*/
  if(component > 2)
    return INDEX_OUT_OF_BOUNDS;

  /* Assumption 3:
  This function also assumes that the requested component is not fixed (a component of
  position with a prescribed displacement BC) since these components can not
  be modified. */
  if(Fixed_Pos[component] == true) {
    printf("Precribed boundary condition in the %d direction\n", component);
    return FIXED_POSITION_COMPONENT;
  } // if(Fixed_Pos[component] == true) {

  // If if all of the assumptions have been satisified then update the current position
  Current_Position[component] = New_Position_Component;
  return SUCCESS;
} // Errors Node::Update_Position(const double New_Position_Component, const unsigned int component) {


Errors Node::Set_Original_Position(const Array_3<double> Original_Position_In, const Array_3<bool> Fixed_Pos_In) {
  /* Assumption 1:

  This function assumes that the node has not been set up already. */
  if(Node_Set_Up == true)
    return NODE_ALREADY_SET_UP;

  // Set the Original Position using the passed Array
  Original_Position = Original_Position_In;

  // Set the Fixed Position array using the passed array
  Fixed_Pos = Fixed_Pos_In;

  // Original Position has been set
  Node_Set_Up = true;

  // Now, set the Current position to the original position
  Current_Position = Original_Position;

  return SUCCESS;
} // Errors Node::Set_Original_Position(const Array_3<double> Original_Position_In, const Array_3<bool> Fixed_Pos_In) {



////////////////////////////////////////////////////////////////////////////////
// Getter methods

Errors Node::Get_Original_Position(Array_3<double> & Original_Position_Out) const {
  /* Assumption 1:

  This function assumes that this node has already been set up. If it hasn't,
  then there is no "Original position" to report. */
  if(Node_Set_Up == false)
    return NODE_NOT_SET_UP;

  // Copy original position to Orig_Pos_Out variable
  Original_Position_Out = Original_Position;

  return SUCCESS;
} // Errors Node::Get_Original_Position(Array_3<double> & Orig_Position_Out) const {




Errors Node::Get_Current_Position(Array_3<double> & Current_Position_Out) const {
  /* Assumption 1:

  This function assumes that this node has already been set up. IF it hasn't,
  then there's no current position to report. */
  if(Node_Set_Up == false)
    return NODE_NOT_SET_UP;

  // Copy original position to Orig_Pos_Out variable.
  Current_Position_Out = Current_Position;

  return SUCCESS;
} // Errors Node::Get_Current_Position(Array_3<double> & Current_Position_Out) const {


////////////////////////////////////////////////////////////////////////////////
// Other methods

Errors Node::Print(void) const {
  /* Assumption 1:

  This function assumes that this node has already been set up. IF it hasn't,
  then there's nothing to print. */
  if(Node_Set_Up == false)
    return NODE_NOT_SET_UP;

  printf(         "Original Position:            [%6.3lf, %6.3lf, %6.3lf]\n", Original_Position[0],  Original_Position[1], Original_Position[2]);
  printf(         "Current Position :            [%6.3lf, %6.3lf, %6.3lf]\n", Current_Position[0] ,  Current_Position[1] , Current_Position[2] );
  printf(         "Fixed_Pos        :            [");
    for(int i = 0; i < 3; i++) {
      // Print true if this component of the position is fixed, false otherwise
      if(Fixed_Pos[i] == true)
        printf(" true ");
      else
        printf("false ");

      // Print comment
      if(i != 2)
        printf(", ");
    } // for(int i = 0; i < 3; i++) {
    printf("]\n");

  return SUCCESS;
} // Errors Node::Print(void) const {


#endif
