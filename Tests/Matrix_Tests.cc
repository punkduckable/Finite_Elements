#if !defined(MATRIX_TESTS_SOURCE)
#define MATRIX_TESTS_SOURCE

#include "Tests.h"
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
  catch (Matrix_Exceptions::Index_Out_Of_Bounds Err) {
    printf("Caught an Index_Out_Of_Bounds exception!\n");
    Err.Msg();
    printf("\n");
  } // catch (Matrix_Exceptions::Index_Out_Of_Bounds Err) {

  printf("\nColumn Out-of-bounds test:\n");
  try {
    // Try accessing something that is in the 5th column (M only has 3 columns)
    int x = M(0,4);
    printf("M(0,4) = %d\n",x);
  } // try {
  catch (Matrix_Exceptions::Index_Out_Of_Bounds Err) {
    printf("Caught an Index_Out_Of_Bounds exception!\n");
    Err.Msg();
  } // catch (Matrix_Exceptions::Index_Out_Of_Bounds Err) {



  printf("\nMatrix-Multiplication dimension-mismatch test:\n");
  try {
    // Try multiplying two matricies that are incompatible
    Matrix<double> M1{5,2, Memory::ROW_MAJOR};
    Matrix<double> M2{3,4, Memory::COLUMN_MAJOR};

    Matrix<double> M3 = M1*M2;
  } // try {
  catch (Matrix_Exceptions::Dimension_Mismatch Err) {
    printf("Caught a dimension-mismatch exception!\n");
    Err.Msg();
  } // catch (Matrix_Exceptions::Dimension_Mismatch Err) {
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
    printf("|");
    for(unsigned j = 0; j < Num_Cols; j++)
      printf(" %3d ", M1(i,j));
    printf(" |\n");
  } // for(unsigned i = 0; i < Num_Rows; i++) {



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
  for(unsigned i = 0; i < Num_Rows; i++) {
    printf("|");
    for(unsigned j = 0; j < Num_Cols; j++)
      printf(" %6.3lf ", M2(i,j));
    printf(" |\n");
  } // for(unsigned i = 0; i < Num_Rows; i++) {



  printf("\nMatrix-Matrix Multiplication test:\n");
  Matrix<double> M3{2,4,Memory::COLUMN_MAJOR};
  Matrix<double> M4{4,3,Memory::COLUMN_MAJOR};

  // Populate and Printout M3
  for(unsigned i = 0; i < M3.Get_Num_Rows(); i++)
    for(unsigned j = 0; j < M3.Get_Num_Cols(); j++)
      M3(i,j) = i+j;

  printf("M3:\n");
  for(unsigned i = 0; i < M3.Get_Num_Rows(); i++) {
    printf("|");
    for(unsigned j = 0; j < M3.Get_Num_Cols(); j++)
      printf(" %6.3lf ", M3(i,j));
    printf(" |\n");
  } // for(unsigned i = 0; i < M3.Get_Num_Rows(); i++) {

  // Populate and Printout M4
  for(unsigned i = 0; i < M4.Get_Num_Rows(); i++)
    for(unsigned j = 0; j < M4.Get_Num_Cols(); j++)
      M4(i,j) = i+j;

  printf("M4:\n");
  for(unsigned i = 0; i < M4.Get_Num_Rows(); i++) {
    printf("|");
    for(unsigned j = 0; j < M4.Get_Num_Cols(); j++)
      printf(" %6.3lf ", M4(i,j));
    printf(" |\n");
  } // for(unsigned i = 0; i < M4.Get_Num_Rows(); i++) {
    
  // Now, compute their product and print it out
  Matrix<double> M5 = M3*M4;
  printf("M3*M4:\n");
  for(unsigned i = 0; i < M5.Get_Num_Rows(); i++) {
    printf("|");
    for(unsigned j = 0; j < M5.Get_Num_Cols(); j++)
      printf(" %6.3lf ", M5(i,j));
    printf(" |\n");
  } // for(unsigned i = 0; i < M5.Get_Num_Rows(); i++) {

  printf("M5 Memory Layout: ");
  if(M5.Get_Memory_Layout() == Memory::ROW_MAJOR)
    printf("ROW_MAJOR\n");
  else
    printf("COLUMN_MAJOR\n");
} // void Test::Matrix_Correctness_Tests(void) {

#endif
