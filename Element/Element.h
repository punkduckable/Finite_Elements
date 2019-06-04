#if !defined(ELEMENT_HEADER)
#define ELEMENT_HEADER

#include "../Node/Node.h"
#include "../Errors.h"
#include "../Matrix/Matrix.h"

class Element {
private:
  //////////////////////////////////////////////////////////////////////////////
  // Static members

  static bool Static_Members_Set;                // True if the static members have been set
  static Matrix<unsigned> * ID;                  // Points to the ID Matrix
  static Matrix<double> * K;                     // Points to the global stiffness matrix
  static double * F;                             // Points to the global force vector.
  static Node * Global_Node_Array;               // Points to the array of nodes.

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
  bool Fe_Set_Up = false;

  // Node information
  struct Node_Data { unsigned int ID;           // Global Node number (array index)
                double Xa;                      // X spatial coordinate (original) of the node
                double Ya;                      // Y spatial coordinate (original) of the node
                double Za; };                   // Z spatial coordinate (original) of the node
  Node_Data Element_Nodes[8];


  /* Assembly arrays
  Local_Eq_Num_To_Global_Eq_Num is used to map Ke into K. */
  unsigned Local_Eq_Num_To_Global_Eq_Num[24];    // Stores the global equation # associated with each local equation

  /* Prescribed positions array. If the ith local equation corresponds to a
  fixed position then the ith component of this array stores the associated
  prescribed BC. Otherwise (if the equation corresponds to a free component),
  the ith component of this array is zero */
  double Prescribed_Positions[24];


  // Local element stiffness matrix, Force Vector
  Matrix<double> Ke{24, 24, Memory::COLUMN_MAJOR};
  double Fe[24];


  /* Calculate Coefficient matrix, Determinant.
  This method is kept private because the only time that it should be called is
  when the Populate_Ke method is running.

  This method calculates Coeff + J, allowing us to compute Na_x, Na_y, and Na_z
  at each integration point in the Element. */
  void Calculate_Coefficient_Matrix(const unsigned Integration_Point_Index,    // Intent: Read
                                    Matrix<double> & Coeff,                    // Intent: Write
                                    double & J);                               // Intent: Write


  /* Calculate Ba and move it into B.
  This method is kept private because the only time that it should be called is
  when the Populate_Ke method is running.

  This method computes the spatial partial derivatives (Na_x, Na_y, Na_z),
  uses them to construct Ba, and them moves Ba into B. */
  void Add_Ba_To_B(const unsigned Node,                                        // Intent: Read
                   const unsigned Integration_Point,                           // Intent: Read
                   const Matrix<double> & Coeff,                               // Intent: Read
                   const double J,                                             // Intent: Read
                   Matrix<double> & B);                                        // Intent: Write
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
  Element_Errors Set_Nodes(const unsigned Node0_ID,                            // Intent: Read
                           const unsigned Node1_ID,                            // Intent: Read
                           const unsigned Node2_ID,                            // Intent: Read
                           const unsigned Node3_ID,                            // Intent: Read
                           const unsigned Node4_ID,                            // Intent: Read
                           const unsigned Node5_ID,                            // Intent: Read
                           const unsigned Node6_ID,                            // Intent: Read
                           const unsigned Node7_ID);                           // Intent: Read

  /* Populate Ke.
  This function populates the Ke matrix for this Element */
  Element_Errors Populate_Ke(void);
  Element_Errors Fill_Ke_With_1s(void);                                        // A function to test the assembly procedure

  /* Populate Fe */
  Element_Errors Populate_Fe(void);

  /* Move Ke into K */
  Element_Errors Move_Ke_To_K(void) const;

  /* Move Fe into F */
  Element_Errors Move_Fe_To_F(void) const;



  friend Element_Errors Set_Element_Static_Members(Matrix<unsigned> * ID_Ptr,  // Intent: Read
                                                   Matrix<double> * K_Ptr,     // Intend: Read
                                                   double * F_Ptr,             // Intent: Read
                                                   Node * Node_Array_Ptr);     // Intent: Read

  friend Element_Errors Set_Element_Material(const double E,                   // Intent : Read
                                             const double v);                  // Intent : Read

  //////////////////////////////////////////////////////////////////////////////
  // Disable Implicit methods
  /* C++ implicitly defines the = operator and the copy construct for all
  classes. Both of these methods work by member-by-member copying the members of
  one object into another. The issue is that the Element class has dynamically
  allocated members. Copying member-by-member is a shallow copy. This could lead
  to disaster.

  Further, I really doesn't make any sense to set two elements equal to one
  another, and I have no intention of using the copy constructor for Elements

  Thus, I explicitly disable both of these this method so that the compiler
  will catch any accidential usage of these methods (thereby flagging potential
  issues at compile time) */

  Element(const Element & El) = delete;

  Element & operator=(const Element & El) = delete;
}; // class Element {

Element_Errors Set_Element_Static_Members(Matrix<unsigned> * ID_Ptr,           // Intent: Read
                                          Matrix<double> * K_Ptr,              // Intent: Read
                                          double * F_Ptr,                      // Intent: Read
                                          Node * Node_Array_Ptr);              // Intent: Read

Element_Errors Set_Element_Material(const double E,                            // Intent : Read
                                    const double v);                           // Intent : Read

#endif
