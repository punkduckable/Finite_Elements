#if !defined(INP_READER_HEADER)
#define INP_READER_HEADER

#include "Errors.h"
#include "Element/Element.h"                     // For Element_Types type
#include "Array.h"
#include <string.h>
#include <fstream>
#include <list>
#include <vector>
#include <stdio.h>

namespace IO {
  namespace Read {
    /* Structure to hold boundary information (this is used by the inp reader
    to read in boundary information from inp files). */
    struct inp_boundary_data {
      unsigned Node_Number;
      unsigned Start_DOF;
      unsigned End_DOF;
      double displacement;
    }; // struct inp_boundary_data {

    void inp(const std::string & File_Name,                                    // Intent: Read
             class std::list<Array<double,3>> & Node_Positions,                // Intent: Write
             class std::list<Array<unsigned,8>> & Element_Node_Lists,          // Intent: Write
             class std::list<inp_boundary_data> & Boundary_List);              // Intent: Write

    void node_set(const std::string & File_Name,                               // Intent: Read
                  class std::list<unsigned> & Node_Set_List,                   // Intent: Read
                  const std::string & Node_Set_Name = std::string("\0"));      // Intent: Read

    bool Contains(const char* Buffer,                                          // Intent: Read
                  const char* Word,                                            // Intent: Read
                  unsigned Search_At = 0);                                     // Intent: Read

    std::vector<std::string> Split(std::string & S,                            // Intent: Read
                                   const char Delim = ',');                    // Intent: Read
  } // namespace Read {
} // namespace IO {

#endif
