#if !defined(SIMULATION_HEADER)
#define SIMULATION_HEADER

#include <string>
#include <stdio.h>
#include <list>

#include "Errors.h"
#include "Array.h"
#include "Matrix.h"
#include "Node/Node.h"
#include "Element/Element.h"
#include "IO/inp_reader.h"

#define ID_MONITOR

using std::list;

namespace Simulation {
  double E = 10;                        // Young's modulus                     : Units GPA
  double v = .3;                        // Poisson's ratio                     : Unitless

  void From_File(const std::string & File_Name);                               // Intent: Read

  class Node* Process_Node_Lists(class list<Array<double,3>> & Node_Positions,           // Intent: Read/Write
                                 class list<IO::Read::inp_boundary_data> & Boundary_List,// Intent: Read/Write
                                 const unsigned Num_Nodes);                              // Intent: Read

  unsigned SetUp_ID_Num_Global_Eq(class Matrix<int> & ID,                      // Intent: Write
                                  const Node * Nodes,                          // Intent: Read
                                  const unsigned Num_Nodes);                   // Intent: Read

  class Element* Process_Element_List(class list<Array<unsigned, 8>> & Element_Node_Lists,    // Intent: Read/Write
                                      const unsigned Num_Elements);                           // Intent: Read
} // namespace Simulation {

#endif
