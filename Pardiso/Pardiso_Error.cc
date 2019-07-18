#include "Pardiso.h"
#include <stdio.h>

void Report_Pardiso_Error(int Error_Code) {
  switch(Error_Code) {
    case 0:
      printf("Pardiso Error code %d:\n"
            "No problem here... what are you complaining about?\n", Error_Code);
      break;

    case -1:
      printf("Pardiso Error code %d:\n"
             "Invalid inputs. Something is wrong with A, IA, JA, or B.\n", Error_Code);
      break;

    case -2:
      printf("Pardiso Error code %d:\n"
             "Not enough memory! There is not enough memory for Pardiso to do it's job!\n", Error_Code);
      break;

    case -3:
      printf("Pardiso Error code %d:\n"
             "Reordering problem\n", Error_Code);
      break;

    case -4:
      printf("Pardiso Error code %d:\n"
             "Zero pivot, numerical factorization, or iterative refinement problem\n", Error_Code);
      break;

    case -5:
      printf("Pardiso Error code %d:\n"
             "Uknown error. Idk what you did\n", Error_Code);
      break;

    case -6:
      printf("Pardiso Error code %d:\n"
             "Preordering error.\n", Error_Code);
      break;

    case -7:
      printf("Pardiso Error code %d:\n"
             "Diagonal matrix problem\n", Error_Code);
      break;

    case -8:
      printf("Pardiso Error code %d:\n"
             "32-bit overflow problem\n", Error_Code);
      break;

    case -10:
      printf("Pardiso Error code %d:\n"
             "Your Pardiso license file is missing. pardiso.lic must be in the\n"
             "same directory as your executable!\n", Error_Code);
      break;

    case -11:
      printf("Pardiso Error code %d:\n"
             "Your license has expired! Get a new one, you bum.\n", Error_Code);
      break;

    case -12:
      printf("Pardiso Error code %d.\n"
             "Invalid username or hostname.", Error_Code);
      break;

    case -100:
      printf("Pardiso Error code %d.\n"
             "Reached maximum number of Krylov-subspace iterations in iterative solver\n", Error_Code);
      break;

    case -101:
      printf("Pardiso Error code %d.\n"
             "Krylov-subspace iterations did not sufficiently converge within 25 iterations\n", Error_Code);
      break;

    case -102:
      printf("Pardiso Error code %d.\n"
             "Error in Krylov-subspace iterations\n", Error_Code);
      break;

    case -103:
      printf("Pardiso Error code %d.\n"
             "Break-Down in Krylov-subspace iterations\n", Error_Code);
      break;

    default:
      printf("Pardiso Error code %d:\n"
             "This is an unknown Error code. I have no idea what happened...\n", Error_Code);
  } // switch(Error_Code) {
} // void Report_Pardiso_Error(int Error_Code) {
