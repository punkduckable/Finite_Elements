#if !defined(ERRORS_SOURCE)
#define ERRORS_SOURCE

#include "Errors.h"
#include <stdio.h>

// Node errors
void Node_Errors::Handle_Error(const Node_Errors::Errors Error) {
  switch(Error) {
    case SUCCESS:
      printf("No Node Errors!\n");
      break;

    case NODE_NOT_SET_UP:
      printf("Node Error! This node has not has its position/BC's set up!\n");
      break;

    case ORIGINAL_POSITION_SET:
      printf("Node Error! You tried to modify a Node's origional position!\n");
      break;

    case FIXED_POSITION_COMPONENT:
      printf("Node Error! You tried to set a component of a Node's position that is fixed (precribed BC)\n");
      break;

    case INDEX_OUT_OF_BOUNDS:
      printf("Node Error! The component index must be 0, 1, or 2\n");
      break;

    default:
      printf("Node Error! Unknown Error\n");
      break;
  } //   switch(Error) {
} // void Node_Errors::Handle_Error(const Node_Errors::Errors Error) {



// Element errors
void Element_Errors::Handle_Error(const Element_Errors::Errors Error) {
  switch(Error) {
    case SUCCESS:
      printf("No Element errors!\n");
      break;

    case STATIC_MEMBERS_NOT_SET:
      printf("Element Error! Static members have not been set.\n");
      break;

    case STATIC_MEMBERS_ALREADY_SET:
      printf("Element Error! This element's static members have already been set!\n");
      break;

    case INDEX_OUT_OF_BOUNDS:
      printf("Element Error! Index out of bounds!\n");
      break;

    case ELEMENT_NOT_SET_UP:
      printf("Element Error! This element has not been set up (Node list not set!)\n");
      break;

    default:
      printf("Element Error! Unknown Error\n");
      break;
  } // switch(Error) {
} // void Element_Errors::Handle_Error(const Element_Errors::Errors Error) {

#endif
