#if !defined(NODE_HEADER)
#define NODE_HEADER

#include "../Errors.h"
#include "../Array.h"
#include <math.h>

class Node {
private:
  // Core members of the Node class
  Array_3<double> Position;                      // Spatial position of Node     Units : M
  bool Position_Set_Up = false;                  // Makes sure that Position isn't set twice

  Array_3<bool> Has_BC;                          // ith component is true if the Node has a BC in that component
public:
  //////////////////////////////////////////////////////////////////////////////
  // Constructors, Destructor

  Node(void);                /* Default constructor */
  ~Node(void) {};            /* Do nothing destructor */


  //////////////////////////////////////////////////////////////////////////////
  // Disable Implicit methods
  /* C++ implicitly defines the = operator and the copy construct for all
  classes. Both of these methods work by member-by-member copying the members of
  one object into another. However, I have no intention of allowing Nodes to be
  created using the copy constructor or set equal to one another using the =
  operator. Thus, I explicitly delete these methods */

  Node(const Node & Node_In) = delete;
  Node & operator=(const Node & Node_In) = delete;


  //////////////////////////////////////////////////////////////////////////////
  // Set the position of the Node (used for construction of K)

  void Set_Position(const Array_3<double> Position_In); // Intent: Read


  //////////////////////////////////////////////////////////////////////////////
  // Update Node methods

  // Updates a component of the spatial position of the node
  void Update_Position_Component(const unsigned int component,                 // Intent: Read
                                 const double New_Position);                   // Intent: Read

  // Sets a Particular component of the BC's
  void Set_BC(const unsigned component,                                        // Intent: Read
              const double BC_In);                                             // Intent: Read


  //////////////////////////////////////////////////////////////////////////////
  // Getter methods

  bool Get_Has_BC(const unsigned Index) const;                                 // Intent: Read
  double Get_Position_Component(const unsigned Index) const;                   // Intent: Read


  //////////////////////////////////////////////////////////////////////////////
  // Other public methods

  void Print(void) const;
}; // class Node {

#endif
