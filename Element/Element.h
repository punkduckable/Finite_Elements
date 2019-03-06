#if !defined(ELEMENT_HEADER)
#define ELEMENT_HEADER

#include "../Node.h"
#include "../Errors.h"
#include "../Matrix.h"
#define ELEMENT_MONITOR


class Element {
private:
  //////////////////////////////////////////////////////////////////////////////
  // Static members

  static bool Static_Members_Set;                // True if the static members have been set
  static Matrix<unsigned> * ID;                  // Points to the ID Matrix
  static Matrix<double> * K;                     // Points to the global stiffness matrix
  static Node * Nodes;                           // Points to the array of nodes.

  static Matrix<double> Na;                      // Value of each shape function at each integrating point
  static Matrix<double> Na_Xi;                   // Zeta-partial of each shape function at each integrating point
  static Matrix<double> Na_Eta;                  // Eta-partial of each shape function at each integrating point
  static Matrix<double> Na_Zeta;                 // Zeta-partial of each shape function at each integrating point

  static bool Material_Set;                      // True if the material parameter have been set (D is set up)
  static Matrix<double> D;                       // Voigt notation elasticity tensor.

  const static unsigned FIXED_COMPONENT = -1;    // Used to indicate that a particular component of a node's displacement is fixed



  //////////////////////////////////////////////////////////////////////////////
  // Object specific private members

  // Flags
  bool Element_Set_Up = false;
  bool Ke_Set_Up = false;

  // Node information
  unsigned int Node_List[8];                     // Global node numbers for the nodes associated with this Element.
  double Xa[8];                                  // X spatial coordinate of each Node
  double Ya[8];                                  // Y spatial coordinate of each Node
  double Za[8];                                  // Z spatial coordinate of each Node


  /* Assembly array.
  Local_Eq_Num_To_Global_Eq_Num is used to map Ke into K. */
  unsigned Local_Eq_Num_To_Global_Eq_Num[24];    // Stores the global equation # associated with each local eqiation

  // Local element stiffness matrix;
  Matrix<double> Ke = Matrix<double>(24, 24, Memory::COLUMN_MAJOR);


  /* Calculate Coefficient matrix, Determinant.
  This method is kept private since it us really only used by the Populate_Ke
  method. This method calculates Coeff + J, allowing us to compute Na_x, Na_y,
  and Na_z at each integration point in the Element. */
  Element_Errors::Errors Calculate_Coefficient_Matrix(const unsigned Integration_Point_Index, // Intent: Read
                                                      Matrix<double> & Coeff,  // Intent: Write
                                                      double & J);             // Intent: Write
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

  This function sets Num_Local_Eq, Local_Eq_Num_To_Global_Eq_Num, and the
  node position arrays (Xa, Ya, Za). */
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
  Element_Errors::Errors Fill_Ke_With_1s(void);                                // A function to test the assembly procedure

  /* Move Ke into K */
  Element_Errors::Errors Move_Ke_To_K(void) const;


  friend Element_Errors::Errors Set_Element_Static_Members(Matrix<unsigned> * ID_Ptr,    // Intent: Read
                                                           Matrix<double> * K_Ptr,       // Intend: Read
                                                           Node * Nodes_Ptr);            // Intent: Read

  friend Element_Errors::Errors Set_Element_Material(const double E,           // Intent : Read
                                                     const double v);          // Intent : Read

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
                                                  Node * Nodes_Ptr);           // Intent: Read

Element_Errors::Errors Set_Element_Material(const double E,                    // Intent : Read
                                            const double v);                   // Intent : Read

#endif
