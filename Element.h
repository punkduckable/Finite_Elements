#if !defined(ELEMENT_HEADER)
#define ELEMENT_HEADER

#include "Node.h"
#include "Errors.h"
#include "Matrix.h"


class Element {
private:
  //////////////////////////////////////////////////////////////////////////////
  // Static members
  static bool Static_Members_Set;                                    // True if the static members have been set
  static Matrix<unsigned> * ID;                                      // Points to the ID Matrix
  static Matrix<double> * K;                                         // Points to the global stiffness matrix
  static double (*F)(unsigned, unsigned, unsigned, unsigned);        // Given Node/component, this Calculates an element of Ke
  const static unsigned FIXED_COMPONENT = -1;                        // Used to indicate that a particular component of a node's displacement is fixed


  //////////////////////////////////////////////////////////////////////////////
  // Object specific private members

  // Flags
  bool Element_Set_Up = false;
  bool Ke_Set_Up = false;

  // The Global node numbers for the nodes associated with this Element.
  unsigned int Node_List[8];

  /* Assembly arrays. Local_Eq_Num_To_Node is used to construct Ke while
  Local_Eq_Num_To_Global_Eq_Num is used to map Ke into K. */
  unsigned Local_Eq_Num_To_Node[24*2];                     // Stores the global node # and component associated with each local equation
  unsigned Local_Eq_Num_To_Global_Eq_Num[24];              // Stores the global equation # associated with each local eqiation

  // Local element stiffness matrix;
  Matrix<double> Ke = Matrix<double>(24, 24, Memory::COLUMN_MAJOR);

public:
  //////////////////////////////////////////////////////////////////////////////
  // Constructors, Destructor

  // Default do nothing constructor
  Element(void) { };

  // Destructor
  ~Element(void);


  //////////////////////////////////////////////////////////////////////////////
  // Class methods

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


  friend Element_Errors::Errors Set_Element_Static_Members(Matrix<unsigned> * ID_Ptr,    // Intent: Read
                                                           Matrix<double> * K_Ptr,       // Intent: Read
                                                           double (*Integrating_Function)(unsigned, unsigned, unsigned, unsigned));   // Intent: Read


  //////////////////////////////////////////////////////////////////////////////
  // Disable Implicit methods
  /* C++ implicitly defines the = operator and the copy construct for all
  classes. Both of these methods work by member-by-member copying the members of
  one object into another. The issue is that the Element class has dynamically
  allocated members. Copying member-by-member is a shallow copy. This could lead
  to disaster. Thus, I explicitly define these this method to basically disable
  the implicit ones. All that these methods do is yell at the user for using
  then. */

  // Disabled copy constructor
  Element(const Element & El);

  // disabled = operator.
  Element & operator=(const Element & El);
}; // class Element {

Element_Errors::Errors Set_Element_Static_Members(Matrix<unsigned> * ID_Ptr,   // Intent: Read
                                                  Matrix<double> * K_Ptr,      // Intent: Read
                                                  double (*Integrating_Function)(unsigned, unsigned, unsigned, unsigned));  // Intent: Read

#endif
