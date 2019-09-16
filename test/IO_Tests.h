#if !defined(IO_TESTS_HEADER)
#define IO_TESTS_HEADER

#include "IO/inp_Reader.h"
#include "IO/String_Ops.h"
#include <string>
#include <vector>
#include <iostream>

namespace Test {
  void Contains();                               // Tests String_Ops::Contains
  void Split();                                  // Tests String_Ops::Split
} // namespace Test {

#endif
