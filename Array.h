#if !defined(ARRAY_HEADER)
#define ARRAY_HEADER

template <typename Type>
class Array_3 {

friend class Node;
friend class Element;

private:
  // The actual array.
  Type Ar[3];


  /* Quicker access methods. These ones do not run an index check. This access
  method is therefore faster but less safe. Thus, it is a private method */

  // For reading an element of the array
  Type operator[](const unsigned int Index) const {
    return Ar[Index];
  } //   Type operator[](const unsigned int Index) const {

  // For writing to an element of the array
  Type & operator[](const unsigned int Index) {
    return Ar[Index];
  } // Type & operator[](const unsigned int Index) {


public:
  //////////////////////////////////////////////////////////////////////////////
  // Constructors, destructor

  // Default constructor
  Array_3(void) {}

  // Initialize using a 3 component array
  Array_3(const Type Ar_In[3]) {
    Ar[0] = Ar_In[0];
    Ar[1] = Ar_In[1];
    Ar[2] = Ar_In[2];
  } // Array_3(const Type Ar_In[3]) {

  // Initialize using a 3 component array
  Array_3(const Type a, const Type b, const Type c) {
    Ar[0] = a;
    Ar[1] = b;
    Ar[2] = c;
  } // Array_3(const Type Ar_In[3]) {


  // Destructor
  ~Array_3(void) {}



  //////////////////////////////////////////////////////////////////////////////
  // Operator overloading

  /* Used to read an element of the Array. */
  Type operator()(const unsigned int Index) const {
    // Check if index is out of bounds
    if(Index >= 3)
      printf("Error! Requested index is out of bounds\n");

    return Ar[Index];
  } // Type operator()(const unsigned int Index) cosnt {



  /* Used to write to an element of the Array. */
  Type & operator()(const unsigned int Index) {
    // Check if index is out of bounds
    if(Index >= 3)
      printf("Error! Requested index is out of bounds\n");

    return Ar[Index];
  } // Type & operator()(const unsigned int Index) {
};  // class Array_3 {

#endif
