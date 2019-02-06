#if !defined(ELEMENT_HEADER)
#define ELEMENT_HEADER

#include "Node.h"
#include "Errors.h"


class Element {
private:
  //////////////////////////////////////////////////////////////////////////////
  // Static members
  static bool Static_Members_Set;                                    // True if the static members have been set
  static unsigned Num_Elements;                                      // Number of elements created
  static Node * Node_Array;                                          // Points to the Array of all the Nodes for the object being simulated
  static unsigned * ID;                                              // Points to the ID array
  static double (*F)(unsigned, unsigned, unsigned, unsigned);        // Given Node/component, this Calculates an element of Ke
  static double * K;                                                 // Points to the Stiffness matrix
  static unsigned Num_Global_Eq;                                     // Number of global equations.


  //////////////////////////////////////////////////////////////////////////////
  // Object specific private members

  // Flags
  bool Element_Set_Up = false;
  bool Ke_Set_Up = false;

  // The Global node numbers for the nodes associated with this Element.
  unsigned int Node_List[8];

  // Number of local equations
  unsigned int Num_Local_Eq;

  /* Assembly arrays. Local_Eq_Num_To_Node is used to construct Ke while
  Local_Eq_Num_To_Global_Eq_Num is used to map Ke into K. */
  unsigned int * Local_Eq_Num_To_Node;                     // Stores the global node # and component associated with each local equation
  unsigned int * Local_Eq_Num_To_Global_Eq_Num;            // Stores the global equation # associated with each local eqiation

  // Local element stiffness matrix;
  double * Ke;

public:
  //////////////////////////////////////////////////////////////////////////////
  // Constructors, Destructor

  // Default do nothing constructor
  Element(void) { Num_Elements++; };

  // Deep copy constructor
  Element(const Element & El);                                                 // Intent: Read

  // Destructor
  ~Element(void);

  //////////////////////////////////////////////////////////////////////////////
  // Class methods

  /* Explicit = operator method. C++ implicitly defines this method to simply
  perform an member-by-member copy. The issue is that the Element class has
  dynamically allocated members. Copying member-by-member would involve a
  shallow copy. This could lead to disaster. Thus, I explicitly defined
  this method to basically disable element equality. */
  Element & operator=(const Element & El);                                     // Intent: Read

  /* Set nodes.

  This function sets Num_Local_Eq and allocates Local_Eq_Num_To_Node,
  Local_Eq_Num_To_Global_Eq_Num, and Ke.

  Local_Eq_Num_To_Node and Local_Eq_Num_To_Global_Eq_Num are set after being
  allocated. */
  Element_Errors::Errors Set_Nodes(const unsigned Node0_ID,                    // Intent: Read
                                   const unsigned Node1_ID,                    // Intent: Read
                                   const unsigned Node2_ID,                    // Intent: Read
                                   const unsigned Node3_ID,                    // Intent: Read
                                   const unsigned Node4_ID,                    // Intent: Read
                                   const unsigned Node5_ID,                    // Intent: Read
                                   const unsigned Node6_ID,                    // Intent: Read
                                   const unsigned Node7_ID);                   // Intent: Read

  /* Populate Ke.
  This function populates the Ke matrix for this Element */
  Element_Errors::Errors Populate_Ke(void);

  /* Move Ke into K */
  Element_Errors::Errors Move_Ke_To_K(void) const;

  /* Move Ke into K */

  // Sets ID_Out to the ID of the ith Node in the Node list
  Element_Errors::Errors Node_ID(const unsigned i,                             // Intent: Read
                                 unsigned & ID_Out) const;                     // Intent: Write


  friend Element_Errors::Errors Set_Element_Static_Members(Node * Node_Array_Ptr,   // Intent: Read
                                                           unsigned * ID_Ptr,       // Intent: Read
                                                           double (*Integrating_Function)(unsigned, unsigned, unsigned, unsigned),    // Intent: Read
                                                           double * K_Ptr,          // Intent: Read
                                                           const unsigned Num_Global_Eq);     // Intent: Read
}; // class Element {

Element_Errors::Errors Set_Element_Static_Members(Node * Node_Array_Ptr,       // Intent: Read
                                                  unsigned * ID_Ptr,           // Intent: Read
                                                  double (*Integrating_Function)(unsigned, unsigned, unsigned, unsigned),        // Intent: Read
                                                  double * K_Ptr,              // Intent: Read
                                                  const unsigned Num_Global_Eq);    // Intent: Read

#endif
