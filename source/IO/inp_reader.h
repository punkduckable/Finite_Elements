#if !defined(INP_READER_HEADER)
#define INP_READER_HEADER

#include "../Errors.h"
#include "../Array.h"
#include <string.h>
#include <fstream>
#include <list>
#include <stdio.h>

namespace IO {
  namespace Read {
    void inp(const std::string & File_Name,                                    // Intent: Read
             class std::list<Array<double,3>> & Node_Positions,           // Intent: Write
             class std::list<Array<unsigned,8>> & Element_Node_Lists);    // Intent: Write
  } // namespace Read {
} // namespace IO {

#endif
