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

  /* First print the CELLS statement. To do that, we need to know how many Wedge
  and Brick elements are in the Elements array.

  Further, the code below accesses the element types several times. In order to
  reduce redundant checks, we write down the element types into an Array as
  we determine the number of brick and wedge elements. This should (in theory)
  improve performance. */
  Element_Types Element_Type_Array[Num_Elements];
  unsigned Num_Brick = 0;
  unsigned Num_Wedge = 0;

  for(unsigned i = 0; i < Num_Elements; i++) {
    if(Elements[i].Get_Element_Type() == Element_Types::BRICK) {
      Num_Brick++;
      Element_Type_Array[i] = Element_Types::BRICK;
    } // if(Elements[i].Get_Element_Type() == Element_Types::BRICK) {
    else { // if(Elements[i].Get_Element_Type() == Element_Types::WEDGE) {
      Num_Wedge++;
      Element_Type_Array[i] = Element_Types::WEDGE;
    } // else {
  } // for(unsigned i = 0; i < Num_Elements; i++) {

  unsigned n = (8+1)*Num_Brick + (6+1)*Num_Wedge;

  File << "CELLS " << Num_Elements << " " << n << "\n";

  /* Next, cycle through the Elements, printing each one's node list to the
  file. */
  for(unsigned i = 0; i < Num_Elements; i++) {
    if(Element_Type_Array[i] == Element_Types::BRICK) {
      File << "8";
      for(unsigned j = 0; j < 8; j++) { File << " " << Elements[i].Get_Node_ID(j); }
      File << "\n";
    } // if(Element_Type_Array[i] == Element_Types::BRICK) {
    else { // (Element_Type_Array[i] == Element_Types::WEDGE)
      File << "6";

      /* Nodes 2, 3 and 6, 7 are identical for wedge elements. Thus,
      we only want to print nodes 0, 1, 2 and 4, 5, 6 to file */
      for(unsigned j = 0; j < 3; j++) { File << " " << Elements[i].Get_Node_ID(j); }
      for(unsigned j = 4; j < 7; j++) { File << " " << Elements[i].Get_Node_ID(j); }
      File << "\n";
    } // else {
  } // for(unsigned i = 0; i < Num_Elements; i++) {

  /* Now print the Cell_Type statement */
  File << "CELL_TYPES " << Num_Elements << "\n";

  /* Now print the cell types. In our case, every cell is either a hexahedral
  or wedge element. hexahedral elements correspond to id 12. Wedge elements
  correspond to id 13 */
  for(unsigned i = 0; i < Num_Elements; i++) {
    if(Element_Type_Array[i] == Element_Types::BRICK) { File << "12\n"; }
    else { File << "13\n"; }
  } // for(unsigned i = 0; i < Num_Elements; i++) {
} // void IO::Write::vtk_elements(std::ofstream & File, const Element* Elements, const unsigned Num_Elements) {

#endif
