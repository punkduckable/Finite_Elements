#if !defined(ARRAY_SOURCE)
#define ARRAY_SOURCE

#include "Array.h"
#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////
// Constructors, Destructor

// Copy constructor
template<typename Type>
Array_3<Type>::Array_3(const Array_3<Type> & Ar_In) {
  Ar[0] = Ar_In.Ar[0];
  Ar[1] = Ar_In.Ar[1];
  Ar[2] = Ar_In.Ar[2];
} // Array_3<Type>::Array_3(const Array_3<Type> & Ar_In) {



// Construct by components.
template<typename Type>
Array_3<Type>::Array_3(const Type a, const Type b, const Type c) {
  Ar[0] = a;
  Ar[1] = b;
  Ar[2] = c;
} // Array_3<Type>::Array_3(const Type a, const Type b, const Type c) {



// Constructor using a 3 component array
template<typename Type>
Array_3<Type>::Array_3(const Type Ar_In[3]) {
  Ar[0] = Ar_In[0];
  Ar[1] = Ar_In[1];
  Ar[2] = Ar_In[2];
} // Array_3<Type>::Array_3(const Type Ar_In[3]) {





//////////////////////////////////////////////////////////////////////////////
// Operator overloading

/* Used to read an element of the Array. */
template<typename Type>
Type Array_3<Type>::operator()(const unsigned int Index) const {
  // Check if index is out of bounds. If so, throw an exception
  if(Index >= 3) {
    char Error_Message_Buffer[500];
    sprintf(Error_Message_Buffer,
            "Array Index Out Of Bounds Exception: thrown by Array_3<Type>::operator()\n"
            "Array_3 objects only have 3 components. Thus, the allowed indicies are 0,\n"
            "1, and 2. Requested index is %d\n",
            Index);
    throw Array_Index_Out_Of_Bounds(Error_Message_Buffer);
  } // if(Index >= 3) {

  return Ar[Index];
} // Type Array_3<Type>::operator()(const unsigned int Index) const {



/* Used to write to an element of the Array. */
template<typename Type>
Type & Array_3<Type>::operator()(const unsigned int Index) {
  // Check if index is out of bounds. If so, throw an exception
  if(Index >= 3) {
    char Error_Message_Buffer[500];
    sprintf(Error_Message_Buffer,
            "Array Index Out Of Bounds Exception: thrown by Array_3<Type>::operator()\n"
            "Array_3 objects only have 3 components. Thus, the allowed indicies are 0,\n"
            "1, and 2. Requested index is %d\n",
            Index);
    throw Array_Index_Out_Of_Bounds(Error_Message_Buffer);
  } // if(Index >= 3) {

  return Ar[Index];
} // Type & Array_3<Type>::operator()(const unsigned int Index) {



template<typename Type>
Array_3<Type> & Array_3<Type>::operator=(const Array_3<Type> Ar_In) {
  for(int i = 0; i < 3; i++)
    Ar[i] = Ar_In.Ar[i];

  return *this;
} // Array_3<Type> & Array_3<type>::operator=(const Array_3<Type> Ar_In) {

#endif
