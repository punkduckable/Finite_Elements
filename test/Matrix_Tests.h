#if !defined(MATRIX_TESTS_HEADER)
#define MATRIX_TESTS_HEADER

#include "Errors.h"
#include "Matrix.h"

namespace Test {
  void Matrix_Error_Tests(void);
  void Matrix_Correctness_Tests(void);
  void Print(const Matrix<double> & M);          // Used to print out matricies
} // namespace Test {

#endif
