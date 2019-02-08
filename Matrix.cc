#if !defined(MATRIX_SOURCE)
#define MATRIX_SOURCE

#include "Matrix.h"
#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////
// Constructor, destructor

// Constructor
template <typename Type>
Matrix<Type>::Matrix(void) {
  Ar = NULL;
  Num_Rows = 0;
  Num_Cols = 0;
} // Matrix<Type>::Matrix(void) {



template<typename Type>
Matrix<Type>::Matrix(const unsigned Rows_In, const unsigned Cols_In, const Memory::Layout Layout_In) {
  (*this).Set_Up(Rows_In, Cols_In, Layout_In);
} // Matrix<Type>::Matrix(const unsigned Rows_In, const unsigned Cols_In, const Memory::Layout Layout_In) {



// Destructor
template <typename Type>
Matrix<Type>::~Matrix(void) {
  delete [] Ar;
} // Matrix<Type>::~Matrix(void) {





////////////////////////////////////////////////////////////////////////////////
// Class methods

// Set up method
template <typename Type>
void Matrix<Type>::Set_Up(const unsigned Rows_In, const unsigned Cols_In, const Memory::Layout Layout_In) {
  // Allocate the matrix
  Ar = new Type[Rows_In*Cols_In];

  Num_Rows = Rows_In;
  Num_Cols = Cols_In;

  // Set the Layout (this is used to access elements of the matrix)
  Memory_Layout = Layout_In;
} // void Matrix<Type>::Set_Up(const unsigned Rows_In, const unsigned Cols_In, const Memory::Layout Dominace_In) {



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





////////////////////////////////////////////////////////////////////////////////
// Disabled implicit methods

template <typename Type>
Matrix<Type>::Matrix(const Matrix<Type> & M_In) {
  printf("The copy constructor is disabled for the Matrix class! BAD!\n");
} // Matrix<type>::Matrix(const Matrix<Type> & M_In) {



template <typename Type>
Matrix<Type> & Matrix<Type>::operator=(const Matrix<Type> & M_In) {
  printf("Matrix equality is not defined! BAD!\n");
  return *this;
} // Matrix<Type> & Matrix<Type>::operator=(const Matrix<Type> & M_In) {

#endif
