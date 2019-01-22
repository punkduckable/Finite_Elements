#if !defined(NODE_HEADER)
#define NODE_HEADER

class Node {
// Elements need to be able to access the private Node members.
friend class Element;

private:
  const Array_3<double> Original_Position;  // Material position of the node    Units : M
  Array_3<double> Current_Position;         // Spatial position of the node     Units : M
  const Array_3<bool> Has_BC;               // Has_BC[i] is true if the ith component of this node has a prescribed displacement BC
  const Array_3<double> BC;                 // The Prescribed displacement BC's

public:
  //////////////////////////////////////////////////////////////////////////////
  // Constructors, Destructor

  /* Constructor to explicitly set the Node's Original Position, and BC's. Note,
  if no array is passed for Has_BC_In or BC_In then we will assume that this
  node has no prescribed displacement */
  Node(const Array_3<double> Original_Position_In, const Array_3<bool> Has_BC_In, const Array_3<double> BC_In)
       : Original_Position(Original_Position_In), Has_BC(Has_BC_In), BC(BC_In) {

    /* Set up the current position (the other private members are set up with
    the member initialization list). Basically, if there's a prescribed
    displacement then the current position is specified by that. Otherwise,
    the original position is the current position. */
    for(int i = 0; i < 3; i++)
      if(Has_BC[i] == true)
        Current_Position[i] = BC[i];
      else
        Current_Position[i] = Original_Position[i];
  } // Node(const Array_3<double> Original_Position_In, const Array_3<bool> Has_BC_In, const Array_3<bool> BC_In)...


  // Do nothing destructor.
  ~Node(void) {};



  //////////////////////////////////////////////////////////////////////////////
  // Setter methods

  // Updates the current (spatial) position of the node
  void Update_Position(const Array_3<double> New_Position);



  //////////////////////////////////////////////////////////////////////////////
  // Getter methods

  Array_3<double> Get_Original_Position(void) const;
  Array_3<double> Get_Current_Position(void) const;
}; // class Node {

#endif
