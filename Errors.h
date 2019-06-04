#if !defined(ERRORS_HEADER)
#define ERRORS_HEADER

#include <string>

// Node Errors
enum class Node_Errors {SUCCESS,
                        NODE_NOT_SET_UP, NODE_ALREADY_SET_UP,
                        BC_ALREADY_SET,
                        INDEX_OUT_OF_BOUNDS};

void Handle_Error(const Node_Errors Error);






////////////////////////////////////////////////////////////////////////////////
// Element Exceptions

/* Here I define the Element exception classes. Every Element exception class is
derived from Element_Exception. As such, a Element_Exception handler will
catch every type of Element exception. With that said, you can (and should)
write handlers for specific types of Element exceptions.

Note: All exception objects should be initialized with the message that you want to
print out when the exception is caught.
________________________________________________________________________________
                        Description of the subclasses:

Element_Not_Set_Up: This exception is thrown whenever the user runs a method
that requires data that has not yet been set up. Many methods in the element
class can not run until certian data paramaters (static  members, material data,
Ke, etc...). It is possible to run these methods without this data, so an
exception is thrown. An "Element_Not_Set_Up" exception basically says "you
forgot to set up this element".

Element_Already_Set_Up: On the flip side, this exception is thrown whenever
the user tries to set a data member that has already been set (and should not
be set again). The material, for example, should only be set once. An
"Element_Already_Set_Up" exception basically says "you tried to set something
that has already been set and should not be modified".

Element_Bad_Determinant: This exception is thrown whenever an unphysical
jacobian determinant (one that is <= 0) is computed. J should always be
strictly positive, so getting an Element_Bad_Determinant exception probably
means that something is wrong with the way that the nodes are ordered. */

// Element Exception base class
class Element_Exception {
  private:
    const std::string Error_Message;
  public:
    Element_Exception(const char* Error_Message) : Error_Message(Error_Message) {};
    const char* what() const { return Error_Message.c_str(); }
}; // class Element_Exception {



class Element_Not_Set_Up : public Element_Exception {
  public:
    Element_Not_Set_Up(const char* Error_Message) : Element_Exception(Error_Message) {}
}; // class Element_Not_Set_Up : public Element_Errors {



class Element_Already_Set_Up : public Element_Exception {
  public:
    Element_Already_Set_Up(const char* Error_Message) : Element_Exception(Error_Message) {}
}; // class Element_Already_Set_Up : public Element_Errors {



class Element_Bad_Determinant : public Element_Exception {
  public:
    Element_Bad_Determinant(const char* Error_Message) : Element_Exception(Error_Message) {}
}; // class Element_Bad_Determinant : public Element_Exception {

enum class Element_Errors{SUCCESS,
                          STATIC_MEMBERS_NOT_SET, STATIC_MEMBERS_ALREADY_SET,
                          MATERIAL_NOT_SET, MATERIAL_ALREADY_SET,
                          ELEMENT_ALREADY_SET_UP, ELEMENT_NOT_SET_UP,
                          KE_ALREADY_SET_UP, KE_NOT_SET_UP,
                          FE_ALREADY_SET_UP, FE_NOT_SET_UP,
                          NODE_ID_INDEX_OUT_OF_BOUNDS,
                          BAD_DETERMINANT};
void Handle_Error(const Element_Errors Error);






////////////////////////////////////////////////////////////////////////////////
// Matrix exceptions

/* Here I define the Matrix class exceptions.  Every Matrix exception class is
derived from Matrix_Exception. As such, a Matrix_Exception handler will
catch every type of Matrix exception. With that said, you can (and should)
write handlers for specific types of Matrix exceptions.

Note: All exception objects should be initialized with the message that you want to
print out when the exception is caught.
________________________________________________________________________________
                        Description of the subclasses:

Matrix_Dimension_Mismatch: This exception is thrown whenever an operation
involving two matricies is not possible because the two matricies are
incompatible. This could happen if you're trying to compute M1*M2 when
the number of columns in M1 does not equal the number of rows in M2, or if you
try to set two matricies with different dimensions and/or memory layout equal
to another (move = operator).

Matrix_Index_Out_Of_Bounds: This exception is thrown whenever the user tries to
access a component that is outside of the matrix (requested row or column index
is too big).*/

// Matrix Exception base class
class Matrix_Exception {
  private:
    const std::string Error_Message;
  public:
      Matrix_Exception(const char* Error_Message) : Error_Message(Error_Message) {};
      const char* what() const { return Error_Message.c_str(); }
}; // class Matrix_Exception {



class Matrix_Dimension_Mismatch : public Matrix_Exception {
  public:
      Matrix_Dimension_Mismatch(const char* Error_Message) : Matrix_Exception(Error_Message) {}
}; // class Matrix_Dimension_Mismatch : public Matrix_Exception {



class Matrix_Index_Out_Of_Bounds : public Matrix_Exception {
  public:
      Matrix_Index_Out_Of_Bounds(const char* Error_Message) : Matrix_Exception(Error_Message) {}
}; // class Matrix_Index_Out_Of_Bounds : public Matrix_Exception {

#endif
