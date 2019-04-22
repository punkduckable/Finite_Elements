#if !defined(MATRIX_TESTS_SOURCE)
#define MATRIX_TESTS_SOURCE

#include "Tests.h"
#include <stdio.h>

void Test::Matrix_Tests(void) {
  printf("Row dominant test:\n");
  unsigned Num_Rows = 3;
  unsigned Num_Cols = 4;

  // First, declare the ROW_MAJOR matrix
  Matrix<int> M1{Num_Rows, Num_Cols, Memory::ROW_MAJOR};

  // Populate M1
  printf("Populating matrix... ");
  for(int i = 0; i < Num_Rows; i++)
    for(int j = 0; j < Num_Cols; j++)
      M1(i,j) = i + j;
  printf("Done!\n");

  // Now, print out M1
  for(int i = 0; i < Num_Rows; i++) {
    // Start each row with a "|"
    printf("|");

    for(int j = 0; j < Num_Cols; j++)
      printf(" %3d ", M1(i,j));

    // End each row with a "|"
    printf(" |\n");
  } // for(int i = 0; i < Num_Rows; i++) {



  printf("\n\nColumn dominant test:\n");
  Num_Rows = 5;
  Num_Cols = 2;

  // First, declare the COLUMN_MAJOR matrix.
  Matrix<double> M2{Num_Rows, Num_Cols, Memory::COLUMN_MAJOR};

  // Populate M2
  printf("Populating Matrix... ");
  for(int j = 0; j < Num_Cols; j++)
    for(int i = 0; i < Num_Rows; i++)
      M2(i,j) = 1./( (double)(i + j + 1));
  printf("Done!\n");

  // Now, print out M2
  for(int i = 0; i < Num_Rows; i++) {
    // Start each row with a "|"
    printf("|");

    for(int j = 0; j < Num_Cols; j++)
      printf(" %6.3lf ", M2(i,j));

    // End each row with a "|"
    printf(" |\n");
  } // for(int i = 0; i < Num_Rows; i++) {
} // void Test::Matrix_Tests(void) {

#endif
