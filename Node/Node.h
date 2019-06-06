#if !defined(NODE_HEADER)
#define NODE_HEADER

#include "../Errors.h"
#include "../Array.h"
#include <math.h>

class Node {
// Elements need to be able to access the private Node members.
friend class Element;

private:
  // Core members of the Node class
  Array_3<double> Original_Position;             // Initial spatial position of Node     Units : M
  bool Original_Position_Set_Up = false;         // Prevents Original position from being overwritten.

  Array_3<double> Position;                      // Current Spatial position of the Node Units : M
  Array_3<bool> Has_BC;                          // ith component is true if the Node has a BC in that component
  Array_3<double> BC;                            // Prescribed Boundary Conditions       Units : M
public:
  //////////////////////////////////////////////////////////////////////////////
  // Constructors, Destructor

  // Default constructor
  Node(void);

  // Do nothing destructor.
  ~Node(void) {};


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
  // Set Original position of the Node (used for construction of K)
  void Set_Original_Position(const Array_3<double> Original_Position_In); // Intent: Read


  //////////////////////////////////////////////////////////////////////////////
  // Update Node methods

  // Updates the current (spatial) position of the node
  void Update_Position(const unsigned int component,                           // Intent: Read
                       const double New_Position);                             // Intent: Read

  // Sets a Particular component of the BC's
  void Set_BC(const unsigned component,                                        // Intent: Read
              const double BC_In);                                             // Intent: Read

  // Updates positions using the prescribed position BCs
  void Update_Position_Using_BCs(void);


  //////////////////////////////////////////////////////////////////////////////
  // Getter methods

  bool Get_Has_BC(const unsigned index) const;                                 // Intent: Read

  const Array_3<double> & Get_Current_Position(void) const;

  const Array_3<double> & Get_Original_Position(void) const;


  //////////////////////////////////////////////////////////////////////////////
  // Other public methods

  void Print(void) const;
}; // class Node {

#endif
