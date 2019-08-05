#if !defined(MATRIX_HEADER)
#define MATRIX_HEADER

#include "Errors.h"
#include <stdio.h>

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


  Matrix<Type> operator*(const Matrix<Type> & Other) const;
  Matrix<Type> operator*(const Type c) const;
  Matrix<Type> & operator+=(const Matrix<Type> & Other);

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

// Move assignement operator
template <typename Type>
Matrix<Type> & Matrix<Type>::operator=(Matrix<Type> && Other) {
  // First, check for self assignment. If Other is *this, then we're already done!
  if(this == &Other)
    return *this;

  /* Now, make sure that Other is compatible with *this. compatability, in this
  case, means that both matricies have the same dimensions and memory layout */
  if( Num_Cols != Other.Num_Cols ||
      Num_Rows != Other.Num_Rows ||
      Memory_Layout != Other.Memory_Layout) {

    char Error_Message_Buffer[500];
    sprintf(Error_Message_Buffer,
            "Matrix Dimension Mismatch Error: you tried to set one matrix equal to\n"
            "another (using the move assignement operator) but those matricies are\n"
            "incompatible. (dimension or memory layout mismatch)\n"
            "M1.Num_Rows = %d         M2.Num_Rows = %d\n"
            "M1.Num_Cols = %d         M2.Num_Cols = %d\n",
            Num_Rows, Other.Num_Rows, Num_Cols, Other.Num_Cols);
    throw Matrix_Dimension_Mismatch(Error_Message_Buffer);
  } // if( Num_Cols != Other.Num_Coll }||...

  // Now, delete our array
  delete [] Ar;

  // Finally, transfer ownership of Ar.
  Ar = Other.Ar;
  Other.Ar = nullptr;

  return *this;
} // Type & Matrix<Type>::operator=(Matrix<Type> && Other) {



// Write to an element of the matrix
template <typename Type>
Type & Matrix<Type>::operator()(const unsigned i, const unsigned j) {
  /* Assumptions:
  This function assumes that i < Num_Rows and that j < Num_Cols. If this is
  not the case then an exception is thrown. */
  if(i >= Num_Rows) {
    char Error_Message_Buffer[500];
    sprintf(Error_Message_Buffer,
            "Matrix Index-out-of-bounds Error: you tried accessing the %dth column\n"
            "however, this matrix only has %d columns (max allowed column index is %d)\n",
            i, Num_Rows, Num_Rows-1);
    throw Matrix_Index_Out_Of_Bounds(Error_Message_Buffer);
  } // if(i >= Num_Rows) {
  else if(j >= Num_Cols) {
    char Error_Message_Buffer[500];
    sprintf(Error_Message_Buffer,
            "Matrix Index-out-of-bounds Error: you tried accessing the %dth column\n"
            "however, this matrix only has %d columns (max allowed column index is %d)\n",
            j, Num_Cols, Num_Cols-1);
    throw Matrix_Index_Out_Of_Bounds(Error_Message_Buffer);
  } // else if(j >= Num_Cols) {


  // Now, return the requested component.
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
  if(i >= Num_Rows) {
    char Error_Message_Buffer[500];
    sprintf(Error_Message_Buffer,
            "Matrix Index-out-of-bounds Error: you tried accessing the %dth column\n"
            "however, this matrix only has %d columns (max allowed column index is %d)\n",
            i, Num_Rows, Num_Rows-1);
    throw Matrix_Index_Out_Of_Bounds(Error_Message_Buffer);
  } // if(i >= Num_Rows) {
  else if(j >= Num_Cols) {
    char Error_Message_Buffer[500];
    sprintf(Error_Message_Buffer,
            "Matrix Index-out-of-bounds Error: you tried accessing the %dth column\n"
            "however, this matrix only has %d columns (max allowed column index is %d)\n",
            j, Num_Cols, Num_Cols-1);
    throw Matrix_Index_Out_Of_Bounds(Error_Message_Buffer);
  } // else if(j >= Num_Cols) {


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
  if(Num_Cols != Other.Num_Rows) {
    char Error_Message_Buffer[500];
    sprintf(Error_Message_Buffer,
            "Matrix Dimension Mismatch Error: you tried to multiply two incompatible matricies\n"
            "For M1*M2 to be defined, the number of columns of M1 must equal the number of\n"
            "rows in M2. In this case, M1.Num_Cols = %d and M2.Num_Rows = %d\n",
            Num_Cols, Other.Num_Rows);
    throw Matrix_Dimension_Mismatch(Error_Message_Buffer);
  } // if(Num_Cols != Other.Num_Rows) {


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



// Scalar-Matrix multiplication
template <typename Type>
Matrix<Type> Matrix<Type>::operator*(const Type c) const {
  /* Function Description:
  This method (and the next one) define scalar-Matrix multiplication. I
  wanted to define both "orders" of scalar-matrix multiplication. That is,
  c*M and M*c. It should be noted that overliaind c*M requires using the
  'two-argument'operator* overload (see below).

  As usual, to improve performance, the way that the product is computed depends
  on the memory layout of the matrix involved. */

  if(Memory_Layout == Memory::ROW_MAJOR) {
    Matrix<Type> Mc{Num_Rows, Num_Cols, Memory::ROW_MAJOR};
    for(unsigned i = 0; i < Num_Rows; i++)
      for(unsigned j = 0; j < Num_Cols; j++)
        Mc.Ar[i*Num_Cols + j] = c*Ar[i*Num_Cols + j];

    return Mc;
  } // if(Memory_Layout == Memory::ROW_MAJOR) {

  else { // if(Memory_Layout == Memory::COLUMN_MAJOR) {
    Matrix<Type> Mc{Num_Rows, Num_Cols, Memory::COLUMN_MAJOR};
    for(unsigned j = 0; j < Num_Cols; j++)
      for(unsigned i = 0; i < Num_Rows; i++)
        Mc.Ar[i + j*Num_Rows] = c*Ar[i + j*Num_Rows];

    return Mc;
  } // else {
} // Matrix<Type> Matrix<Type>::operator*(const Type c) const {

template <typename Type>
Matrix<Type> operator*(Type c, const Matrix<Type> & M) {  return M*c; }



// Compund Matrix-Matrix addition
template <typename Type>
Matrix<Type> & Matrix<Type>::operator+=(const Matrix<Type> & Other) {
  /* Function Description:
  This function defines compound matrix-matrix ddition. */

  /* Assumptions: Matrix-Matrix addition is only defined if both matricies have
  the same dimensions. */
  if(Num_Rows != Other.Num_Rows || Num_Cols != Other.Num_Cols) {
    char Error_Message_Buffer[500];
    sprintf(Error_Message_Buffer,
            "Matrix Dimension Mismatch Error: Thrown by Matrix<Type>::operator+=\n"
            "M1 += M2  is only defined if both matricies have the same dimensions\n"
            "In this case, M1.Num_Rows = %d, M2.Num_Rows = %d, M1.Num_Cols = %d, M2.Num_Cols = %d\n",
            Num_Rows, Other.Num_Rows, Num_Cols, Other.Num_Cols);
    throw Matrix_Dimension_Mismatch(Error_Message_Buffer);
  } // if(Num_Rows != Other.Num_Rows || Num_Cols != Other.Num_Cols) {


  /* Now, perform the addition. I use the memory layout of *this to determine
  how to do the addition. If the two matricies use different memory layout then
  there is no way to optimize the addition. However, the memory layout of both
  matricies is needed to determine how to access the two matricies. */
  if(Memory_Layout == Memory::ROW_MAJOR) {
    if(Other.Memory_Layout == Memory::ROW_MAJOR)
      for(unsigned i = 0; i < Num_Rows; i++)
        for(unsigned j = 0; j < Num_Cols; j++)
          Ar[i*Num_Cols + j] += Other.Ar[i*Num_Cols + j];
    else // if(Other.Memory_Layout == Memory::COLUMN_MAJOR)
      for(unsigned i = 0; i < Num_Rows; i++)
        for(unsigned j = 0; j < Num_Cols; j++)
          Ar[i*Num_Cols + j] += Other.Ar[i + j*Num_Rows];
  } // if(Memory_Layout == Memory::ROW_MAJOR) {
  else { // if(Memory_Layout == COLUMN_MAJOR)
    if(Other.Memory_Layout == Memory::ROW_MAJOR)
      for(unsigned j = 0; j < Num_Cols; j++)
        for(unsigned i = 0; i < Num_Rows; i++)
          Ar[i + j*Num_Rows] += Other.Ar[i*Num_Cols + j];
    else // if(Other.Memory_Layout == Memory::COLUMN_MAJOR)
      for(unsigned j = 0; j < Num_Cols; j++)
        for(unsigned i = 0; i < Num_Rows; i++)
          Ar[i + j*Num_Rows] += Other.Ar[i + j*Num_Rows];
  } // else {

  return *this;
} // Matrix<Type> & operator+=(const Matrix<Type> & Other) const {






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
