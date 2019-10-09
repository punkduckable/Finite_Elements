#if !defined(MATRIX_TESTS_SOURCE)
#define MATRIX_TESTS_SOURCE

#include "Matrix_Tests.h"
#include <stdio.h>



void Test::Matrix_Error_Tests(void) {
  // First, let's make a matrix
  Matrix<int> M{2, 3, Memory::ROW_MAJOR};

  /* Now, let's try accessing out-of-bounds elements of this matrix, and
  see what happens. */
  printf("Row Out-of-bounds test:\n");
  try {
    // Try accessing something that is in the 3rd row (M only has 2 rows)
    M(2,1) = 4;
  } // try {
  catch (Matrix_Exception Err) {
    printf("Caught a Matrix_Exception!\n");
    printf("%s",Err.what());
  } // catch (Matrix_Exception Err) {

  printf("\nColumn Out-of-bounds test:\n");
  try {
    // Try accessing something that is in the 5th column (M only has 3 columns)
    int x = M(0,4);
    printf("M(0,4) = %d\n",x);
  } // try {
  catch (Matrix_Index_Out_Of_Bounds Err) {
    printf("Caught a Matrix_Index_Out_Of_Bounds Exception!\n");
    printf("%s",Err.what());
  } // catch (Matrix_Index_Out_Of_Bounds Err) {



  printf("\nMatrix-Multiplication dimension-mismatch test:\n");
  try {
    // Try multiplying two matricies that are incompatible
    Matrix<double> M1{5,2, Memory::ROW_MAJOR};
    Matrix<double> M2{3,4, Memory::COLUMN_MAJOR};

    Matrix<double> M3 = M1*M2;
  } // try {
  catch (Matrix_Dimension_Mismatch Err) {
    printf("Caught a Matrix_Dimension_Mismatch exception!\n");
    printf("%s",Err.what());
  } // catch (Matrix_Dimension_Mismatch Err) {
} // void Test::Matrix_Error_Tests(void) {



void Test::Matrix_Correctness_Tests(void) {
  printf("Row dominant test:\n");
  unsigned Num_Rows = 3;
  unsigned Num_Cols = 4;

  // First, declare the ROW_MAJOR matrix
  Matrix<double> M1{Num_Rows, Num_Cols, Memory::ROW_MAJOR};

  // Populate M1
  printf("Populating matrix... ");
  for(unsigned i = 0; i < Num_Rows; i++)
    for(unsigned j = 0; j < Num_Cols; j++)
      M1(i,j) = i + j;
  printf("Done!\n");

  // Now, print out M1
  Print(M1);



  printf("\nColumn dominant test:\n");
  Num_Rows = 5;
  Num_Cols = 2;

  // First, declare the COLUMN_MAJOR matrix.
  Matrix<double> M2{Num_Rows, Num_Cols, Memory::COLUMN_MAJOR};

  // Populate M2
  printf("Populating Matrix... ");
  for(unsigned j = 0; j < Num_Cols; j++)
    for(unsigned i = 0; i < Num_Rows; i++)
      M2(i,j) = 1./( (double)(i + j + 1));
  printf("Done!\n");

  // Now, print out M2
  Print(M2);



  printf("\nMatrix-Matrix Multiplication test:\n");
  Matrix<double> M3{2,4,Memory::COLUMN_MAJOR};
  Matrix<double> M4{4,3,Memory::COLUMN_MAJOR};

  // Populate and Print M3
  for(unsigned i = 0; i < M3.Get_Num_Rows(); i++)
    for(unsigned j = 0; j < M3.Get_Num_Cols(); j++)
      M3(i,j) = i+j;

  printf("M3:\n");
  Print(M3);

  // Populate and Print M4
  for(unsigned i = 0; i < M4.Get_Num_Rows(); i++)
    for(unsigned j = 0; j < M4.Get_Num_Cols(); j++)
      M4(i,j) = i+j;

  printf("M4:\n");
  Print(M4);

  // Now, compute their product and print it out
  Matrix<double> M5 = M3*M4;
  printf("M5 = M3*M4:\n");
  Print(M5);

  printf("M5 Memory Layout: ");
  if(M5.Get_Memory_Layout() == Memory::ROW_MAJOR)
    printf("ROW_MAJOR\n");
  else
    printf("COLUMN_MAJOR\n");



  printf("\nMatrix-scalar multiplication test:\n");
  Matrix<double> M6{4,5, Memory::ROW_MAJOR};

  // Populate and print M6
  for(unsigned i = 0; i < M6.Get_Num_Rows(); i++)
    for(unsigned j = 0; j < M6.Get_Num_Cols(); j++)
      M6(i,j) = 1;
  printf("M6:\n");
  Print(M6);

  // compute and print c*M6 and M7 *c
  Matrix<double> M7 = M6*4.;
  printf("M7 = 5*M6:\n");
  Print(M7);

  Matrix<double> M8 = 5.*M7;
  printf("5*M7:\n");
  Print(M8);



  printf("\nCompound Matrix=Matrix Addition test:\n");

  // Initialize and populate M9
  Matrix<double> M9{5, 4, Memory::COLUMN_MAJOR};
  M9.Fill(0);
  for(int i = 0; i < 4; i++)
    M9(i,i) = 1;

  // Print M9
  printf("M9:\n");
  Print(M9);

  // Initialize and populate M10
  Matrix<double> M10{5, 4, Memory::COLUMN_MAJOR};
  M10.Fill(0);
  for(int i = 0; i < 4; i++)
    M10(i,i) = i;

  // Print M10
  printf("M10:\n");
  Print(M10);

  M9 += M10;
  printf("M9 after += M10\n");
  Print(M9);
} // void Test::Matrix_Correctness_Tests(void) {



void Test::Print(const Matrix<double> & M) {
  // Loop through the rows of M, printing out each one.
  for(unsigned i = 0; i < M.Get_Num_Rows(); i++) {
    // First, Print out a vertical bar (for the start of the row)
    printf("|");

    // Now, print out the contents of the ith row
    for(unsigned j = 0; j < M.Get_Num_Cols(); j++)
      printf(" %6.3f ", M(i,j));

    // Finish with another vertical bar before moving onto the next row.
    printf(" |\n");
  } // for(unsigned i = 0; i < M.Get_Num_Rows(); i++) {
} // void Test::Print(const Matrix<double> & M) {

#endif
