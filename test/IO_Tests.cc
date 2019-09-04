#include "IO_Tests.h"

void Test::Contains(void) {
  /* First, create some Bufers and some words. */
  const char Buffer1[100] = "Hello, my name is Bob\n";
  const char Buffer2[100] = "*Element, type=C3D8\n";

  const char Word1[20] = "name";
  const char Word2[20] = "C3D8";
  const char Word3[20] = "Hello";
  const char Word4[20] = "type";

  const char BadWord1[20] = "Bobb";
  const char BadWord2[20] = "C3D6";


  /* Check that the Contains function can identify Word1 in Buffer1 and Word2
  in Buffer2*/

  unsigned Tests_Passed = 0;
  unsigned Tests_Failed = 0;

  if(IO::Read::Contains(Buffer1, Word1) == true) { Tests_Passed++; }
  else { Tests_Failed++; }

  if(IO::Read::Contains(Buffer2, Word2) == true) { Tests_Passed++; }
  else { Tests_Failed++; }


  /* Now check that Contains can not identify BadWord1 and BadWord2 in Buffer1
  and Buffer2, respectivly. */

  if(IO::Read::Contains(Buffer1, BadWord1) == false) { Tests_Passed++; }
  else { Tests_Failed++; }

  if(IO::Read::Contains(Buffer2, BadWord2) == false) { Tests_Passed++; }
  else { Tests_Failed++; }


  /* Now check that Contains can identify Word3 and Word4 in Buffer1 and
  Buffer2, respectivly, but that it can't identify them with a sufficiently
  large offset. */

  if(IO::Read::Contains(Buffer1, Word3) == true) { Tests_Passed++; }
  else { Tests_Failed++; }

  if(IO::Read::Contains(Buffer1, Word3, 0) == true) { Tests_Passed++; }
  else { Tests_Failed++; }

  if(IO::Read::Contains(Buffer1, Word3, 3) == false) { Tests_Passed++; }
  else { Tests_Failed++; }

  if(IO::Read::Contains(Buffer1, Word3, 25) == false) { Tests_Passed++; }
  else { Tests_Failed++; }


  if(IO::Read::Contains(Buffer2, Word4, 5) == true) { Tests_Passed++; }
  else { Tests_Failed++; }

  if(IO::Read::Contains(Buffer2, Word4, 10) == true) { Tests_Passed++; }
  else { Tests_Failed++; }

  if(IO::Read::Contains(Buffer2, Word4, 15) == false) { Tests_Passed++; }
  else { Tests_Failed++; }


  printf("Tests Passed: %u\n", Tests_Passed);
  printf("Tests Failed: %u\n", Tests_Failed);
} // void Test::Contains(void) {
