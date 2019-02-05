#if !defined(ELEMENT_HEADER)
#define ELEMENT_HEADER

#include "Node.h"
#include "Errors.h"

using namespace Element_Errors;

class Element {
private:
  //////////////////////////////////////////////////////////////////////////////
  // Static members
  static unsigned Num_Elements;                  // Number of elements created
  static Node * Node_Array;                      // Points to the Array of all the Nodes for the object being simulated
  static unsigned * ID;                          // Points to the ID array


  //////////////////////////////////////////////////////////////////////////////
  // Object specific private members

  // The Global node numbers for the nodes associated with this Element.
  bool Nodes_Set = false;
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
  Element(const Element & El);

  // Destructor
  ~Element(void);

  //////////////////////////////////////////////////////////////////////////////
  // Class methods

  /* Explicit = operator method. C++ implicitly defines this method to simply
  perform an member-by-member copy. The issue is that the Element class has
  dynamically allocated members. Copying member-by-member would involve a
  shallow copy. This could lead to disaster. Thus, I explicitly defined
  this method to basically disable element equality. */
  Element & operator=(const Element & El);

  /* Set nodes.

  This function sets Num_Local_Eq and allocates Local_Eq_Num_To_Node,
  Local_Eq_Num_To_Global_Eq_Num, and Ke.

  Local_Eq_Num_To_Node and Local_Eq_Num_To_Global_Eq_Num are set after being
  allocated. */
  Errors Set_Nodes(const unsigned Node0_ID,
                 const unsigned Node1_ID,
                 const unsigned Node2_ID,
                 const unsigned Node3_ID,
                 const unsigned Node4_ID,
                 const unsigned Node5_ID,
                 const unsigned Node6_ID,
                 const unsigned Node7_ID);


  Errors Node_ID(const unsigned i, unsigned & ID_Out) const;     // Returns the ID of the ith Node in the Node list

  friend Errors Set_Element_Static_Members(Node * Node_Array_Ptr, unsigned * ID_Ptr);
}; // class Element {

Errors Set_Element_Static_Members(Node * Node_Array_Ptr, unsigned * ID_Ptr);

#endif
