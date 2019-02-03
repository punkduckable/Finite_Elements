#if !defined(ARRAY_SOURCE)
#define ARRAY_SOURCE

#include "Array.h"
#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////
// Constructors, Destructor

template<typename Type>
Array_3<Type>::Array_3(const Type Ar_In[3]) {
  Ar[0] = Ar_In[0];
  Ar[1] = Ar_In[1];
  Ar[2] = Ar_In[2];
} // Array_3<Type>::Array_3(const Type Ar_In[3]) {


// Initialize using a 3 component array
template<typename Type>
Array_3<Type>::Array_3(const Type a, const Type b, const Type c) {
  Ar[0] = a;
  Ar[1] = b;
  Ar[2] = c;
} // Array_3<Type>::Array_3(const Type a, const Type b, const Type c) {




//////////////////////////////////////////////////////////////////////////////
// Operator overloading

/* Used to read an element of the Array. */
template<typename Type>
Type Array_3<Type>::operator()(const unsigned int Index) const {
  // Check if index is out of bounds
  if(Index >= 3)
    printf("Error! Requested index is out of bounds\n");

  return Ar[Index];
} // Type Array_3<Type>::operator()(const unsigned int Index) const {


/* Used to write to an element of the Array. */
template<typename Type>
Type & Array_3<Type>::operator()(const unsigned int Index) {
  // Check if index is out of bounds
  if(Index >= 3)
    printf("Error! Requested index is out of bounds\n");

  return Ar[Index];
} // Type & Array_3<Type>::operator()(const unsigned int Index) {

#endif
