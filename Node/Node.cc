#if !defined(NODE_SOURCE)
#define NODE_SOURCE

#include "Node.h"
#include <stdio.h>

using namespace Node_Errors;

////////////////////////////////////////////////////////////////////////////////
// Setter methods

Errors Node::Update_Position(const unsigned component, const double New_Position) {
  /* Function Description:
  This function is used to update a particular component of the Node's position. */

  /* Assumption 1:
  This function assumes that this specific Node has been set up. */
  if(Node_Set_Up == false) {
    printf("Error in Update_Position\n");
    return NODE_NOT_SET_UP;
  } // if(Node_Set_Up == false) {

  /* Assumption 2:
  This functions also assumes that the specified index is either 0, 1, or 2
  since the Position array only has 3 components.*/
  if(component > 2) {
    printf("Error in Update_Position\n");
    return INDEX_OUT_OF_BOUNDS;
  } // if(component > 2) {

  // If if all of the assumptions have been satisified then update the current position
  Position[component] = New_Position;
  return SUCCESS;
} // Errors Node::Update_Position(const unsigned component, const double New_Position) {



Errors Node::Set_Original_Position(const Array_3<double> Original_Position_In) {
  /* Function Description:
  This function is used to set the original position of the Node. The original
  position is used to compute K. */

  /* Assumption 1:
  This function assumes that the node has not been set up already. */
  if(Node_Set_Up == true) {
    printf("Error in Set_Original_Position\n");
    return NODE_ALREADY_SET_UP;
  } // if(Node_Set_Up == true) {

  // Set the Original Position and Current_Position using the passed Array
  Original_Position = Original_Position_In;
  Position = Original_Position;

  // Set trivial BC's (assume that everything is free)
  Has_BC[0] = 0;
  Has_BC[1] = 0;
  Has_BC[2] = 0;

  // Original Position has been set
  Node_Set_Up = true;

  return SUCCESS;
} // Errors Node::Set_Original_Position(const Array_3<double> Original_Position_In) {



Errors Node::Set_BC(const unsigned Component, const double BC_In) {
  /* Function Description:
  This function is used to set the BC for a particular node. */

  /* Assumption 1:
  This function assumes that the node has been set up */
  if(Node_Set_Up == false) {
    printf("Error in Set_BC\n");
    return NODE_NOT_SET_UP;
  } // if(Node_Set_Up == false) {

  /* Assumption 2:
  This function assumes that the Component is 0, 1, or 2
  since the Position array only has 3 components.*/
  if(Component > 2) {
    printf("Error in Set_BC\n");
    return INDEX_OUT_OF_BOUNDS;
  } // if(component > 2) {

  /* Assumption 3:
  This function assumes that the specificed BC has not already been set */
  if(Has_BC[Component] == true) {
    printf("Error in Set_BC\n");
    return BC_ALREADY_SET;
  } // if(Has_BC[Component] == true) {

  // Set the BC.
  BC[Component] = BC_In;
  Has_BC[Component] = true;

  // All done!
  return SUCCESS;
} // Errors Node::Set_BC(const unsigned Component, const double BC_In) {



Errors Node::Update_Position_Using_BCs(void) {
  /* Function Description:
  This function is used to update the current position of the node using the
  Prescribed Position BCs. This function transfers any prescribed boundary BCs
  to the Position vector. This function, in general, should be called after
  each time step (when we're updating the nodal positions).

  This function also resets the BC's (Has_BC is set to all false) */

  /* Assumption 1:
  This function assumes that the node has been set up */
  if(Node_Set_Up == false) {
    printf("Error in Update_Position_From_BCs\n");
    return NODE_NOT_SET_UP;
  } // if(Node_Set_Up == false) {

  /* Cycle through the components. If a particular component has a BC, set
  the position to that value and then reset Has_BC. */
  for(int i = 0; i < 3; i++)
    if(Has_BC[i] == true) {
      Position[i] = BC[i];
      Has_BC[i] = false;
    } // if(Has_BC[i] == true) {

  // All done!
  return SUCCESS;
} // Errors Node::Update_Position_Using_BCs(void) {






////////////////////////////////////////////////////////////////////////////////
// Getter methods

Errors Node::Get_Has_BC(const unsigned index, bool & Has_Bc_Out) const {
  /* Function Description:
  This function is used to determine if a particular component of a Node's
  position is fixed (has a Prescribed displacement BC) or free. */

  /* Assumption 1:
  This function assumes that the Node has been set up. Otherwise, there's
  nothing to report */
  if(Node_Set_Up == false) {
    printf("Error in Get_Has_BC\n");
    return NODE_NOT_SET_UP;
  } // if(Node_Set_Up == false) {

  /* Assumption 2:
  This function assumes that the Index is either 0, 1, or 2. The Fixed_Pos
  array only has 3 elements, so anything with index > 2 is undefined */
  if(index > 2) {
    printf("Error in Get_Has_BC\n");
    return INDEX_OUT_OF_BOUNDS;
  } // if(index > 2) {

  Has_Bc_Out = Has_BC[index];

  return SUCCESS;
} // Errors Node::Get_Has_BC(const unsigned index, bool & Has_Bc_Out) const {



Errors Node::Get_Current_Position(Array_3<double> & Current_Position_Out) const {
  /* Function Description:
  This function is used to obtain the current position of the Node. */

  /* Assumption 1:
  This function assumes that this node has already been set up. If it hasn't,
  then there is no "Original position" to report. */
  if(Node_Set_Up == false) {
    printf("Error in Get_Current_Position\n");
    return NODE_NOT_SET_UP;
  } // if(Node_Set_Up == false) {

  // Copy position to Pos_Out variable
  Current_Position_Out = Position;

  return SUCCESS;
} // Errors Node::Get_Current_Position(Array_3<double> & Position_Out) const {



Errors Node::Get_Original_Position(Array_3<double> & Original_Position_Out) const {
  /* Function Description:
  This function is used to obtain the Original position of the Node. */

  /* Assumption 1:
  This function assumes that this node has already been set up. If it hasn't,
  then there is no "Original position" to report. */
  if(Node_Set_Up == false) {
    printf("Error in Get_Original_Position\n");
    return NODE_NOT_SET_UP;
  } // if(Node_Set_Up == false) {

  // Copy position to Pos_Out variable
  Original_Position_Out = Original_Position;

  return SUCCESS;
} // Errors Node::Get_Original_Position(Array_3<double> & Original_Position_Out) const {





////////////////////////////////////////////////////////////////////////////////
// Other methods

Errors Node::Print(void) const {
  /* Function Description:
  This function prints out information about the node. */

  /* Assumption 1:
  This function assumes that this node has already been set up. IF it hasn't,
  then there's nothing to print. */
  if(Node_Set_Up == false) {
    printf("Error in Print\n");
    return NODE_NOT_SET_UP;
  } // if(Node_Set_Up == false) {

  printf(         "Original Position   :         [%6.3lf, %6.3lf, %6.3lf]\n", Original_Position[0],  Original_Position[1], Original_Position[2]);
  printf(         "Position            :         [%6.3lf, %6.3lf, %6.3lf]\n", Position[0], Position[1], Position[2]);
  printf(         "BC                  :         [");
    for(int i = 0; i < 3; i++) {
      // Print true if this component of the position is fixed, false otherwise
      if(Has_BC[i] == false)
        printf("none   ");
      else
        printf("%6.3lf ", BC[i]);

      // Print comment
      if(i != 2)
        printf(", ");
    } // for(int i = 0; i < 3; i++) {
    printf("]\n");

  return SUCCESS;
} // Errors Node::Print(void) const {


#endif
