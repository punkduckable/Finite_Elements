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



// Element errors
void Handle_Error(const Element_Errors Error) {
  switch(Error) {
    case Element_Errors::SUCCESS:
      printf("No Element errors!\n");
      break;


    case Element_Errors::STATIC_MEMBERS_NOT_SET:
      printf("Element Error! Static members have not been set.\n");
      break;

    case Element_Errors::STATIC_MEMBERS_ALREADY_SET:
      printf("Element Error! This element's static members have already been set!\n");
      break;



    case Element_Errors::MATERIAL_NOT_SET:
      printf("Element Error! The element material has not been set!\n");
      break;

    case Element_Errors::MATERIAL_ALREADY_SET:
      printf("Element Error! The element material has already been set!\n");
      break;



    case Element_Errors::ELEMENT_ALREADY_SET_UP:
      printf("Element Error! This element has already been set up! You can't set up an element twice!\n");
      break;

    case Element_Errors::ELEMENT_NOT_SET_UP:
      printf("Element Error! This element has not been set up (Node list not set!)\n");
      break;



    case Element_Errors::KE_ALREADY_SET_UP:
      printf("Element Error! This element's stiffness matrix (Ke) has already been set!\n");
      break;

    case Element_Errors::KE_NOT_SET_UP:
      printf("Element Error! This element's stiffness matirx (Ke) has not been set!\n");
      break;



    case Element_Errors::FE_ALREADY_SET_UP:
      printf("Element Error! This element's force vector (Fe) has already been set!\n");
      break;

    case Element_Errors::FE_NOT_SET_UP:
      printf("Element Error! This element's force vector (Fe) has not been set!\n");
      break;



    case Element_Errors::NODE_ID_INDEX_OUT_OF_BOUNDS:
      printf("Element Error! Invalid Node ID Index! Valid indices are 0-7\n");
      break;

    case Element_Errors::BAD_DETERMINANT:
      printf("Element Error! J <= 0. Something went very wrong.\n");
      break;



    default:
      printf("Element Error! Unknown Error\n");
      break;
  } // switch(Error) {
} // void Handle_Error(const Element_Errors Error) {

#endif
