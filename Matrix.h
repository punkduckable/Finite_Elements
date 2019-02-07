#if !defined(MATRIX_HEADER)
#define MATRIX_HEADER

namespace Memory {
  enum Layout{ROW_DOMINANT, COLUMN_DOMINANT};
} // namespace Memory {

template <typename Type>
class Matrix {
private:
  // Where the array is actually stored.
  Type * Ar;

  // Dimension of the matrix
  unsigned int Num_Rows;
  unsigned int Num_Cols;

  // Specify row or column dominant storage scheme
  Memory::Layout Dominance;
public:
  // Constructor
  Matrix(const unsigned Rows_In,                                               // Intent: Read
         const unsigned Cols_In,                                               // Intent: Read
         const Memory::Layout Dominance_In);                                   // Intent: Read

  // Destructor
  ~Matrix(void);

  //////////////////////////////////////////////////////////////////////////////
  // Read/write operators

  // Write to an element of the matrix
  Type & operator()(const unsigned i,                                          // Intent: Read
                    const unsigned j);                                         // Intent: Read

  // Read an eleent of the matrix
  Type operator()(const unsigned i,                                            // Intent: Read
                  const unsigned j) const;                                     // Intent: Read

  //////////////////////////////////////////////////////////////////////////////
  // Barking methods.
  /* C++ implicitly defines the = operator and copy constructor. I do not want
  either of these to be useable. Therefore, I wrote explicit version of these
  functions that prints an angry message when either of them is used */

  // Barking Copy Constructor
  Matrix(const Matrix<Type> & M_In) {
    printf("The copy constructor is disabled for the Matrix class! BAD!\n");
  } // Matrix(const Matrix<Type> & M_In) {

  // Barking = operator
  Matrix<Type> & operator=(const Matrix<Type> & M_In) {
    printf("Matrix equality is not defined! BAD!\n");
    return *this;
  } //   Matrix & Matrix(const Matrix<Type> & M_In) {
}; // class Matrix {

#include "Matrix.cc"

#endif
