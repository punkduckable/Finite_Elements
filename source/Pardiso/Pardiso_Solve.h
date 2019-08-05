#if !defined(PARDISO_SOLVE_HEADER)
#define PARDISO_SOLVE_HEADER

#define PARDISO_MONITOR

#include <stdio.h>
#include "Matrix.h"
#include "Compress_K.h"
#include "Pardiso.h"

int Pardiso_Solve(const Matrix<double> & K, double* x, double* F);

#endif
