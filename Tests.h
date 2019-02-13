#if !defined(TESTS_HEADER)
#define TESTS_HEADER

#include "Node.h"
#include "Element.h"
#include "Matrix.h"
#include "Errors.h"

namespace Test {
  void Node_Errors(void);
  void Node(void);

  void Matrix_Tests(void);

  void Element(void);
} // namespace Test


namespace Simulation {
  double F(unsigned, unsigned, unsigned, unsigned);
  void Print_K_To_File(const Matrix<double> & K);
} // namespace Simulation {

#endif
