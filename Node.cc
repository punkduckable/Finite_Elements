#if !defined(NODE_SOURCE)
#define NODE_SOURCE

//////////////////////////////////////////////////////////////////////////////
// Setter methods

void Node::Update_Position(const Array_3<double> New_Position) {
  Current_Position = New_Position;
} // void Node::Update_Position(const Array_3<double> New_Position) {



//////////////////////////////////////////////////////////////////////////////
// Getter methods

Array_3<double> Node::Get_Original_Position(void) const {
  return Original_Position;
} // Array_3<double> Node::Get_Original_Position(void) const {



Array_3<double> Node::Get_Current_Position(void) const {
  return Current_Position;
} // Array_3<double> Node::Get_Current_Position(void) const {



#endif
