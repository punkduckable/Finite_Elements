#if !defined(STRING_OPERATIONS_HEADER)
#define STRING_OPERATIONS_HEADER

#include <string>
#include <vector>

namespace String_Ops {
  bool Contains(const char* Buffer,                                            // Intent: Read
                const char* Word,                                              // Intent: Read
                unsigned Search_At = 0);                                       // Intent: Read

  std::vector<std::string> Split(std::string & S,                              // Intent: Read
                                 const char Delim = ',');                      // Intent: Read
} // namespace String_Ops {

#endif
