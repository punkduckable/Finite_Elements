#if !defined(VTK_WRITER_HEADER)
#define VTK_WRITER_HEADER

#include <fstream>
#include <string.h>

#include "Errors.h"
#include "Node/Node.h"
#include "Element/Element.h"

namespace IO {
  namespace Write {
    void vtk(const Node* Nodes,                                                // Intent: Read
             const unsigned Num_Nodes,                                         // Intent: Read
             const Element* Elements,                                          // Intent: Read
             const unsigned Num_Elements);                                     // Intent: Read

    void vtk_header(std::ofstream & File);                                     // Intent: Write

    void vtk_points(std::ofstream & File,                                      // Intent: Write
                    const Node* Nodes,                                         // Intent: Read
                    const unsigned Num_Nodes);                                 // Intent: Read

    void vtk_elements(std::ofstream & File,                                    // Intent: Write
                      const Element* Elements,                                 // Intent: Read
                      const unsigned Num_Elements);                            // Intent: Read
  } // namespace Write {
} // namespace IO {

#endif
