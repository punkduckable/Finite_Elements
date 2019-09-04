#include "IO_Tests.h"

void Test::Contains(void) {
  /* First, create some Bufers and some words. */
  const char Buffer1[100] = "Hello, my name is Bob\n";
  const char Buffer2[100] = "*Element, type=C3D8\n";

  const char Word1[20] = "name";
  const char Word2[20] = "C3D8";

  const char BadWord1[20] = "Bobb";
  const char BadWord2[20] = "C3D6";

  /* Now check that the compare function sees the words in the buffers and
  rejects the bad words. */
  unsigned Tests_Passed = 0;
  unsigned Tests_Failed = 0;

  if(IO::Read::Contains(Buffer1, Word1) == true) { Tests_Passed++; }
  else { Tests_Failed++; }

  if(IO::Read::Contains(Buffer2, Word2) == true) { Tests_Passed++; }
  else { Tests_Failed++; }

  if(IO::Read::Contains(Buffer1, BadWord1) == false) { Tests_Passed++; }
  else { Tests_Failed++; }

  if(IO::Read::Contains(Buffer2, BadWord2) == false) { Tests_Passed++; }
  else { Tests_Failed++; }

  printf("Tests Passed: %u\n", Tests_Passed);
  printf("Tests Failed: %u\n", Tests_Failed);
} // void Test::Contains(void) {
