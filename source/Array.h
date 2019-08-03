#if !defined(ARRAY_HEADER)
#define ARRAY_HEADER

#include <stdio.h>
#include <assert.h>
#include "Errors.h"



template <typename T, unsigned n>
class Array {
  private:
    // The actual array.
    T Ar[n];

  public:
    //////////////////////////////////////////////////////////////////////////////
    // Constructors, destructor

    // Constructors
    Array(void) {}                                         // Do nothing default constructor
    Array(const T Array_In[n]) {                           // Create an Array object from a regular Array
      for(unsigned i = 0; i < n; i++) { (*this).Ar[i] = Array_In[i]; }
    } // Array(const T Array_In[n]) {
    Array(const Array<T, n> & Array_In) {                  // Copy constructor
      for(unsigned i = 0; i < n; i++) { (*this).Ar[i] = Array_In.Ar[i]; }
    } // Array(const Array<T, n> & Ar_In) {
    Array(Array<T, n>&& Ar_In) = delete;                   // Deleted move constructor


    // Do nothing destructor
    ~Array(void) {}


    //////////////////////////////////////////////////////////////////////////////
    // Operator overloading

    // Read an element of the array
    T operator[](const unsigned Index) const {
      assert(Index < n);
      return Ar[Index];
    } // T operator[](const unsigned Index) const {

    // Write to an element of the array
    T & operator[](const unsigned Index) {
      assert(Index < n);
      return Ar[Index];
    } // T & operator[](const unsigned Index) {


    Array<T,n> & operator=(Array<T, n> Array_In) {         // Copy assignment operator
      for(unsigned i = 0; i < n; i++) { Ar[i] = Array_In.Ar[i]; }
      return *this;
    }
    Array<T,n> & operator=(Array<T,n> && Ar_In) = delete;  // Deleted move assignment operator


    //////////////////////////////////////////////////////////////////////////////
    // Other methods

    /* Determine the size of the array (number of elements) */
    unsigned GetSize(void) const { return n; }


    /* Fill the array with a set value */
    void Fill(T Value) {
      for(unsigned i = 0; i < n; i++) { Ar[i] = Value; }
    } // void Fill(T Value) {
};  // class Array {


#endif
