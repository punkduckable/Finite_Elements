#if !defined(INP_READER_SOURCE)
#define INP_READER_SOURCE

#include "inp_Reader.h"

void IO::Read::inp(const std::string & File_Name, class std::list<Array<double, 3>> & Node_Positions, class std::list<Array<unsigned,8>> & Element_Node_Lists, class std::list<inp_boundary_data> & Boundary_List) {
  /* Function description:
  This function is designed to read in node positions, node boundary data,
  and element connectivity from an .inp file. This information is turn returned
  through the Node_Positions, Element_Node_Lists, and Boundary_List lists. The
  requested file should be in the IO directory (Note: this is not source/IO). */

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
  // Read in Node, Element, and Boundary data.

  char buffer[256];                              // buffer to hold data read in from File
  File.getline(buffer, 256);                     // Read up to 256 characters (or end of line)

  /* Read in all nodes positions, element connectivity, and boundary information
  from the file  */
  while(File.eof() == false && File.fail() == false) {
    /* Check if line begins with a *. If so, we may be at the start of the
    Node (*Node), Element (*Elemnet), or Boundary (*Boundar) listing. */

    if(buffer[0] == '*') {
      /* Check if current line starts with "*Node" */
      if(String_Ops::Contains(buffer, "*Node") ) {

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
      } // if(String_Ops::Contains(bufffer, "*Node") {


      /* Check if current line starts with "*Element" */
      if( String_Ops::Contains(buffer, "*Element") ) {

        /* If so then we have found the start of the element node listings.
        Before we can read the elements in, we need to identify which type of
        element we're dealing with */
        Element_Types Type;
        if( String_Ops::Contains(buffer, "type=C3D8", 8) ) { Type = Element_Types::BRICK; }
        else { Type = Element_Types::WEDGE; } // if( String_Ops::Contains(buffer, "type=C3D6", 8) )

        while(File.eof() == false && File.fail() == false) {
          File.getline(buffer, 256);

          // Check if we have reached the end of the element listings.
          if(buffer[0] ==  '*') { break; }

          /* Otherwise, read in element node lists from the buffer and push it
          onto the Element_Node_Lists list. */
          Array<unsigned,8> Node_List;      // Hold the current element position

          if(Type == Element_Types::BRICK) {
            sscanf(buffer,
                   "%*d, %u, %u, %u, %u, %u, %u, %u, %u",
                   &Node_List[0], &Node_List[1], &Node_List[2], &Node_List[3], &Node_List[4], &Node_List[5], &Node_List[6], &Node_List[7]);
          } // if(Type == Element_Types::BRICK) {
          else { // if(Type == Element_Types::WEDGE)
            sscanf(buffer,
                   "%*d, %u, %u, %u, %u, %u, %u",
                   &Node_List[0], &Node_List[1], &Node_List[2], &Node_List[4], &Node_List[5], &Node_List[6]);

             // Nodes 2 and 3, as well as 6 and 7 are identical
             Node_List[3] = Node_List[2];
             Node_List[7] = Node_List[6];
          } // else {

          /* Convert from 1 index to 0 index */
          for(unsigned i = 0; i < 8; i++) { Node_List[i]--; }

          Element_Node_Lists.push_back(Node_List);
        } // while(File.eof() == false && File.fail() == false) {

        /* If we're here then we've finished reading in the elements. We may have
        found the start of the node listing, so we don't want to read in the next
        line yet. Thus, continue onto the next iteration (skipping the "read in
        next line" instruction at the end of the while loop) */
        continue;
      }  // if( String_Ops::Contains(buffer, "*Element") ) {


      /* Check if current line starts with "*Boundary" */
      if( String_Ops::Contains(buffer, "*Boundary") ) {

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
      } // if( String_Ops::Contains(buffer, "*Boundary") ) {
    } // if(buffer[0] == '*') {

    File.getline(buffer, 256);                         // Read in next line (or up to 256 characters)
  } // while(File.eof() == false && File.fail() == false) {


  //////////////////////////////////////////////////////////////////////////////
  // All done! Close the file.
  File.close();
} // void IO::Read::inp(const std::string & File_Name, class std::list<Array<double, 3>> & Node_Positions...



void IO::Read::node_set(const std::string & File_Name, class std::list<unsigned> & Node_Set_List, const std::string & Node_Set_Name) {
  /* Function description:
  This function is designed to read in a node set from the specified file.
  The defaulted "Node_Set_Name" argument can be used to specify which node set
  you want to read in. The requested file should be in the IO directory (Note:
  this is not source/IO).

  If a Node_Set_Name is explicitly passed then this function will search for a
  node set with the specified name in the specified file. If found, all node
  ID's in that node set will be appended to the Node_Set_List. If the requested
  node set can't be found then nothing will be appened to Node_Set_List.

  If no Node_Set_Name is specified, then the function will append the contents
  of every node set that it finds in File_Name to the Node_Set_List. */

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
            "Can't Open File Exception: Thrown by IO::Read::node_set\n"
            "You tried to open the file %s (%s.inp).\n"
            "However, no such file could be found in the IO directory.\n",
            File_Name.c_str(), File_Name.c_str());
    throw Cant_Open_File(Error_Message_Buffer);
  } // if(File.is_open() == false) {


  //////////////////////////////////////////////////////////////////////////////
  // Read in Node Set data.

  char buffer[256];                              // buffer to hold data read in from File
  File.getline(buffer, 256);                     // Read up to 256 characters (or end of line)

  /* First, check if the user passed a Node_Set_Name. If not, then Node_Set_Name
  will be set to the character '\0'. */
  bool Passed_Node_Set_Name;
  if( Node_Set_Name.c_str()[0] == '\0') { Passed_Node_Set_Name = false; }
  else { Passed_Node_Set_Name = true; }

  /* Read in the requested Node Set.  */
  while(File.eof() == false && File.fail() == false) {
    /* Check if the current line contains "*Nset" */
    if( String_Ops::Contains(buffer, "*Nset") ) {
      /* If the user passed a Node_Set_Name then check if the current Node Set's
      name matches it. */
      if( Passed_Node_Set_Name == true) {
        /* in general, the node set's name may match something in the current
        line other than the node set name. Thus, insead of just searching for
        the Node_Set_Name, we search for "nset=Node_Set_Name". */
        std::string Name_with_nset = "nset=" + Node_Set_Name;

        /* Now, check for a match */
        if( String_Ops::Contains(buffer, Name_with_nset.c_str() ) ) {}
        else { continue; }
      } // if( Passed_Node_Set_Name == true) {

      /* In inp files, node sets can be formatted in one of two ways:
      generate form or list form.


      Generate:
      In generate form, the node set consists of a sequence of lines, each one
      of which is formatted as follows:
               N_start, N_end, Inc
      This states that node number N_start and every Inc'th node after that
      whose node number is less than or equal to N_End belongs to the set. In
      other words, start by adding node N_start, repeatedly increment by Inc
      and add the resulting node number to the set. Continue this until you
      reach a node whose number is greater than N_end.

      Node sets that use generate form will have the keyword "generate"
      in the node set header (the line that starts with *Nset).


      List:
      In list form, the nodes that belong to the node set are simply listed
      in a comma separated list. Any node set whose header does not contain the
      "generate" keyword is a list node set.


      Because of the difference, we need to check if we're reading in a
      generator or list type node set */
      if( String_Ops::Contains(buffer, "generate") ) {
        /* Read in successive lines until we reach the end of the node set. */
        while(File.eof() == false && File.fail() == false) {
          File.getline(buffer, 256);

          /* Check if the current line begins with a *. If so, then we've reached
          the end of the current node set section. */
          if(strcmp(&buffer[0], "*") == 0) { break; }

          unsigned N_Start, N_End, Inc;
          sscanf(buffer, "%u, %u, %u\n", &N_Start, &N_End, &Inc);

          /* Convert from 1 index to 0 index */
          N_Start--;
          N_End--;

          for(unsigned i = N_Start; i <= N_End; i += Inc) { Node_Set_List.push_back(i); }
        } // while(File.eof() == false && File.fail() == false) {
      } // if( String_Ops::Contains(buffer, "generate") ) {
      else { // list node set
        /* Finish me! */
      } // else {

      /* If we're here then we've finished reading in the node set. We may have
      found the start of another type of listing. Thus, we don't want to read
      in the next line yet. We therefore continue onto the next iteration
      (skipping the "read in next line" instruction at the end of the while
      loop) */
      continue;
    } // if( String_Ops::Contains(buffer, "*Nset") ) {

    File.getline(buffer, 256);                         // Read in next line (or up to 256 characters)
  } // while(File.eof() == false && File.fail() == false) {


  //////////////////////////////////////////////////////////////////////////////
  // All done! Close the file.
  File.close();
} // void IO::Read::node_set(const std::string & File_Name, class std::list<unsigned> & Node_Set_List, const std::string & Node_Set_Name) {

#endif
