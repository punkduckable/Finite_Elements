#if !defined(INP_READER_HEADER)
#define INP_READER_HEADER

#include "Errors.h"
#include "Element/Element.h"                     // For Element_Types type
#include "IO/String_Ops.h"
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

    /* Class to set boundary conditions for a node set. */
    class nset_BC {
      private:
        bool x_BC_set;
        bool y_BC_set;
        bool z_BC_set;

        double x_BC;
        double y_BC;
        double z_BC;
      public:
        nset_BC() : x_BC_set(false), y_BC_set(false), z_BC_set(false) {}

        void Set_x_BC(double x_BC_In) { (*this).x_BC_set = true; (*this).x_BC = x_BC_In; }
        void Set_y_BC(double y_BC_In) { (*this).y_BC_set = true; (*this).y_BC = y_BC_In; }
        void Set_z_BC(double z_BC_In) { (*this).z_BC_set = true; (*this).z_BC = z_BC_In; }

        bool Has_x_BC(void) const { return (*this).x_BC_set; }
        bool Has_y_BC(void) const { return (*this).y_BC_set; }
        bool Has_z_BC(void) const { return (*this).z_BC_set; }

        double Get_x_BC(void) const { return (*this).x_BC; }
        double Get_y_BC(void) const { return (*this).y_BC; }
        double Get_z_BC(void) const { return (*this).z_BC; }
    }; // class nset_BC {

    void inp(const std::string & File_Name,                                    // Intent: Read
             class std::list<Array<double,3>> & Node_Positions,                // Intent: Write
             class std::list<Array<unsigned,8>> & Element_Node_Lists,          // Intent: Write
             class std::list<inp_boundary_data> & Boundary_List);              // Intent: Write

    void node_set(const std::string & File_Name,                               // Intent: Read
                  class std::list<unsigned> & Node_Set_List,                   // Intent: Read
                  const std::string & Node_Set_Name = std::string("\0"));      // Intent: Read
  } // namespace Read {
} // namespace IO {

#endif
