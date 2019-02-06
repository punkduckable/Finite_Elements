#if !defined(NODE_HEADER)
#define NODE_HEADER

#include "Errors.h"
#include "Array.h"

class Node {
// Elements need to be able to access the private Node members.
friend class Element;

private:
  // Core members of the Node class
  Array_3<double> Original_Position;        // Material position of the node    Units : M
  Array_3<double> Current_Position;         // Spatial position of the node     Units : M
  Array_3<bool> Fixed_Pos;                  // if Fixed_Pos[i] is true then the ith component of the original position is fixed/cannot be updated (prescribed position BC).

  /* this is a flag that prevents the Fixed_Pos and Origional position variables
  from being overwritten once set. Basically, this makes it so that the node
  can only be set up once. */
  bool Node_Set_Up = false;
public:
  //////////////////////////////////////////////////////////////////////////////
  // Constructors, Destructor

  // Do nothing Default constructor
  Node(void) {};

  // Do nothing destructor.
  ~Node(void) {};

  //////////////////////////////////////////////////////////////////////////////
  // Setter methods

  // Updates the current (spatial) position of the node
  Node_Errors::Errors Update_Position(const double New_Position_Component,     // Intent: Read
                                      const unsigned int component);           // Intent: Read

  // Set internal variables
  Node_Errors::Errors Set_Original_Position(const Array_3<double> Original_Position_In,                                // Intent: Read
                                            const Array_3<bool> Fixed_Pos_In = Array_3<bool>(false, false, false));    // Intent: Read


  //////////////////////////////////////////////////////////////////////////////
  // Getter methods

  Node_Errors::Errors Get_Original_Position( Array_3<double> & Original_Position_Out ) const;      // Intent: Write
  Node_Errors::Errors Get_Current_Position( Array_3<double> & Current_Position_Out ) const;        // Intent: Write

  //////////////////////////////////////////////////////////////////////////////
  // Other public methods

  Node_Errors::Errors Print(void) const;
}; // class Node {

#endif
