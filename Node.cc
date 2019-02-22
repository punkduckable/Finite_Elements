#if !defined(NODE_SOURCE)
#define NODE_SOURCE

#include "Node.h"
#include <stdio.h>

using namespace Node_Errors;

////////////////////////////////////////////////////////////////////////////////
// Setter methods

Errors Node::Update_Position(const unsigned component, const double New_Position) {
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
  Position[component] = New_Position;
  return SUCCESS;
} // Errors Node::Update_Position(const unsigned component, const double New_Position) {



Errors Node::Set_Position(const Array_3<double> Position_In, const Array_3<bool> Fixed_Pos_In) {
  /* Assumption 1:

  This function assumes that the node has not been set up already. */
  if(Node_Set_Up == true)
    return NODE_ALREADY_SET_UP;

  // Set the Original Position using the passed Array
  Position = Position_In;

  // Set the Fixed Position array using the passed array
  Fixed_Pos = Fixed_Pos_In;

  // Original Position has been set
  Node_Set_Up = true;

  return SUCCESS;
} // Errors Node::Set_Original_Position(const Array_3<double> Position_In, const Array_3<bool> Fixed_Pos_In) {



////////////////////////////////////////////////////////////////////////////////
// Getter methods

Errors Node::Get_Is_Fixed(const unsigned index, bool & Is_Fixed_Out) const {
  /* Assumption 1:
  This function assumes that the Node has been set up. Otherwise, there's
  nothing to report */
  if(Node_Set_Up == false)
    return NODE_NOT_SET_UP;

  /* Assumption 2:
  This function assumes that the Index is either 0, 1, or 2. The Fixed_Pos
  array only has 3 elements, so anything with index > 2 is undefined */
  if(index > 2)
    return INDEX_OUT_OF_BOUNDS;

  Is_Fixed_Out = Fixed_Pos[index];

  return SUCCESS;
} // Errors Node::Get_Is_Fixed(const unsigned index, bool & Is_Fixed_Out) const {



Errors Node::Get_Position(Array_3<double> & Position_Out) const {
  /* Assumption 1:

  This function assumes that this node has already been set up. If it hasn't,
  then there is no "Original position" to report. */
  if(Node_Set_Up == false)
    return NODE_NOT_SET_UP;

  // Copy position to Pos_Out variable
  Position_Out = Position;

  return SUCCESS;
} // Errors Node::Get_Original_Position(Array_3<double> & Position_Out) const {



////////////////////////////////////////////////////////////////////////////////
// Other methods

Errors Node::Print(void) const {
  /* Assumption 1:

  This function assumes that this node has already been set up. IF it hasn't,
  then there's nothing to print. */
  if(Node_Set_Up == false)
    return NODE_NOT_SET_UP;

  printf(         "Position :          [%6.3lf, %6.3lf, %6.3lf]\n", Position[0],  Position[1], Position[2]);
  printf(         "Fixed_Pos :         [");
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
