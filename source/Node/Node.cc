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
  method, Set_Position.

  All that this constructor really does is give the Node trivial BC's, position,
  and Force (each of which can be updated with one of the Node setters.) */

  Force[0]        = Force[1]        = Force[2]        = 0;
  Position[0]     = Position[1]     = Position[2]     = 0;
  Displacement[0] = Displacement[1] = Displacement[2] = 0;
  Has_BC[0]       = Has_BC[1]       = Has_BC[2]       = false;
} // Node::Node(void) {






////////////////////////////////////////////////////////////////////////////////
// Setter methods

void Node::Set_Position_Component(const unsigned component, const double Position_In) {
  /* Function Description:
  This function is used to update a particular component of the Node's position. */

  /* Assumption 1:
  This functions also that the specified component is either 0, 1, or 2
  since the Position array only has 3 components.*/
  if(component > 2) {
    char Error_Message_Buffer[500];
    sprintf(Error_Message_Buffer,
            "Array Index Out Of Bounds Exception: Thrown by Node::Set_Position_Component\n"
            "The component array only has 3 components (corresponding to the three\n"
            "component directions). Thus, the valid indicies are 0, 1, and 2. You\n"
            "requested index %d\n",
            component);
    throw Array_Index_Out_Of_Bounds(Error_Message_Buffer);
  } // if(component > 2) {

  // If if all of the assumptions have been satisified then update the position
  Position[component] = Position_In;
} // void Node::Set_Position_Component(const unsigned component, const double Position_In) {



void Node::Set_BC_Component(const unsigned component, const double BC_In) {
  /* Function Description:
  This function is used to set the displacement BC for a particular component. */

  /* Assumption 1:
  This function assumes that the component is 0, 1, or 2 since the Position
  array only has 3 componnents. If this is not the case, then we thrown an
  Array exception. */
  if(component > 2) {
    char Error_Message_Buffer[500];
    sprintf(Error_Message_Buffer,
            "Array Index Out Of Bounds Exception: Thrown by Node::Set_BC_Component\n"
            "The BC array has 3 components (corresponding to the 3 coordinate\n"
            "directions). Thus, valid indicies are 0, 1, or 2.\n"
            "You requested index %d.\n",
            component);
    throw Array_Index_Out_Of_Bounds(Error_Message_Buffer);
  } // if(component > 2) {

  // Set the BC.
  Displacement[component] = BC_In;
  Has_BC[component] = true;
} // void Node::Set_BC_Component(const unsigned component, const double BC_In) {



void Node::Set_Force_Component(const unsigned component, const double Force_In) {
  /* Function description:
  This function is used to set a specific component of the net force vector on
  the node. */

  /* This function assumes that the component is 0, 1, or 2 since the Position
  array only has 3 componnents. If this is not the case, then we thrown an
  Array exception. */
  if(component > 2) {
    char Error_Message_Buffer[500];
    sprintf(Error_Message_Buffer,
            "Array Index Out Of Bounds Exception: Thrown by Node::Set_Force_Component\n"
            "The BC array has 3 components (corresponding to the 3 coordinate\n"
            "directions). Thus, valid indicies are 0, 1, or 2.\n"
            "You requested index %d.\n",
            component);
    throw Array_Index_Out_Of_Bounds(Error_Message_Buffer);
  } // if(component > 2) {

  Force[component] = Force_In;
} // void Node::Set_Force_Component(const unsigned component, const double Force_In) {



void Node::Set_Displacement_Component(const unsigned component, const double Displacement_In) {
  /* Function Description:
  This function is used to directly set the displacement of the node in a
  particular direction. In general, if a component has a prescribed displacement
  BC then that component can NOT have its displacement set. */

  /* Assumption 1:
  We assume that component is 0, 1, or 2. The displacement array only has 3
  components... so if this is not the case then idek what to tell you. */
  if(component > 2) {
    char Error_Message_Buffer[500];
    sprintf(Error_Message_Buffer,
            "Array Index Out Of Bounds Exception: Thrown by Node::Set_Displacement_Component\n"
            "The BC array has 3 components (corresponding to the 3 coordinate\n"
            "directions). Thus, valid indicies are 0, 1, or 2.\n"
            "You requested index %d.\n",
            component);
    throw Array_Index_Out_Of_Bounds(Error_Message_Buffer);
  } // if(component > 2) {

  /* Assumption 2:
  If the node has a prescribed displacement BC in a particular component then we
  can not change the displacement in that direction (once the BC is set, it's
  essentially fixed). */
  if(Has_BC[component] == true) {
    char Error_Message_Buffer[500];
    sprintf(Error_Message_Buffer,
            "Fixed Component Exception: Thrown by Node::Set_Displacement_Component\n"
            "If a node has a BC set in a particular direction then you can not directly\n"
            "set the displacement in that component. You tried to update the %d\n"
            "component of Position but that componnet is a BC.\n",
            component);
    throw Fixed_Component(Error_Message_Buffer);
  } // if(Has_BC[component] == true) {

  Displacement[component] = Displacement_In;
} // void Node::Set_Displacement_Component(const unsigned component, const double Displacement_In) {





////////////////////////////////////////////////////////////////////////////////
// Getter methods

bool Node::Get_Has_BC(const unsigned component) const {
  /* Function Description:
  This function is used to determine if a particular component of a Node's
  position is fixed (has a Prescribed displacement BC) or free. */

  /* Assumption 1:
  This function assumes that the component is either 0, 1, or 2. The Has_BC
  array only has 3 elements, so anything with component > 2 is undefined */
  if(component > 2) {
    char Error_Message_Buffer[500];
    sprintf(Error_Message_Buffer,
            "Array Index Out Of Bounds Exception: Thrown by Node::Get_Has_BC\n"
            "The BC array only has 3 components. Thus, valid indicies are 0, 1, and 2\n"
            "you requested index %d\n",
            component);
    throw Array_Index_Out_Of_Bounds(Error_Message_Buffer);
  } // if(component > 2) {

  return Has_BC[component];
} // bool Node::Get_Has_BC(const unsigned component) const {



double Node::Get_Position_Component(const unsigned component) const {
  /* Function Description:
  This function is used to obtain a component of the Node's position */

  /* Assumption 1:
  This function assumes that the component is either 0, 1, or 2. The Position
  array only has 3 elements, so anything with component > 2 is undefined */
  if(component > 2) {
    char Error_Message_Buffer[500];
    sprintf(Error_Message_Buffer,
            "Array Index Out Of Bounds Exception: Thrown by Node::Get_Position_Component\n"
            "The BC array only has 3 components. Thus, valid indicies are 0, 1, and 2\n"
            "you requested index %d\n",
            component);
    throw Array_Index_Out_Of_Bounds(Error_Message_Buffer);
  } // if(component > 2) {

  // Return the requested component of position
  return Position[component];
} // double Node::Get_Position_Component(const unsigned component) const {



double Node::Get_Force_Component(const unsigned component) const {
  /* Function description:
  This function returns the specified component of the node's force vector. */

  /* Assumption 1:
  component is either 0 1 or 2. The force vector only has 3 components (x, y, z)*/
  if(component > 2) {
    char Error_Message_Buffer[500];
    sprintf(Error_Message_Buffer,
            "Array Index Out Of Bounds Exception: Thrown by Node::Get_Force_Component\n"
            "The BC array only has 3 components. Thus, valid indicies are 0, 1, and 2\n"
            "you requested index %d\n",
            component);
    throw Array_Index_Out_Of_Bounds(Error_Message_Buffer);
  } // if(component > 2) {

  return Force[component];
} // double Node::Get_Force_Component(const unsigned component) const {



double Node::Get_Displacement_Component(const unsigned component) const {
  /* Function description:
  This function returns the specified component of the node's displacement vector. */

  /* Assumption 1:
  component is either 0 1 or 2. The displacement vector only has 3 components (x, y, z)*/
  if(component > 2) {
    char Error_Message_Buffer[500];
    sprintf(Error_Message_Buffer,
            "Array Index Out Of Bounds Exception: Thrown by Node::Get_Displacement_Component\n"
            "The BC array only has 3 components. Thus, valid indicies are 0, 1, and 2\n"
            "you requested index %d\n",
            component);
    throw Array_Index_Out_Of_Bounds(Error_Message_Buffer);
  } // if(component > 2) {

  return Displacement[component];
} // double Node::Get_Displacement_Component(const unsigned component) const {






////////////////////////////////////////////////////////////////////////////////
// Other methods

void Node::Print(void) const {
  /* Function Description:
  This function prints out information about the node. */

  printf(         "Position            :         [%6.3lf, %6.3lf, %6.3lf]\n", Position[0], Position[1], Position[2]);
  printf(         "Displacement        :         [%6.3lf, %6.3lf, %6.3lf]\n", Displacement[0], Displacement[1], Displacement[2]);
  printf(         "Force               :         [%6.3lf, %6.3lf, %6.3lf]\n", Force[0], Force[1], Force[2]);
  printf(         "Fixed (has BC)      :         [");
    for(int i = 0; i < 3; i++) {
      // Print true if this component of the position is fixed, false otherwise
      if(Has_BC[i] == false) { printf("false "); }
      else { printf("true "); }
    } // for(int i = 0; i < 3; i++) {
    printf("]\n");
} // void Node::Print(void) const {


#endif
