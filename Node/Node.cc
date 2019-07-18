#if !defined(NODE_SOURCE)
#define NODE_SOURCE

#include "Node.h"
#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////
// Constructor

Node::Node(void) {
  /* Normally, you would want the constructor to set up an object. Unfortuneatly,
  C++ sucks at initializing arrays of objects. Nodes are always created in
  arrays. Thus, we move the code that actually sets up the Node class to another
  method, Set_Original_Position.

  All that this constructor really does is give the Node trivial BC's (which
  can be overwritten using the "Set_BC" method. */

  // Set trivial BC's (assume that everything is free)
  Has_BC[0] = false;
  Has_BC[1] = false;
  Has_BC[2] = false;
} // Node::Node(void) {






////////////////////////////////////////////////////////////////////////////////
// Setter methods

void Node::Update_Position(const unsigned component, const double New_Position) {
  /* Function Description:
  This function is used to update a particular component of the Node's position. */

  /* Assumption 1:
  This function assumes that this specific Node has been set up. */
  if(Original_Position_Set_Up == false) {
    char Error_Message_Buffer[500];
    sprintf(Error_Message_Buffer,
            "Node Not Set Up Exception: Thrown by Node::Update_Position\n"
            "You need to set the origional position before updating the position\n"
            "It doesn't make sense to update the position before setting the original one\n");
  } // if(Original_Position_Set_Up == false) {

  /* Assumption 2:
  This functions also assumes that the specified index is either 0, 1, or 2
  since the Position array only has 3 components.*/
  if(component > 2) {
    char Error_Message_Buffer[500];
    sprintf(Error_Message_Buffer,
            "Array Index Out Of Bounds Exception: Thrown by Node::Update_Position\n"
            "The component array only has 3 components (corresponding to the three\n"
            "component directions). Thus, the valid indicies are 0, 1, and 2. You\n"
            "requested index %d\n",
            component);
    throw Array_Index_Out_Of_Bounds(Error_Message_Buffer);
  } // if(component > 2) {

  // If if all of the assumptions have been satisified then update the current position
  Position[component] = New_Position;
} // void Node::Update_Position(const unsigned component, const double New_Position) {



void Node::Set_Original_Position(const Array_3<double> Original_Position_In) {
  /* Function Description:
  This function is used to set the original position of the Node. The original
  position is used to compute K. */

  /* Assumption 1:
  This function assumes that the node has not been set up already. Once a Node's
  original position has been set, it can not be reset. */
  if(Original_Position_Set_Up == true) {
    char Error_Message_Buffer[500];
    scanf(Error_Message_Buffer,
          "Node Already Set Up Exception: Thrown by Node::Set_Original_Position\n"
          "Once a node's original position has been set, it can not be reset.\n");
    throw Node_Already_Set_Up(Error_Message_Buffer);
  } // if(Original_Position_Set_Up == true) {

  // Set the Original Position and Current_Position using the passed Array
  Original_Position = Original_Position_In;
  Position = Original_Position;

  // Original Position has been set
  Original_Position_Set_Up = true;
} // void Node::Set_Original_Position(const Array_3<double> Original_Position_In) {



void Node::Set_BC(const unsigned Component, const double BC_In) {
  /* Function Description:
  This function is used to set the BC for a particular component. */

  /* Assumption 1:
  This function assumes that the Component is 0, 1, or 2 since the Position
  array only has 3 componnents. If this is not the case, then we thrown an
  Array exception. */
  if(Component > 2) {
    char Error_Message_Buffer[500];
    sprintf(Error_Message_Buffer,
            "Array Index Out Of Bounds Exception: Thrown by Node::Set_BC\n"
            "The BC array has 3 components (corresponding to the 3 coordinate\n"
            "directions). Thus, valid indicies are 0, 1, or 2.\n"
            "You requested index %d.\n",
            Component);
    throw Array_Index_Out_Of_Bounds(Error_Message_Buffer);
  } // if(component > 2) {

  /* Assumption 2:
  This function assumes that the specificed BC has not already been set */
  if(Has_BC[Component] == true) {
    char Error_Message_Buffer[500];
    sprintf(Error_Message_Buffer,
            "Node Already Set Up Exception: Thrown by Node::Set_BC\n"
            "Once a Node's BC's have been set, they can not be unset. You tried to\n"
            "set the %d component of the boundary condition but that component has\n"
            "already been set with BC[%d] = %lf\n",
            Component, Component, BC[Component]);
    throw Node_Already_Set_Up(Error_Message_Buffer);
  } // if(Has_BC[Component] == true) {

  // Set the BC.
  BC[Component] = BC_In;
  Has_BC[Component] = true;
} // void Node::Set_BC(const unsigned Component, const double BC_In) {






////////////////////////////////////////////////////////////////////////////////
// Getter methods

bool Node::Get_Has_BC(const unsigned Index) const {
  /* Function Description:
  This function is used to determine if a particular component of a Node's
  position is fixed (has a Prescribed displacement BC) or free. */

  /* Assumption 1:
  This function assumes that the Index is either 0, 1, or 2. The Fixed_Pos
  array only has 3 elements, so anything with index > 2 is undefined */
  if(Index > 2) {
    char Error_Message_Buffer[500];
    sprintf(Error_Message_Buffer,
            "Array Index Out Of Bounds Exception: Thrown by Node::Get_Has_BC\n"
            "The BC array only has 3 components. Thus, valid indicies are 0, 1, and 2\n"
            "you requested index %d\n",
            Index);
    throw Array_Index_Out_Of_Bounds(Error_Message_Buffer);
  } // if(index > 2) {

  return Has_BC[Index];
} // bool Node::Get_Has_BC(const unsigned Index) const {



const Array_3<double> & Node::Get_Current_Position(void) const {
  /* Function Description:
  This function is used to obtain the current position of the Node. */

  /* Assumption 1:
  This function assumes that this node has already been set up. If it hasn't,
  then there is no "Original position" to report. */
  if(Original_Position_Set_Up == false) {
    char Error_Message_Buffer[500];
    sprintf(Error_Message_Buffer,
            "Node Not Set Up Exception: Thrown by Node::Get_Current_Position\n"
            "The Original Position needs to be set before we can return the current\n"
            "position. Set_Original_Position must be run BEFORE Get_Current_Position\n");
    throw Node_Not_Set_Up(Error_Message_Buffer);
  } // if(Original_Position_Set_Up == false) {

  // Copy position to Pos_Out variable
  return Position;
} // const Array_3<double> & Node::Get_Current_Position(void) const {



const Array_3<double> & Node::Get_Original_Position(void) const {
  /* Function Description:
  This function is used to obtain the Original position of the Node. */

  /* Assumption 1:
  This function assumes that this node has already been set up. If it hasn't,
  then there is no "Original position" to report. */
  if(Original_Position_Set_Up == false) {
    char Error_Message_Buffer[500];
    sprintf(Error_Message_Buffer,
            "Node Not Set Up Exception: Thrown by Node::Get_Current_Position\n"
            "The Original Position needs to be set before we can return the Original\n"
            "position. Set_Original_Position must be run BEFORE Get_Original_Position\n");
    throw Node_Not_Set_Up(Error_Message_Buffer);
  } // if(Original_Position_Set_Up == false) {

  // Copy position to Pos_Out variable
  return Original_Position;
} // const Array_3<double> & Node::Get_Original_Position(void) const {






////////////////////////////////////////////////////////////////////////////////
// Other methods

void Node::Print(void) const {
  /* Function Description:
  This function prints out information about the node. */

  /* Assumption 1:
  This function assumes that this node has already been set up. If it hasn't,
  then there's nothing to print. */
  if(Original_Position_Set_Up == false) {
    char Error_Message_Buffer[500];
    sprintf(Error_Message_Buffer,
            "Node Not Set Up Exception: Thrown by Node::Print\n"
            "We can't print the Node data if the node hasn't been set up.\n"
            "Set_Original_Position must run BEFORE Print\n");
    throw Node_Not_Set_Up(Error_Message_Buffer);
  } // if(Original_Position_Set_Up == false) {

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
} // void Node::Print(void) const {


#endif
