#if !defined(INP_READER_HEADER)
#define INP_READER_HEADER

#include "../Errors.h"
#include <string.h>
#include <fstream>
#include <list>
#include <array>
#include <string.h>
#include <stdio.h>

namespace IO {
  namespace Read {
    void inp(const std::string & File_Name,                                    // Intent: Read
             class std::list<std::array<double,3>> & Node_Positions,           // Intent: Write
             class std::list<std::array<unsigned,8>> & Element_Node_Lists);    // Intent: Write
  } // namespace Read {
} // namespace IO {

#endif
