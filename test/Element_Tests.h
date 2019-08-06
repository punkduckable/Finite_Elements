#if !defined(ELEMENT_TESTS_HEADER)
#define ELEMENT_TESTS_HEADER

#include "Errors.h"
#include "Element/Element.h"
#include "Pardiso/Pardiso_Solve.h"
#include "IO/KFX_Writer.h"

namespace Test {
  void Element_Error_Tests(void);
  void Element(void);
} // namespace Test {

#endif
