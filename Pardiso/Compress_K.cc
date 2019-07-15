/* File description:
This is the source file Compress_K. In particular, this file holds the
implementation of the methods for the Compressed_Matrix class */


#include "Compress_K.h"

Compressed_Matrix::Compressed_Matrix(const Matrix<double> & M) {
  /* We require that M is square. */
  assert(M.Get_Num_Rows() == M.Get_Num_Cols());

  /* First, let's determine n_IA. This is just the number of rows in M plus 1.
  allocate IA. Once we know this information, we can allocate IA. */
  unsigned n = M.Get_Num_Rows();
  n_IA = n + 1;
  IA = new unsigned[n_IA];

  /* Now, let's determine n_JA.

  Every diagional element of M as well as any non-zero upper triangular
  components of M must appear in A. Thus, n_JA is just the number of diagional
  elements in M (which is the number of rows in A) plus the number of non-zero
  upper triangular elements in M. */
  n_JA = n;
  for(unsigned i = 0; i < n-1; i++) {
    for(unsigned j = i+1; j < n; j++) {
      if(M(i,j) != 0) { n_JA++; }
    } // for(unsigned j = i+1; j < n; j++) {
  } // for(unsigned i = 0; i < n-1; i++) {

  /* We can now populate the last element of IA (which is n_JA) */
  IA[n] = n_JA;

  /* Now that we know n_Ja, we can allocate JA and A. */
  JA = new unsigned[n_JA];
  A = new double[n_JA];

  /* Finally, let's populate IA, JA, and A.

  IA[i] holds the index of the first component of IA or A from the ith row of A.
  In the case of a symmetric matrix, the first component in the ith row is
  always the ith diagional element of M.

  JA[j] holds the column number of the jth component in A.

  A holds the compressed version of matrix M (see Pardiso notes/manual) */
  unsigned k = 0;            // Keeps track of how many things have been added to JA/A
  for(unsigned i = 0; i < n; i++) {
    /* First, add the ith diagional element of M to A/JA/IA */
    IA[i] = k;
    JA[k] = i;
    A[k] = M(i,i);
    k++;

    /* Now add the non-zero off diagional elements of M into A and JA. */
    for(unsigned j = i+1; j < n; j++) {
      if(M(i,j) != 0) {
        JA[k] = j;
        A[k] = M(i,j);
        k++;
      } // if(M(i,j) != 0) {
    } // for(unsigned j = i+1; j < n; j++) {
  } // for(unsigned i = 0; i < n; i++) {

  #if defined(COMPRESS_K_MONITOR)
    printf("IA: [");
    for(unsigned i = 0; i < n_IA; i++) { printf(" %u ", IA[i]); }
    printf("]\n");

    printf("JA: [");
    for(unsigned j = 0; j < n_JA; j++) { printf(" %d ", JA[j]); }
    printf("]\n");

    printf("A:  [");
    for(unsigned j = 0; j < n_JA; j++) { printf(" %lf ", A[j]); }
    printf("]\n");
  #endif
} // Compressed_Matrix::Compressed_Matrix(const Matrix<T> & M) {


Compressed_Matrix::~Compressed_Matrix() {
  /* de-allocate IA, JA, and A. */
  delete [] IA;
  delete [] JA;
  delete [] A;
} // Compressed_Matrix::~Compressed_Matrix() {
