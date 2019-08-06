#if !defined(KFX_WRITER_SOURCE)
#define KFX_WRITER_SOURCE

#include "KFX_Writer.h"

void IO::Write::K_To_File(const Matrix<double>& K, const Printing_Mode Mode) {
  // First, open a new file
  FILE* File = fopen("./IO/K.txt","w");

  /* Check if there was a problem opening the file. If so, throw an exception. */
  if(File == nullptr) {
    char Buffer[500];
    sprintf(Buffer,
            "Can't Open File Exception: Thrown by IO::Write::K_To_File\n"
            "For whatever reason, we couldn't open the K.txt file\n");
    throw Cant_Open_File(Buffer);
  } // if(File == nullptr) {

  // Get the number of Rows, Columns for K
  const unsigned Num_Rows = K.Get_Num_Rows();
  const unsigned Num_Cols = K.Get_Num_Cols();

  /* Note, this is not a very fast way to print K, since K is a column major
  matrix and we're printing it it in row major order. Oh well, this only runs
  once anyway */
  for(unsigned i = 0; i < Num_Rows; i++) {
    fprintf(File,"| ");

    for(unsigned j = 0; j < Num_Cols; j++) {
      if(Mode == Printing_Mode::INTEGER) { fprintf(File,"%1.0lf ", K(i,j)); }
      else { fprintf(File,"%8.1e ", K(i,j)); }
    } // for(unsigned j = 0; j < Num_Cols; j++) {

    fprintf(File,"|\n");
  } // for(unsigned i = 0; i < Num_Rows; i++) {

  // All done. Close the file
  fclose(File);
} // void IO::Write::K_To_File(const Matrix<double>& K, const Printing_Mode Mode) {



void IO::Write::F_To_File(const double* F, const unsigned Num_Global_Eq) {
  // First, open a new file.
  FILE* File = fopen("./IO/F.txt","w");

  /* Check if there was a problem opening the file. If so, throw an exception. */
  if(File == nullptr) {
    char Buffer[500];
    sprintf(Buffer,
            "Can't Open File Exception: Thrown by IO::Write::F_To_File\n"
            "For whatever reason, we couldn't open the F.txt file\n");
    throw Cant_Open_File(Buffer);
  } // if(File == nullptr) {

  // Print the contents of F to the file.
  for(unsigned i = 0; i < Num_Global_Eq; i++) { fprintf(File, "F[%3d] = %10.3e\n", i, F[i]); }

  // All done, close the file
  fclose(File);
} // void IO::Write::F_To_File(const double* F, const unsigned Num_Global_Eq) {



void IO::Write::x_To_File(const double* x, const unsigned Num_Global_Eq) {
  // First, open a new file
  FILE* File = fopen("./IO/X.txt","w");

  /* Check if there was a problem opening the file. If so, throw an exception. */
  if(File == nullptr) {
    char Buffer[500];
    sprintf(Buffer,
            "Can't Open File Exception: Thrown by IO::Write::X_To_File\n"
            "For whatever reason, we couldn't open the X.txt file\n");
    throw Cant_Open_File(Buffer);
  } // if(File == nullptr) {

  // Print the contents of X to the file
  for(unsigned i = 0; i < Num_Global_Eq; i++) { fprintf(File, "x[%3d] = %10.5lf\n", i, x[i]); }

  // All done, close the file
  fclose(File);
} // void IO::Write::x_To_File(const double* x, const unsigned Num_Global_Eq) {


#endif
