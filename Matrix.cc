#if !defined(MATRIX_SOURCE)
#define MATRIX_SOURCE

#include "Matrix.h"
#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////
// Constructor, destructor

// Constructor
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



// Move constructor
template <typename Type>
Matrix<Type>::Matrix(Matrix<Type> && Other) : Num_Rows(Other.Num_Rows),
                                              Num_Cols(Other.Num_Cols),
                                              Memory_Layout(Other.Memory_Layout) {
  // Transfer ownership of Ar.
  Ar = Other.Ar;
  Other.Ar = nullptr;
} // Matrix<Type>::Matrix(Matrix<Type> && Other) {



// Destructor
template <typename Type>
Matrix<Type>::~Matrix(void) {
  delete [] Ar;
} // Matrix<Type>::~Matrix(void) {





////////////////////////////////////////////////////////////////////////////////
// Operator overloads


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
template <typename Type>
Matrix<Type> Matrix<Type>::operator*(const Matrix<Type> & Other) const {
  /* Function Description:
  This method is designed to handle matrix-matrix multiplication.

  To improve performance (memory usage), the way that I compute the product
  depends on the memory layout of both matricies. */

  /* Assumptions:
  This method assumes that the two matricies "match". This means that the
  number of columns in *this is the same as the number of rows in Other.
  Matrix multiplication is only defined if this is the case. Thus, if this
  assumption fails then we throw an exception. */
  if(Num_Cols != Other.Num_Rows)
    throw Matrix_Exceptions::Dimension_Mismatch(Num_Cols, Other.Num_Rows);

  /* Now, compute the product depending on the memory layout of *this and
  Other. It should be noted that this memory layout determins the memory
  layout of the product.

  Note: multiplication always works from Left-to-Right. Thus, something like
  M1*M2*M3 gets processed as (M1*M2)*M3. Therefore, it is worth assuming that
  the matrix computed here will end up on the left side of any future
  multiplications.*/
  const unsigned Other_Num_Rows = Other.Num_Rows;
  const unsigned Other_Num_Cols = Other.Num_Cols;
  const unsigned Product_Num_Rows = Num_Rows;
  const unsigned Product_Num_Cols = Other_Num_Cols;

  if(Memory_Layout == Memory::ROW_MAJOR) {
    if(Other.Memory_Layout == Memory::ROW_MAJOR) {
      /* When both matricies involved are row-major, it is impossible to
      efficiently store their product in a column-major matrix. Thus,
      the product is stored in a Row-major matrix. */
      Matrix<Type> Product{Product_Num_Rows, Product_Num_Cols, Memory::ROW_MAJOR};

      /* First, zero the Product */
      Product.Zero();

      /* Next, compute the product. In this situation, memory usage is
      optimized if order of the loops is i-k-j.*/
      for(unsigned  i = 0; i < Product_Num_Rows; i++)
        for(unsigned  k = 0; k < Num_Cols; k++)
          for(unsigned  j = 0; j < Product_Num_Cols; j++)
            Product.Ar[i*Product_Num_Cols + j] += Ar[i*Num_Cols + k]*Other.Ar[k*Other_Num_Cols + j];

      /* Return the product (Note: this will invoke the Move-constructor) */
      return Product;
    } // if(Other.Memory_Layout == Memory::ROW_MAJOR)

    else { // if(Other.Memory_Layout == Memory::COLUMN_MAJOR)
      /* In this case, the memory layout of the product is irrelivent. In
      either case, the order of the loops can be optimized. My code tends to
      prefer Column-major matricies, so I made the product Column-major */
      Matrix<Type> Product{Product_Num_Rows, Product_Num_Cols, Memory::COLUMN_MAJOR};

      /* First, zero the Product */
      Product.Zero();

      /* Next, compute the product. In this situation, memory usage is optimized
      if order of the loops is j-i-k.*/
      for(unsigned  j = 0; j < Product_Num_Cols; j++)
        for(unsigned  i = 0; i < Product_Num_Rows; i++)
          for(unsigned  k = 0; k < Num_Cols; k++)
            Product.Ar[i + j*Product_Num_Rows] += Ar[i*Num_Cols + k]*Other.Ar[k + j*Other_Num_Rows];

      /* Return the product (Note: this will invoke the Move-constructor) */
      return Product;
    } // else {
  } // if(Memory_Layout == Memory::ROW_MAJOR) {

  else { // if(Memory_Layout == Memory::COLUMN_MAJOR)
    if(Other.Memory_Layout == Memory::ROW_MAJOR) {
      /* In this case, the memory layout of the product is irrelivent. In
      both cases, the order of the loops can be optimized. My code tends to
      prefer Column-major matricies, so I made the product Column-major */
      Matrix<Type> Product{Product_Num_Rows, Product_Num_Cols, Memory::COLUMN_MAJOR};

      /* First, zero the Product */
      Product.Zero();

      /* Next, compute the product. In this situation, memory usage is optimized
      if order of the loops is k-j-i.*/
      for(unsigned  k = 0; k < Num_Cols; k++)
        for(unsigned  j = 0; j < Product_Num_Cols; j++)
          for(unsigned  i = 0; i < Product_Num_Rows; i++)
            Product.Ar[i + j*Product_Num_Rows] += Ar[i + k*Num_Rows]*Other.Ar[k*Other_Num_Cols + j];

      /* Return the product (Note: this will invoke the Move-constructor) */
      return Product;
    } // if(Other.Memory_Layout == Memory::COLUMN_MAJOR) {

    else { // if(Other.Memory_Layout == Memory::COLUMN_MAJOR)
      /* When both matricies involved are column-major, it is impossible to
      efficiently store their product in a row-major matrix. Thus,
      the product is stored in a column-major matrix. */
      Matrix<Type> Product{Product_Num_Rows, Product_Num_Cols, Memory::COLUMN_MAJOR};

      /* First, zero the Product */
      Product.Zero();

      /* Next, compute the product. In this situation, memory usage is optimized
      if order of the loops is k-j-i.*/
      for(unsigned  j = 0; j < Product_Num_Cols; j++)
        for(unsigned  k = 0; k < Num_Cols; k++)
          for(unsigned  i = 0; i < Product_Num_Rows; i++)
            Product.Ar[i + j*Product_Num_Rows] += Ar[i + k*Num_Rows]*Other.Ar[k + j*Other_Num_Rows];

      /* Return the product (Note: this will invoke the Move-constructor) */
      return Product;
    } // else {
  } // else {
} // Matrix<Type> Matrix<Type>::operator*(const Matrix<Type> & Other) const





////////////////////////////////////////////////////////////////////////////////
// Other methods

// Zeros out the matrix
template <typename Type>
void Matrix<Type>::Zero(void) {
  /* Function Description:
  This function is designed to "zero out" the elements of a Matrix.

  This function was originally created to promote code reuse in the
  Matrix-Matrix multiplication method. I decided to make it a part of the public
  interfact so that other methods/functions can use this method as well.

  As is usual, the way that this method works depends on the Memory Layout
  of the matrix (to optimize memory usage). */
  
  if(Memory_Layout == Memory::ROW_MAJOR) {
    for(unsigned i = 0; i < Num_Rows; i++)
      for(unsigned j = 0; j < Num_Cols; j++)
        Ar[i*Num_Cols + j] = 0;
  } // if(Memory_Layout == Memory::ROW_MAJOR) {
  else { // if(Memory_Layout == Memory::COLUMN_MAJOR)
    for(unsigned j = 0; j < Num_Cols; j++)
      for(unsigned i = 0; i < Num_Rows; i++)
        Ar[i + j*Num_Rows] = 0;
  } // else {
} // void Matrix<Type>::Zero(void) {

#endif
