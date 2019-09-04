#if !defined(INP_READER_SOURCE)
#define INP_READER_SOURCE

#include "inp_Reader.h"


bool IO::Read::Contains(const char* Buffer, const char* Word, unsigned Start_At) {
  /* Function description:
  This function determines if Word is contained in Buffer. The optional Start_At
  argument can be used to only search through part of the string. If, for
  example, Start_At = 5, then this function will only search for matches that
  begin at index 5 (or later) of Buffer.

  My inp reader frequently checks if a particular word is in a string. Thus, I
  wrote this function to automate that process. */

  /* Assumptions:
  This function assumes that both Buffer and Word are NULL TERMINATED strings.
  That is, I assume that both end with the \0 character. */

  /* First, check if Buffer has fewer than Start_At characters (which happens
  if there is a \0 in a index whose value is less than Start_At). If not, return
  false */
  for(unsigned i = 0; i < Start_At; i++) {
    if(Buffer[i] == '\0') { return false; }
  } // for(unsigned i = 0; i < Start_At; i++) {

  // Loop through the characters of Buffer.
  unsigned i = Start_At;
  while(Buffer[i] != '\0') {
    // At each one, see if Word starts at that character.
    unsigned j = 0;
    while(Buffer[i+j] == Word[j]) {
      j++;

      /* If we're still in here and we've reached the end of "Word" then
      we've found a match! */
      if(Word[j] == '\0') { return true; }

      /* If we haven't reached the end of Word but we have reached the end of
      Buffer then Buffer does not contain Word. */
      if(Buffer[i+j] == '\0') { return false; }
    } // while(Buffer[i+j] == Word[j]) {

    i++;
  } // while(Buffer[i] != '\0') {

  /* If we get here then we cycled through Buffer without finding a match.
  Thus, buffer does not contain Word. */
  return false;
} // bool IO::Read::Contains(const char* Buffer, const char* Word, unsigned Start_At) {


void IO::Read::inp(const std::string & File_Name, class std::list<Array<double, 3>> & Node_Positions, class std::list<Array<unsigned,8>> & Element_Node_Lists, class std::list<inp_boundary_data> & Boundary_List, class std::list<unsigned> & Node_Set_List) {
  /* File description:
  This function is designed to read in node positions, node boundary data,
  and element connectivity from an .inp file. This information is turn returned
  through the Node_Positions, Element_Node_Lists, Boundary_List, and
  Node_Set_List Lists. */

  //////////////////////////////////////////////////////////////////////////////
  /* First, we need to open the file. To do this, we first need to get the file
  path */
  std::string File_Path = "./IO/" + File_Name;
  std::ifstream File{};
  File.open(File_Path.c_str());

  /* Check if the file could be opened. If not then throw an exception */
  if(File.is_open() == false) {
    char Error_Message_Buffer[500];
    sprintf(Error_Message_Buffer,
            "Can't Open File Exception: Thrown by IO::Read::inp\n"
            "You tried to open the file %s (%s.inp).\n"
            "However, no such file could be found in the IO directory.\n",
            File_Name.c_str(), File_Name.c_str());
    throw Cant_Open_File(Error_Message_Buffer);
  } // if(File.is_open() == false) {


  //////////////////////////////////////////////////////////////////////////////
  // Read in Node and Element data.

  char buffer[256];                              // buffer to hold data read in from File
  File.getline(buffer, 256);                         // Read up to 256 characters (or end of line)

  /* Read in all nodes positions, element connectivity, and boundary information
  from the file  */
  while(File.eof() == false && File.fail() == false) {
    /* Check if line begins with a *. If so, we may be at the start of the
    Node (*Node), Element (*Elemnet), or Boundary (*Boundar) listing. */

    if(buffer[0] == '*') {
      /* Check if current line starts with "*Node" */
      if(Contains(buffer, "*Node") ) {

        /* If so then we have found the start of the node listing. Begin
        reading them in. */
        while(File.eof() == false && File.fail() == false) {
          File.getline(buffer, 256);

          // Check if we have reached the end of the node listings.
          if(buffer[0] == '*') { break; }

          /* Otherwise, read in node position from the buffer and push it onto
          the Node_Positions list. */
          Array<double,3> Position;         // Hold the current node position
          sscanf(buffer,
                 "%*d, %lf, %lf, %lf",
                 &Position[0], &Position[1], &Position[2]);
          Node_Positions.push_back(Position);
        } // while(File.eof() == false && File.fail() == false) {

        /* If we're here then we've finished reading in the nodes. We may have
        found the start of the element listing, so we don't want to read in the next
        line yet. Thus, continue onto the next iteration (skipping the "read in
        next line" instruction at the end of the while loop) */
        continue;
      } // if(Contains(bufffer, "*Node") {


      /* Check if current line starts with "*Element" */
      if( Contains(buffer, "*Element") ) {

        /* If so then we have found the start of the element node listings. Begin
        reading them in. */
        while(File.eof() == false && File.fail() == false) {
          File.getline(buffer, 256);

          // Check if we have reached the end of the element listings.
          if(buffer[0] ==  '*') { break; }

          /* Otherwise, read in element node lists from the buffer and push it
          onto the Element_Node_Lists list. */
          Array<unsigned,8> Node_List;      // Hold the current element position
          sscanf(buffer,
                 "%*d, %u, %u, %u, %u, %u, %u, %u, %u",
                 &Node_List[0], &Node_List[1], &Node_List[2], &Node_List[3], &Node_List[4], &Node_List[5], &Node_List[6], &Node_List[7]);

          /* Convert from 1 index to 0 index */
          for(unsigned i = 0; i < 8; i++) { Node_List[i]--; }

          Element_Node_Lists.push_back(Node_List);
        } // while(File.eof() == false && File.fail() == false) {

        /* If we're here then we've finished reading in the elements. We may have
        found the start of the node listing, so we don't want to read in the next
        line yet. Thus, continue onto the next iteration (skipping the "read in
        next line" instruction at the end of the while loop) */
        continue;
      }  // if( Contains(buffer, "*Element") ) {


      /* Check if current line starts with "*Boundary" */
      if( Contains(buffer, "*Boundary") ) {

        /* If so then we have found a boundary section. Let's read in the Boundary
        conditions. */
        while(File.eof() == false && File.fail() == false) {
          File.getline(buffer, 256);

          /* Check if the current line begins with a *. If so, then we've reached
          the end of the boundary section. */
          if(buffer[0] == '*') { break; }

          /* Otherwise read in the boundary information. */
          inp_boundary_data Boundary_Data;
          sscanf(buffer,
                 "%*s,%d,%d,%d,%lf\n",
                 &Boundary_Data.Node_Number, &Boundary_Data.Start_DOF, &Boundary_Data.End_DOF, &Boundary_Data.displacement);

          /* Convert from 1 index to 0 index */
          Boundary_Data.Node_Number--;

          /* Now, push the boundary data onto the Boundary_Data list. */
          Boundary_List.push_back(Boundary_Data);
        } // while(File.eof() == false && File.fail() == false) {

        /* If we're here then we've finished reading in the boundary. We may have
        found the start of another boundary listing, a node listing, or element
        listing. Thus, we don't want to read in the next line yet.
        We therefore continue onto the next iteration (skipping the "read in
        next line" instruction at the end of the while loop) */
        continue;
      } // if( Contains(buffer, "*Boundary") ) {


      /* Check if the current line starts with "*Nset" */
      if( Contains(buffer, "*Nset") ) {

        /* If so then we have found a node set. Let's read it in and add the
        associated nodes to the Node_Set_List. */
        while(File.eof() == false && File.fail() == false) {
          File.getline(buffer, 256);

          /* Check if the current line begins with a *. If so, then we've reached
          the end of the boundary section. */
          if(strcmp(&buffer[0], "*") == 0) { break; }

          /* In inp files, a node set consists of a sequence of lines, each one of
          which explains which nodes belong to the specified set. Each line is
          formatted as follows:
                   N_start, N_end, Inc
          Which states that node number N_start and every Inc'th node after that
          has node number less than or equal to N_end belongs to the set. In
          other words, start by adding node N_start, repeatedly increment by Inc
          and add the resulting node number to the set. Continue this until you
          reach a node whose number is greater than N_end. */
          unsigned N_Start, N_End, Inc;
          sscanf(buffer, "%u, %u, %u\n", &N_Start, &N_End, &Inc);

          /* Convert from 1 index to 0 index */
          N_Start--;
          N_End--;

          for(unsigned i = N_Start; i <= N_End; i += Inc) { Node_Set_List.push_back(i); }
        } // while(File.eof() == false && File.fail() == false) {

        /* If we're here then we've finished reading in the node set. We may have
        found the start of another type of listing. Thus, we don't want to read
        in the next line yet. We therefore continue onto the next iteration
        (skipping the "read in next line" instruction at the end of the while
        loop) */
        continue;
      } // if( Contains(buffer, "*Nset") ) {
    } // if(buffer[0] == '*') {

    File.getline(buffer, 256);                         // Read in next line (or up to 256 characters)
  } // while(File.eof() == false && File.fail() == false) {


  //////////////////////////////////////////////////////////////////////////////
  // All done! Close the file.
  File.close();
} // void IO::Read::inp(const std::string & File_Name, class std::list<Array<double, 3>> & Node_Positions...



#endif
