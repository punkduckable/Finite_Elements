#if !defined(NODE_HEADER)
#define NODE_HEADER

#include "Forward_Declarations.h"
#include "Array.h"

class Node {
// Elements need to be able to access the private Node members.
friend class Element;

private:
  // Core members of the Node class
  Array_3<double> Original_Position;        // Material position of the node    Units : M
  Array_3<double> Current_Position;         // Spatial position of the node     Units : M
  Array_3<bool> Has_BC;                     // Has_BC[i] is true if the ith component of this node has a prescribed displacement BC
  Array_3<double> BC;                       // The Prescribed displacement BC's


  /* Flags. These are used so that the Original_Position, Has_BC, and BC variables
  Can only be set once */
  bool Original_Position_Has_Been_Set = false;
  bool BCs_Have_Been_Set = false;

  /* This method is called by the Set_Original_Position or Set_BCs functions if
  both the BCs and Original_Position have been set */
  void Set_Current_Position(void);
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
  void Update_Position(const double New_Position_Component, const unsigned int component);

  // Set internal variables
  void Set_Original_Position(const Array_3<double> Original_Position_In);
  void Set_BCs(const Array_3<bool> Has_BC_In, const Array_3<double> BC_In);


  //////////////////////////////////////////////////////////////////////////////
  // Getter methods

  Array_3<double> Get_Original_Position(void) const;
  Array_3<double> Get_Current_Position(void) const;

  //////////////////////////////////////////////////////////////////////////////
  // Other public methods

  void Print(void);
}; // class Node {

#endif
