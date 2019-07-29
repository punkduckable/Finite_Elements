#include "Pardiso_Tests.h"

void Test::Compress_Matrix(void) {
  /* In this test, we are going to see that the Compress_K function compresses
  the matrix the way that it should. To do this, we first need to make a
  matrix. Since the matrix class can't be initialized, we need to see the
  elements of the matrix one by one.

  To make this easier to understand, I first initialize an array that holds
  the data that I plan on putting in the matrix. I then use a nested for loop
  to transfer items from the array into the matrix. */
  double M_Array[64] = { 7, 0, 1, 0, 0, 2, 7, 0,
                         0,-4, 8, 0, 2, 0, 0, 0,
                         1, 8, 1, 0, 0, 0, 0, 5,
                         0, 0, 0, 7, 0, 0, 9, 0,
                         0, 2, 0, 0, 5,-1, 5, 0,
                         2, 0, 0, 0,-1, 0, 0, 5,
                         7, 0, 0, 9, 7, 0,11, 0,
                         0, 0, 5, 0, 0, 5, 0, 5};

  Matrix<double> M_Matrix{8, 8, Memory::COLUMN_MAJOR};
  for(unsigned i = 0; i < 8; i++) {
    for(unsigned j = 0; j < 8; j++) {
      M_Matrix(i,j) = M_Array[i*8 + j];
    } // for(unsigned j = 0; j < 8; j++) {
  } // for(unsigned i = 0; i < 8; i++) {

  /* Now that we have defined M, let's compress it. */
  Compressed_Matrix M_Compressed{M_Matrix};
} // void Test::Compress_Matrix(void) {
