#if !defined(ELEMENT_TESTS_HEADER)
#define ELEMENT_TESTS_HEADER

#include "Errors.h"
#include "Element/Element.h"
#include "Pardiso/Pardiso_Solve.h"
#include "IO/KFX_Writer.h"
#include "IO/vtk_Writer.h"

//#define TEST_MONITOR

namespace Test {
  void Element_Error_Tests(void);
  void Brick_Element(void);
  void Wedge_Element(void);
} // namespace Test {

#endif
