#if !defined(NODE_SOURCE)
#define NODE_SOURCE

#include "Node.h"
#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////
// Private methods

void Node::Set_Current_Position(void) {
  for(int i = 0; i < 3; i++)
    if(Has_BC[i] == true)
      Current_Position[i] = BC[i];
    else
      Current_Position[i] = Original_Position[i];
} // void Node::Set_Current_Position(void) {


////////////////////////////////////////////////////////////////////////////////
// Setter methods

void Node::Update_Position(const double New_Position_Component, const unsigned int component) {
  // First, check that the component is 0, 1, or 2
  if(component > 2) {
    printf("Error! The component index must be 0, 1, or 2\n");
    return;
  } // if(component > 2) {

  // Next, check if this component has a BC
  if(Has_BC[component] == true) {
    printf("Error! This node has a precribed boundary condition in the e%d direction\n", component+1);
    return;
  } // if(Has_BC[component] == true) {

  // If both checks pass then update the current position
  Current_Position[component] = New_Position_Component;
} // void Node::Update_Position(const double New_Position_Component, const unsigned int component) {


void Node::Set_Original_Position(const Array_3<double> Original_Position_In) {
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
  } // if(Original_Position_Has_Been_Set == false) {
  else
    printf("Error! This node already had its Original position set\n");
} // void Node::Set_Original_Position(const Array_3<double> Original_Position_In) {



void Node::Set_BCs(const Array_3<bool> Has_BC_In, const Array_3<double> BC_In) {
  /* Check if the Node's Boundary Conditions have already been set. They should
  only be set once when the Node is first being created. */
  if(BCs_Have_Been_Set == false) {
    // Set the Has_BC boolean array and the BC array using the inputs
    Has_BC = Has_BC_In;
    BC = BC_In;

    // The BC's have been set.
    BCs_Have_Been_Set = true;

    // Now, check if we're ready to set the Current_Position
    if(Original_Position_Has_Been_Set == true && BCs_Have_Been_Set == true)
      Set_Current_Position();
  } // if(BCs_Have_Been_Set == false) {
  else
    printf("Error! This node's BCs have already been set!\n");
} // void Node::Set_BCs(const Array_3<bool> Has_BC_In, const Array_3<double> BC_In) {

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
      if(Has_BC[i] == true)
        printf("%6.3lf", BC[i]);
      else
        printf("None  ");

      // Print comment
      if(i != 2)
        printf(", ");
    }
    printf("]\n");
} // void Node::Print(void) {


#endif
