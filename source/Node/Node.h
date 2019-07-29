#if !defined(NODE_HEADER)
#define NODE_HEADER

#include "../Errors.h"
#include "../Array.h"
#include <math.h>

class Node {
private:
  // Core members of the Node class
  Array_3<double> Position;                      // Initial Spatial position of Node     Units : M
  Array_3<double> Displacement;                  // Displacement of the node             Units : M
  Array_3<double> Force;                         // Force acting on the node.            Units : N
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
  // Methods to update node's properties.

  // Set a particular component of the spatial position of the node
  void Set_Position_Component(const unsigned component,                        // Intent: Read
                              const double Position_In);                       // Intent: Read

  // Sets a particular component of the BC's
  void Set_BC_Component(const unsigned component,                              // Intent: Read
                        const double BC_In);                                   // Intent: Read

  // Set a particular component of the Node's Force vector
  void Set_Force_Component(const unsigned component,                           // Intent: Read
                           const double Force_In);                             // Intent: Read

  // Set a particular component of the Node's Displacement vector.
  void Set_Displacement_Component(const unsigned component,                    // Intent: Read
                                  const double Displacement_In);               // Intent: Read

  //////////////////////////////////////////////////////////////////////////////
  // Getter methods

  bool Get_Has_BC(const unsigned component) const;                             // Intent: Read
  double Get_Position_Component(const unsigned component) const;               // Intent: Read
  double Get_Displacement_Component(const unsigned component) const;           // Intent: Read
  double Get_Force_Component(const unsigned component) const;                  // Intent: Read


  //////////////////////////////////////////////////////////////////////////////
  // Other public methods

  void Print(void) const;
}; // class Node {

#endif
