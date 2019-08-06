#if !defined(KFX_WRITER_HEADER)
#define KFX_WRITER_HEADER

#include <stdio.h>
#include "Errors.h"
#include "Matrix.h"

namespace IO {
  namespace Write {
    enum class Printing_Mode{INTEGER, EXP};

    void K_To_File(const Matrix<double>& K,                                    // Intent: Read
                   const Printing_Mode Mode = Printing_Mode::EXP);             // Intent: Read
    void F_To_File(const double* F,                                            // Intent: Read
                   const unsigned Num_Global_Eq);                              // Intent: Read
    void x_To_File(const double* x,                                            // Intent: Read
                   const unsigned Num_Global_Eq);                              // Intent: Read
  } // namespace Write {
} // namespace IO {

#endif
