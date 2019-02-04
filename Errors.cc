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

#endif
