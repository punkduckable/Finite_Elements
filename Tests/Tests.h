#if !defined(TESTS_HEADER)
#define TESTS_HEADER

#include "../Node.h"
#include "../Element/Element.h"
#include "../Matrix.h"
#include "../Errors.h"

namespace Test {
  void Node_Errors(void);
  void Node(void);

  void Matrix_Tests(void);

  void Element_Errors(void);
  void Element(void);
} // namespace Test


namespace Simulation {
  void Print_K_To_File(const Matrix<double> & K);                              // Intent: Read
  void Print_F_To_File(const double * F,                                       // Intent: Read
                       const unsigned Num_Global_Eq);                          // Intent: Read
} // namespace Simulation {

#endif
