#if !defined(COMPRESS_K_HEADER)
#define COMPRESS_K_HEADER

#include "../Matrix.h"
#include <assert.h>

#define COMPRESS_K_MONITOR

/* Compressed matrix class.
This class is used to convert a regular matrix into a compressed matrix that
Pardiso can understand. Thus, it generates IA, JA, and A from M (the input).
To make things easier to work with, everything in this class is public.

This class really only exists for one purpose, to convert K into a format that
Pardiso can understand. This class should not be used in any other context.

NOTE: this class assumes that M is and symmetric. */
class Compressed_Matrix {
  public:
    // Constructor, destructor
    Compressed_Matrix(const Matrix<double> & M);
    ~Compressed_Matrix();

    /* IA, JA, and A (see Pardiso notes and manual). These are all ints because
    pardiso expects them to be ints */
    int* IA;
    int n_IA;
    int* JA;
    int n_JA;
    double* A;
};

#endif
