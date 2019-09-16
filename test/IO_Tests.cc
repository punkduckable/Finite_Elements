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



void Test::Split(void) {
  /* First, create some test strigs */
  std::string S1{"  10,  12,  75,  76,  77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  89,  90\n"};
  std::string S2{" 21,\n"};
  std::string S3{"  17,,,  18, 188\n"};

  /* Check that Split can split each string using the default ',' delimeter. */
  std::vector<std::string> Sub_S1;
  std::vector<std::string> Sub_S2;
  std::vector<std::string> Sub_S3;

  Sub_S1 = IO::Read::Split(S1);
  Sub_S2 = IO::Read::Split(S2);
  Sub_S3 = IO::Read::Split(S3);

  /* Print results */
  unsigned len_Sub_S1 = Sub_S1.size();
  unsigned len_Sub_S2 = Sub_S2.size();
  unsigned len_Sub_S3 = Sub_S3.size();

  for(unsigned i = 0; i < len_Sub_S1; i++) { std::cout << '\"' << Sub_S1[i] << '\"' << std::endl; }
  for(unsigned i = 0; i < len_Sub_S2; i++) { std::cout << '\"' << Sub_S2[i] << '\"' << std::endl; }
  for(unsigned i = 0; i < len_Sub_S3; i++) { std::cout << '\"' << Sub_S3[i] << '\"' << std::endl; }
} // void Test::Split(void) {
