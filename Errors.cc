#if !defined(ERRORS_SOURCE)
#define ERRORS_SOURCE

#include "Errors.h"
#include <stdio.h>

// Node errors
void Handle_Error(const Node_Errors Error) {
  switch(Error) {
    case Node_Errors::SUCCESS:
      printf("No Node Errors!\n");
      break;



    case Node_Errors::NODE_NOT_SET_UP:
      printf("Node Error! This node has not has its position/BC's set up!\n");
      break;

    case Node_Errors::NODE_ALREADY_SET_UP:
      printf("Node Error! You tried to set up a Node that has already been set up!\n");
      break;



    case Node_Errors::BC_ALREADY_SET:
      printf("Node Error! This prescribed boundary BC has already been set!\n");
      break;



    case Node_Errors::INDEX_OUT_OF_BOUNDS:
      printf("Node Error! The component index must be 0, 1, or 2\n");
      break;



    default:
      printf("Node Error! Unknown Error\n");
      break;
  } //   switch(Error) {
} // void Handle_Error(const Node_Errors Error) {


#endif
