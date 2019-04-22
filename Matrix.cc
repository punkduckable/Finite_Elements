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
  This function assumes that i < Num_Rows and that j < Num_Cols. This Class has
  no way of handeling errors (like the Element, Node classes do). Therefore,
  we can't really do anything if these assumptions are not  met. Therefore,
  we simply assume that the user does not try to go out of bounds. */
  if(Memory_Layout == Memory::ROW_MAJOR)
    return Ar[Num_Cols*i + j];
  else // (Memory_Layout == Memory::COLUMN_MAJOR)
    return Ar[i + j*Num_Rows];
} // Type & Matrix<Type>::operator()(const unsigned i, const unsigned j) {



// Read an element of the Matrix
template <typename Type>
Type Matrix<Type>::operator()(const unsigned i, const unsigned j) const {
  /* Assumptions:
  This function assumes that i < Num_Rows and that j < Num_Cols. This Class has
  no way of handeling errors (like the Element, Node classes do). Therefore,
  we can't really do anything if these assumptions are not  met. Therefore,
  we simply assume that the user does not try to go out of bounds. */
  if(Memory_Layout == Memory::ROW_MAJOR)
    return Ar[Num_Cols*i + j];
  else // (Memory_Layout == Memory::COLUMN_MAJOR)
    return Ar[i + j*Num_Rows];
} // Type Matrix<Type>::operator()(const unsigned i, const unsigned j) const {

#endif
