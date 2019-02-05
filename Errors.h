#if !defined(ERRORS_HEADER)
#define ERRORS_HEADER

// Node Errors
namespace Node_Errors {
  enum Errors{SUCCESS, NODE_NOT_SET_UP, ORIGINAL_POSITION_SET, FIXED_POSITION_COMPONENT, INDEX_OUT_OF_BOUNDS};
  void Handle_Error(const Errors Error);
} //namespace Node_Errors {

// Element errors
namespace Element_Errors {
  enum Errors{SUCCESS, STATIC_MEMBERS_NOT_SET, STATIC_MEMBERS_ALREADY_SET, INDEX_OUT_OF_BOUNDS, };
  void Handle_Error(const Errors Error);
} // namespace Element_Errors {

#endif
