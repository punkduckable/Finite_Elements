#if !defined(ERRORS_HEADER)
#define ERRORS_HEADER

// Node Errors
namespace Node_Errors {
  enum Errors{SUCCESS = 0, BC_SET = 1, ORIGINAL_POSITION_SET = 2, BC_FIXED_POS = 3, INDEX_OUT_OF_BOUNDS = 4};
  void Handle_Error(const Errors Error);
} //namespace Node_Errors {

#endif
