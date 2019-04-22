#if !defined(ERRORS_HEADER)
#define ERRORS_HEADER

// Node Errors
enum class Node_Errors {SUCCESS,
                        NODE_NOT_SET_UP, NODE_ALREADY_SET_UP,
                        BC_ALREADY_SET,
                        INDEX_OUT_OF_BOUNDS};

void Handle_Error(const Node_Errors Error);



// Element errors
enum class Element_Errors{SUCCESS,
                          STATIC_MEMBERS_NOT_SET, STATIC_MEMBERS_ALREADY_SET,
                          MATERIAL_NOT_SET, MATERIAL_ALREADY_SET,
                          ELEMENT_ALREADY_SET_UP, ELEMENT_NOT_SET_UP,
                          KE_ALREADY_SET_UP, KE_NOT_SET_UP,
                          FE_ALREADY_SET_UP, FE_NOT_SET_UP,
                          NODE_ID_INDEX_OUT_OF_BOUNDS,
                          BAD_DETERMINANT};
void Handle_Error(const Element_Errors Error);

#endif
