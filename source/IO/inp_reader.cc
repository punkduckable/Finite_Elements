#if !defined(INP_READER_SOURCE)
#define INP_READER_SOURCE

#include "inp_reader.h"

void IO::Read::inp(const std::string & File_Name, class std::list<std::array<double, 3>> & Node_Positions, class std::list<std::array<unsigned,8>> & Element_Node_Lists) {
  /* File description:
  This function is designed to read in node positions and element connectivity
  from an .inp file. This information is turn returned through the Node_Positions
  and Element_Node_Lists Lists. */

  //////////////////////////////////////////////////////////////////////////////
  /* First, we need to open the file. To do this, we first need to get the file
  path */
  std::string File_Path = "../../IO/" + File_Name + ".feb";
  std::ifstream File{};
  File.open(File_Path.c_str());

  /* Check if the file could be opened. If not then throw an exception */
  if(File.is_open() == false) {
    char Error_Message_Buffer[500];
    sprintf(Error_Message_Buffer,
            "File Not Found Exception: Thrown by IO::Read::inp\n"
            "You tried to open the file %s (%s.inp).\n"
            "However, no such file could be found in the IO directory.\n",
            File_Name.c_str(), File_Name.c_str());
    throw File_Not_Found(Error_Message_Buffer);
  } // if(File.is_open() == false) {


  //////////////////////////////////////////////////////////////////////////////
  // Read in Node and Element data.

  char buffer[256];                             // buffer to hold data read in from File
  bool Nodes_Read = false;                       // Flag to indicate that we've read in node data
  bool Elements_Read = false;                    // Flag to indicate that we've read in element data

  File.get(buffer, 256);                         // Read up to 256 characters (or end of line)

  while(File.eof() == false && File.fail() == false && (Nodes_Read == false || Elements_Read == false)) {
    /* Check if the line begins with a *. If so, we may be at the start of the
    Node (*Node) or Element (*Elemnet) listing. */
    if(strcmp(buffer + 0, "*")) {
      /* Check if current line reads "*Node," */
      if( strcmp(buffer + 1, "N") &&
          strcmp(buffer + 2, "o") &&
          strcmp(buffer + 3, "d") &&
          strcmp(buffer + 4, "e") &&
          strcmp(buffer + 5, ",")) {

        /* If so then we have found the start of the node listing. Begin
        reading them in. */
        while(File.eof() == false && File.fail() == false) {
          File.get(buffer, 256);

          // Check if we have reached the end of the node listings.
          if(strcmp(buffer + 0, "*")) { break; }

          /* Otherwise, read in node position from the buffer and push it onto
          the Node_Positions list. */
          int n;                                 // Throw away to hold the node number.
          std::array<double,3> Position;         // Hold the current node position
          sscanf(buffer,
                 "%d,%lf,%lf,%lf",
                 &n, &Position[0], &Position[1], &Position[2]);
          Node_Positions.push_back(Position);
        } // while(File.eof() == false && File.fail() == false) {

        /* If we're here then we've finished reading in the nodes. We may have
        found the start of the element listing, so we don't want to read in the next
        line yet. Thus, continue onto the next iteration (skipping the "read in
        next line" instruction at the end of the while loop) */
        Nodes_Read = true;
        continue;
      } // if( strcmp(buffer + 1, "N") &&


      /* Check if current line reads "*Element" */
      if( strcmp(buffer + 1, "E") &&
          strcmp(buffer + 2, "l") &&
          strcmp(buffer + 3, "e") &&
          strcmp(buffer + 4, "m") &&
          strcmp(buffer + 5, "e") &&
          strcmp(buffer + 6, "n") &&
          strcmp(buffer + 7, "t") &&
          strcmp(buffer + 8, ",")) {

        /* If so then we have found the start of the element node listings. Begin
        reading them in. */
        while(File.eof() == false && File.fail() == false) {
          File.get(buffer, 256);

          // Check if we have reached the end of the element listings.
          if(strcmp(buffer + 0, "*")) { break; }

          /* Otherwise, read in element node lists from the buffer and push it
          onto the Element_Node_Lists list. */
          int n;                                 // Throw away to hold the element number.
          std::array<unsigned,8> Node_List;      // Hold the current element position
          sscanf(buffer,
                 "%d,%u,%u,%u,%u,%u,%u,%u,%u",
                 &n, &Node_List[0], &Node_List[1], &Node_List[2], &Node_List[3], &Node_List[4], &Node_List[5], &Node_List[6], &Node_List[7]);
          Element_Node_Lists.push_back(Node_List);
        } // while(File.eof() == false && File.fail() == false) {

        /* If we're here then we've finished reading in the elements. We may have
        found the start of the node listing, so we don't want to read in the next
        line yet. Thus, continue onto the next iteration (skipping the "read in
        next line" instruction at the end of the while loop) */
        Elements_Read = true;
        continue;
      }  // if( strcmp(buffer + 1, "E") &&
    } // if(strcmp(buffer + 0, "*")) {

    File.get(buffer, 256);                         // Read in next line (or up to 256 characters)
  } // while(File.eof() == false && File.fail() == false && (Nodes_Read == false || Elements_Read == false)) {


  //////////////////////////////////////////////////////////////////////////////
  // All done! Close the file.
  File.close();
} // void IO::Read::inp(const std::string & File_Name, class std::list<double[3]> & Node_Positions...


#endif
