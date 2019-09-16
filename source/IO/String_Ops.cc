#if !defined(STRING_OPERATIONS_SOURCE)
#define STRING_OPERATIONS_SOURCE

#include "String_Ops.h"

bool String_Ops::Contains(const char* Buffer, const char* Word, unsigned Start_At) {
  /* Function description:
  This function determines if Word is contained in Buffer. The optional Start_At
  argument can be used to only search through part of the string. If, for
  example, Start_At = 5, then this function will only search for matches that
  begin at index 5 (or later) of Buffer.

  My inp reader frequently checks if a particular word is in a string. Thus, I
  wrote this function to automate that process. */

  /* Assumptions:
  This function assumes that both Buffer and Word are NULL TERMINATED strings.
  That is, I assume that both end with the \0 character. */

  /* First, check if Buffer has fewer than Start_At characters (which happens
  if there is a \0 in a index whose value is less than Start_At). If not, return
  false */
  for(unsigned i = 0; i < Start_At; i++) {
    if(Buffer[i] == '\0') { return false; }
  } // for(unsigned i = 0; i < Start_At; i++) {

  // Loop through the characters of Buffer.
  unsigned i = Start_At;
  while(Buffer[i] != '\0') {
    // At each one, see if Word starts at that character.
    unsigned j = 0;
    while(Buffer[i+j] == Word[j]) {
      j++;

      /* If we're still in here and we've reached the end of "Word" then
      we've found a match! */
      if(Word[j] == '\0') { return true; }

      /* If we haven't reached the end of Word but we have reached the end of
      Buffer then Buffer does not contain Word. */
      if(Buffer[i+j] == '\0') { return false; }
    } // while(Buffer[i+j] == Word[j]) {

    i++;
  } // while(Buffer[i] != '\0') {

  /* If we get here then we cycled through Buffer without finding a match.
  Thus, buffer does not contain Word. */
  return false;
} // bool String_Ops::Contains(const char* Buffer, const char* Word, unsigned Start_At) {



std::vector<std::string> String_Ops::Split(std::string & S, const char delim) {
  /* Function description:
  This function is designed to read in a string and split it using a delimeter.
  The characters between any two instances of delim (as well as any characters
  before the first instance of delim and the characters after the last instance
  of delim) are packaged together as a substring and added to the substring
  vector (which is what get's returned).

  Delim is a defaulted argument. By default, delim = ','. */

  std::vector<std::string> Sub_Strings;

  unsigned len = S.length();
  unsigned Index_Start = 0;                      // Index of the start of the current substring.
  unsigned N_Chars_Since_Delim = 0;              // Number of characters after the start of the substring that are not Delim
  for(unsigned i = 0; i < len; i++) {
    if(S[i] == delim) {
      // Add a new substring to the Sub String vector.
      Sub_Strings.push_back(S.substr(Index_Start, N_Chars_Since_Delim));

      // Update Index_Start (to just 1 character after the delim index)
      Index_Start = i+1;

      // We're starting a new substring, so N_Chars_Since_Delim = 0;
      N_Chars_Since_Delim = 0;
    } // if(S[i] == delim) {
    else { N_Chars_Since_Delim++; }
  } // for(unsigned i = 0; i < len; i++) {

  /* Finally, make a substring from the charcters after the last instance of
  delim. Note that this only happens if Index_Start < len. */
  if(Index_Start < len) { Sub_Strings.push_back(S.substr(Index_Start, N_Chars_Since_Delim)); }

  return Sub_Strings;
} // std::vector<std::string> String_Ops:Split(std::string & S, const char delim) {

#endif
