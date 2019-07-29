/* File description:
This is the source file Compress_K. In particular, this file holds the
implementation of the methods for the Compressed_Matrix class */


#include "Compress_K.h"

Compressed_Matrix::Compressed_Matrix(const Matrix<double> & M) {
  /* We require that M is square. */
  assert(M.Get_Num_Rows() == M.Get_Num_Cols());

  /* First, let's determine n_IA. This is just the number of rows in M plus 1.
  allocate IA. Once we know this information, we can allocate IA. */
  int n = (int)M.Get_Num_Rows();
  n_IA = n + 1;
  IA = new int[n_IA];


  /* Now, let's determine n_JA. The way that we do this depends on the memory
  layout of M.

  If M is row major, then every diagional element of M as well as all non-zero
  upper triangular components of M must appear in A.

  If M is column major then each diagional components as well as all non-zero
  off diagional components of M must appear in A.

  In either case, n_Ja is just the number of components that meet the criteria.
  For row major matricies, n_JA is the number of diagional components (which is
  just the number of rows in A) plus the number of non-zero upper triangular
  components of M. For column major matricies, n_JA is the number of diagional
  components of M plus the number of non-zero lower triangular components of M. */
  const Memory Memory_Layout = M.Get_Memory_Layout();
  n_JA = n;
  if(Memory_Layout == Memory::ROW_MAJOR) {
    for(int i = 0; i < n-1; i++) {
      for(int j = i+1; j < n; j++) {
        if(M(i,j) != 0) { n_JA++; }
      } // for(int j = i+1; j < n; j++) {
    } // for(int i = 0; i < n-1; i++) {
  } // if(Memory_Layout == Memory::ROW_MAJOR) {
  else { // if(Memory_Layout == Memory::COLUMN_MAJOR)
    for(int j = 0; j < n-1; j++) {
      for(int i = j+1; i < n; i++) {
        if(M(i,j) != 0) { n_JA++; }
      } // for(int i = j+1; i < n; i++) {
    } // for(int j = 0; j < n-1; j++) {
  } // else

  /* We can now populate the last element of IA (which is n_JA) */
  IA[n] = n_JA;

  /* Now that we know n_Ja, we can allocate JA and A. */
  JA = new int[n_JA];
  A = new double[n_JA];

  /* Finally, let's populate IA, JA, and A.

  The way that we do this depends on the memory layout of M. If M is ROW_MAJOR
  then IA[i] holds the index of the first component of JA or A from the ith row
  of A. Since M is symmetric, the first component in the ith row is always the
  ith diagional element of M. JA[j] holds the column number of the jth component
  in A. A holds the compressed version of matrix M (see Pardiso notes/manual)

  If, by contrast, M is COLUMN_MAJOR, then IA[j] holds the index of the first
  component of JA or A that is from the jth row of A (which is just the jth
  diagional element of M). JA[i] holds the row number of the ith component in A
  and A holds the compressed version of matrix M.  */
  int k = 0;            // Keeps track of how many things have been added to JA/A
  if(Memory_Layout == Memory::ROW_MAJOR)
    for(int i = 0; i < n; i++) {
      /* First, add the ith diagional element of M to A/JA/IA */
      IA[i] = k;
      JA[k] = i;
      A[k] = M(i,i);
      k++;

      /* Now add the non-zero off diagional elements of M into A and JA. */
      for(int j = i+1; j < n; j++) {
        if(M(i,j) != 0) {
          JA[k] = j;
          A[k] = M(i,j);
          k++;
        } // if(M(i,j) != 0) {
      } // for(int j = i+1; j < n; j++) {
    } // for(int i = 0; i < n; i++) {
  else { // if(Memory_Layout == Memory::COLUMN_MAJOR)
    for(int j = 0; j < n; j++) {
      /* First, add the jth diagional element of M to A/JA/Ia */
      IA[j] = k;
      JA[k] = j;
      A[k] = M(j,j);
      k++;

      /* Now add the non-zero off diagional elements of M into A and JA. */
      for(int i = j+1; i < n; i++) {
        if(M(i,j) != 0) {
          JA[k] = i;
          A[k] = M(i,j);
          k++;
        } // if(M(i,j) != 0) {
      } // for(int i = j+1; i < n; i++) {
    } // for(int j = 0; j < n; j++) {
  } // elese {

  #if defined(COMPRESS_K_MONITOR)
    printf("M is ");
    if(Memory_Layout == Memory::ROW_MAJOR) { printf("ROW_MAJOR\n"); }
    else { printf("COLUMN_MAJOR\n"); }

    printf("IA: [");
    for(int i = 0; i < n_IA; i++) { printf(" %u", IA[i]); }
    printf(" ]\n");

    printf("JA: [");
    for(int j = 0; j < n_JA; j++) { printf(" %d", JA[j]); }
    printf(" ]\n");

    printf("A:  [");
    for(int j = 0; j < n_JA; j++) { printf(" %4.1lf", A[j]); }
    printf(" ]\n");
  #endif
} // Compressed_Matrix::Compressed_Matrix(const Matrix<T> & M) {


Compressed_Matrix::~Compressed_Matrix() {
  /* de-allocate IA, JA, and A. */
  delete [] IA;
  delete [] JA;
  delete [] A;
} // Compressed_Matrix::~Compressed_Matrix() {
