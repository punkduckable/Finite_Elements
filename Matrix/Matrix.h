#if !defined(MATRIX_HEADER)
#define MATRIX_HEADER

#include "../Errors.h"

enum class Memory{ROW_MAJOR, COLUMN_MAJOR};

template <typename Type>
class Matrix {
private:
  // Where the array is actually stored.
  Type * Ar;

  // Dimension of the matrix
  const unsigned Num_Rows;
  const unsigned Num_Cols;

  // Specify row or column major storage scheme
  const Memory Memory_Layout;
public:
  // Constructor
  Matrix(const unsigned Rows_In,                                               // Intent: Read
         const unsigned Cols_In,                                               // Intent: Read
         const Memory Layout_In);                                              // Intent: Read

  // Deleted Copy constructor
  Matrix(const Matrix & Other) = delete;

  // Move constructor
  Matrix(Matrix && Other);

  // Destructor
  ~Matrix(void);


  //////////////////////////////////////////////////////////////////////////////
  // Operator overloads

  Matrix<Type> & operator=(const Matrix<Type> & Other) = delete;     // Delete Copy Assignement operator
  Matrix<Type> & operator=(Matrix<Type> && Other);                   // Move assignement operator

  // Write to an element of the matrix
  Type & operator()(const unsigned i,                                          // Intent: Read
                    const unsigned j);                                         // Intent: Read

  // Read an element of the matrix
  Type operator()(const unsigned i,                                            // Intent: Read
                  const unsigned j) const;                                     // Intent: Read


  // Matrix-Matrix multiplication
  Matrix<Type> operator*(const Matrix<Type> & Other) const;

  // Matrix-Scalar multiplication
  Matrix<Type> operator*(const Type c) const;

  //////////////////////////////////////////////////////////////////////////////
  // Getter methods

  unsigned Get_Num_Rows(void) const { return Num_Rows; }
  unsigned Get_Num_Cols(void) const { return Num_Cols; }
  Memory Get_Memory_Layout(void) const { return Memory_Layout; }


  //////////////////////////////////////////////////////////////////////////////
  // Disabled implicit methods
  /* C++ implicitly defines the copy assignment operator and copy constructor.
  Both of these implicit methods use a shallow copy. The matrix class has
  dynamically allocated memory. Therefore, a shallow copy to a temporary
  object could end up de-allocating memory that should not be de-allocated,
  leading to disaster. I do not want either of these to be useable for the
  matrix class.

  Further, it really doesn't make any sense (in the context of what my code
  does) to be able to assign one matrix to another matrix that is an L-value
  or to construct a matrix from another L-value Matrix.

  Thus, to eliminate potential errors, I explicitly delete both of these methods.

  It should be noted that the Matrix class does have a Move constructor and
  Move Assignement operator. */

  //////////////////////////////////////////////////////////////////////////////
  // Other methods

  void Zero(void);
}; // class Matrix {

#include "Matrix.cc"

#endif
