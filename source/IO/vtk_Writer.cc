#if !defined(VTK_WRITER_SOURCE)
#define VTK_WRITER_SOURCE

#include "vtk_Writer.h"

void IO::Write::vtk(const Node* Nodes, const unsigned Num_Nodes, const Element* Elements, const unsigned Num_Elements) {
  /* Function description:
  This function prints Node and Element data to a .vtk file that can be read and
  used by paraview. */

  /* First, create the file to be printed to. */
  std::string File_Path = "./IO/Out.vtk";
  std::ofstream File{};
  File.open(File_Path.c_str());

  /* Now print the header to the file. */
  vtk_header(File);

  /* Now print the points (Node positions) to the file */
  vtk_points(File, Nodes, Num_Nodes);

  /* Now print the cells (elements) to the file */
  vtk_elements(File, Elements, Num_Elements);

  /* All done. We can now close the file. */
  File.close();
} // void IO::Write::vtk(const Node* Nodes, const unsigned Num_Nodes, const Element* Elements, const unsigned Num_Elements) {



void IO::Write::vtk_header(std::ofstream & File) {
  /* Function description:
  This function is used to print the VTK file header to the passed ofstream
  object. */

  File << "# vtk DataFile Version 3.0\n";
  File << "FEM output file\n";
  File << "ASCII\n";
  File << "DATASET UNSTRUCTURED_GRID\n";
} // void IO::Write::vtk_header(std::ofstream & File) {



void IO::Write::vtk_points(std::ofstream & File, const Node* Nodes, const unsigned Num_Nodes) {
  /* Function description:
  This function prints point data (Node positions) to the File. */

  /* First, print the points header. */
  File << "POINTS " << Num_Nodes << " double\n";

  /* Next, print the individual points to the file. */
  for(unsigned i = 0; i < Num_Nodes; i++) {
    double xi = Nodes[i].Get_Position_Component(0) + Nodes[i].Get_Displacement_Component(0);
    double yi = Nodes[i].Get_Position_Component(1) + Nodes[i].Get_Displacement_Component(1);
    double zi = Nodes[i].Get_Position_Component(2) + Nodes[i].Get_Displacement_Component(2);

    File << xi << " " << yi << " " << zi << "\n";
  } // for(unsigned i = 0; i < Num_Nodes; i++) {
} // void IO::Write::vtk_points(std::ofstream & File, const Node* Nodes, const unsigned Num_Nodes) {



void IO::Write::vtk_elements(std::ofstream & File, const Element* Elements, const unsigned Num_Elements) {
  /* Function description:
  This function prints cell data (elements) to the File. */

  /* First print the CELLS statement. Here, the size parameter is simply 9 times
  the number of elements. */
  File << "CELLS " << Num_Elements << " " << 9*Num_Elements << "\n";

  /* Next, cycle through the Elements, printing each one's node list to the
  file. */
  for(unsigned i = 0; i < Num_Elements; i++) {
    File << "8";
    for(unsigned j = 0; j < 8; j++) { File << " " << Elements[i].Get_Node_ID(j); }
    File << "\n";
  } // for(unsigned i = 0; i < Num_Elements; i++) {

  /* Now print the Cell_Type statement */
  File << "CELL_TYPES " << Num_Elements << "\n";

  /* Now print the cell types. In our case, every cell is a hexahedral element,
  which corresponds to cell type 12. */
  for(unsigned i = 0; i < Num_Elements; i++) { File << "12\n"; }
} // void IO::Write::vtk_elements(std::ofstream & File, const Element* Elements, const unsigned Num_Elements) {

#endif
