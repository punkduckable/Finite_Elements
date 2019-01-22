#if !defined(FORWARD_DECLARATIONS_HEADER)
#define FORWARD_DECLARATIONS_HEADER

// Forward declarations for the classes in this project

/* Array_3: This is simply a template class for an array with 3 elements.
I realized that my code would be a lot easier to write if I had a template
wrapper class for 3 element arrays (either of booleans or doubles). */
template <typename Type>
class Array_3;

/* Node: This is a class for nodes (Finite element nodes)*/
class Node;

/* Element: This is a class for elements (Finite element) */
class Element;

#endif
