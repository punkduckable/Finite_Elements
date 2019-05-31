#if !defined(MATRIX_TESTS_SOURCE)
#define MATRIX_TESTS_SOURCE

#include "Tests.h"
#include <stdio.h>


void Test::Matrix_Error_Tests(void) {
  // First, let's make a matrix
  Matrix<int> M{2, 3, Memory::ROW_MAJOR};

  /* Now, let's try accessing out-of-bounds elements of this matrix, and
  see what happens. */
  try {
    // Try accessing something that is in the 3rd row (M only has 2 rows)
    printf("Row Out-of-bounds test...\n");
    M(2,1) = 4;
  } // try {
  catch (Matrix_Exceptions::Index_Out_Of_Bounds Err) {
    printf("Caught an Index_Out_Of_Bounds exception!\n");
    Err.Msg();
  } // catch (Matrix_Exceptions::Index_Out_Of_Bounds Err) {

  try {
    // Try accessing something that is in the 5th column (M only has 3 columns)
    printf("Column Out-of-bounds test...\n");
    int x = M(0,4);
    printf("M(0,4) = %d\n",x);
  } // try {
  catch (Matrix_Exceptions::Index_Out_Of_Bounds Err) {
    printf("Caught an Index_Out_Of_Bounds exception!\n");
    Err.Msg();
  } // catch (Matrix_Exceptions::Index_Out_Of_Bounds Err) {
} // void Test::Matrix_Error_Tests(void) {



void Test::Matrix_Correctness_Tests(void) {
  printf("Row dominant test:\n");
  unsigned Num_Rows = 3;
  unsigned Num_Cols = 4;

  // First, declare the ROW_MAJOR matrix
  Matrix<int> M1{Num_Rows, Num_Cols, Memory::ROW_MAJOR};

  // Populate M1
  printf("Populating matrix... ");
  for(unsigned i = 0; i < Num_Rows; i++)
    for(unsigned j = 0; j < Num_Cols; j++)
      M1(i,j) = i + j;
  printf("Done!\n");

  // Now, print out M1
  for(unsigned i = 0; i < Num_Rows; i++) {
    // Start each row with a "|"
    printf("|");

    for(unsigned j = 0; j < Num_Cols; j++)
      printf(" %3d ", M1(i,j));

    // End each row with a "|"
    printf(" |\n");
  } // for(unsigned i = 0; i < Num_Rows; i++) {



  printf("\n\nColumn dominant test:\n");
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
  for(unsigned i = 0; i < Num_Rows; i++) {
    // Start each row with a "|"
    printf("|");

    for(unsigned j = 0; j < Num_Cols; j++)
      printf(" %6.3lf ", M2(i,j));

    // End each row with a "|"
    printf(" |\n");
  } // for(unsigned i = 0; i < Num_Rows; i++) {
} // void Test::Matrix_Correctness_Tests(void) {

#endif
