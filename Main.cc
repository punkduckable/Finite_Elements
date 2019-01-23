#include <stdio.h>


// Include local header files
#include "Forward_Declarations.h"
#include "Array.h"
#include "Node.h"
#include "Element.h"
#include "Tests.h"

// Include local source files
#include "Node.cc"
#include "Element.cc"
#include "Tests.cc"

int main() {
  // Run node tests.
  Test::Node();
  
  return 0;
} // int main() {
