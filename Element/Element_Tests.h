#if !defined(ELEMENT_TESTS_HEADER)
#define ELEMENT_TESTS_HEADER

#include "../Errors.h"
#include "Element.h"

namespace Test {
  void Element_Error_Tests(void);
  void Element(void);

  enum class Printing_Mode{INTEGER, EXP};
  void Print_K_To_File(const Matrix<double> & K,                               // Intent: Read
                       const Printing_Mode Mode = Printing_Mode::EXP);         // Intent: Read
  void Print_F_To_File(const double * F,                                       // Intent: Read
                       const unsigned Num_Global_Eq);                          // Intent: Read
} // namespace Test {

#endif
