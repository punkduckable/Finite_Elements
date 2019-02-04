#if !defined(ERRORS_HEADER)
#define ERRORS_HEADER

// Node Errors
namespace Node_Errors {
  enum Errors{SUCCESS, ORIGINAL_POSITION_SET, FIXED_POSITION_COMPONENT, INDEX_OUT_OF_BOUNDS};
  void Handle_Error(const Errors Error);
} //namespace Node_Errors {

#endif
