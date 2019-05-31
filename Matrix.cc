#if !defined(MATRIX_SOURCE)
#define MATRIX_SOURCE

#include "Matrix.h"
#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////
// Constructor, destructor

template<typename Type>
Matrix<Type>::Matrix(const unsigned Rows_In,
                     const unsigned Cols_In,
                     const Memory Layout_In)
                     : Num_Rows(Rows_In),
                     Num_Cols(Cols_In),
                     Memory_Layout(Layout_In) {

  // Allocate the matrix
  Ar = new Type[Rows_In*Cols_In];
} // Matrix<Type>::Matrix(const unsigned Rows_In,...



// Destructor
template <typename Type>
Matrix<Type>::~Matrix(void) {
  delete [] Ar;
} // Matrix<Type>::~Matrix(void) {





////////////////////////////////////////////////////////////////////////////////
// Class methods


// Write to an element of the matrix
template <typename Type>
Type & Matrix<Type>::operator()(const unsigned i, const unsigned j) {
  /* Assumptions:
  This function assumes that i < Num_Rows and that j < Num_Cols. If this is
  not the case then an exception is thrown. */
  if(i >= Num_Rows)
    throw Matrix_Exceptions::Index_Out_Of_Bounds(i, Num_Rows);
  else if(j >= Num_Cols)
    throw Matrix_Exceptions::Index_Out_Of_Bounds(j, Num_Cols);


  if(Memory_Layout == Memory::ROW_MAJOR)
    return Ar[Num_Cols*i + j];
  else // (Memory_Layout == Memory::COLUMN_MAJOR)
    return Ar[i + j*Num_Rows];
} // Type & Matrix<Type>::operator()(const unsigned i, const unsigned j) {



// Read an element of the Matrix
template <typename Type>
Type Matrix<Type>::operator()(const unsigned i, const unsigned j) const {
  /* Assumptions:
  This function assumes that i < Num_Rows and that j < Num_Cols. If this is
  not the case then an exception is thrown. */
  if(i >= Num_Rows)
    throw Matrix_Exceptions::Index_Out_Of_Bounds(i, Num_Rows);
  else if(j >= Num_Cols)
    throw Matrix_Exceptions::Index_Out_Of_Bounds(j, Num_Cols);


  // Now, return the requested component.
  if(Memory_Layout == Memory::ROW_MAJOR)
    return Ar[Num_Cols*i + j];
  else // (Memory_Layout == Memory::COLUMN_MAJOR)
    return Ar[i + j*Num_Rows];
} // Type Matrix<Type>::operator()(const unsigned i, const unsigned j) const {


// Matrix-Matrix multiplication
//template <typename Type>
//Matrix<Type> Matrix<Type>::operator*(const Matrix<Type> & Other) const {
//  /* This method is designed to handle matrix-matrix multiplication. In theory,
//  we could write an algorithm that compuated the product using the access
//  operators (). This would work and be fairly easy to read, but it would be
//  slow (since each read/write to the matrix would need to call the () operator).
//  Rather, I choose to compute the product by directly pulling from and writing
//  to the arrays underlying each matrix. This is not as readible, but it's fast.
//  Crucially this also means that the way that the product is computed depends
//  on the memory layout of both matricies involved. */
//
//  /* Assumptions:
//  This method assumes that the two matricies "match". This means that the
//  number of columns in *this is the same as the number of rows in Other.
//  Matrix multiplication is only defined if this is the case. Thus, if this
//  assumption fails then we throw an exception. */
//  if(Num_Cols != Other.Num_Rows)
//    throw Matrix_Exceptions::Dimension_Mismatch(Num_Cols, Other.Num_Rows);
//
//  /* Now, compute the product depending on the memory layout of *this and
//  Other. It should be noted that this memory layout determins the memory
//  layout of the product. */
//
//  if(Memory_Layout == Memory::ROW_MAJOR) {
//
//  }
//}
#endif
