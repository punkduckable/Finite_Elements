#if !defined(MATRIX_HEADER)
#define MATRIX_HEADER

namespace Memory {
  enum Layout{ROW_MAJOR, COLUMN_MAJOR};
} // namespace Memory {

template <typename Type>
class Matrix {
private:
  // Where the array is actually stored.
  Type * Ar;

  // Dimension of the matrix
  unsigned Num_Rows;
  unsigned Num_Cols;

  // Specify row or column dominant storage scheme
  Memory::Layout Dominance;
public:
  // Constructor
  Matrix(void);

  // Destructor
  ~Matrix(void);


  //////////////////////////////////////////////////////////////////////////////
  // Class methods

  // Set up matrix method
  void Set_Up(const unsigned Rows_In,                                          // Intent: Read
              const unsigned Cols_In,                                          // Intent: Read
              const Memory::Layout Dominance_In);                              // Intent: Read

  // Write to an element of the matrix
  Type & operator()(const unsigned i,                                          // Intent: Read
                    const unsigned j);                                         // Intent: Read

  // Read an eleent of the matrix
  Type operator()(const unsigned i,                                            // Intent: Read
                  const unsigned j) const;                                     // Intent: Read


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
