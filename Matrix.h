#if !defined(MATRIX_HEADER)
#define MATRIX_HEADER

enum class Memory{ROW_MAJOR, COLUMN_MAJOR};

template <typename Type>
class Matrix {
private:
  // Where the array is actually stored.
  Type * Ar;

  // Dimension of the matrix
  unsigned Num_Rows;
  unsigned Num_Cols;

  // Specify row or column major storage scheme
  Memory Memory_Layout;
public:
  // Default constructor
  Matrix(void);

  // Set up Constructor
  Matrix(const unsigned Rows_In,                                               // Intent: Read
         const unsigned Cols_In,                                               // Intent: Read
         const Memory Layout_In);                                              // Intent: Read

  // Destructor
  ~Matrix(void);


  //////////////////////////////////////////////////////////////////////////////
  // Class methods

  // Set up matrix method
  void Set_Up(const unsigned Rows_In,                                          // Intent: Read
              const unsigned Cols_In,                                          // Intent: Read
              const Memory Layout_In);                                         // Intent: Read

  // Write to an element of the matrix
  Type & operator()(const unsigned i,                                          // Intent: Read
                    const unsigned j);                                         // Intent: Read

  // Read an eleent of the matrix
  Type operator()(const unsigned i,                                            // Intent: Read
                  const unsigned j) const;                                     // Intent: Read


  //////////////////////////////////////////////////////////////////////////////
  // Getter methods

  unsigned Get_Num_Rows(void) const { return Num_Rows; }
  unsigned Get_Num_Cols(void) const { return Num_Cols; }
  Memory Get_Memory_Layout(void) const { return Memory_Layout; }


  //////////////////////////////////////////////////////////////////////////////
  // Disabled implicit methods
  /* C++ implicitly defines the = operator and copy constructor. The issue is
  that both of these implicit methods use a shallow copy. The matrix class has
  dynamically allocated memory. Therefore, a shallow copy to a temporary
  object could end up de-allocating memory that should not be de-allocated,
  leading to disaster. I do not want either of these to be useable for the
  matrix class. Therefore, I wrote explicit version of these functions that
  prints an angry message when either of them is used */

  // Disabled Copy Constructor
  Matrix(const Matrix<Type> & M_In);

  // Disabled = operator
  Matrix<Type> & operator=(const Matrix<Type> & M_In);
}; // class Matrix {

#include "Matrix.cc"

#endif
