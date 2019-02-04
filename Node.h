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

  /* Flags. These are used so that the Original_Position, Fixed_Pos variables
  Can only be set once */
  bool Original_Position_Has_Been_Set = false;
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
  Node_Errors::Errors Update_Position(const double New_Position_Component, const unsigned int component);

  // Set internal variables
  Node_Errors::Errors Set_Original_Position(const Array_3<double> Original_Position_In, const Array_3<bool> Fixed_Pos_In);


  //////////////////////////////////////////////////////////////////////////////
  // Getter methods

  Array_3<double> Get_Original_Position(void) const;
  Array_3<double> Get_Current_Position(void) const;

  //////////////////////////////////////////////////////////////////////////////
  // Other public methods

  void Print(void);
}; // class Node {

#endif
