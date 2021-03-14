#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
  char buffer[10];
  int HH = (int)(seconds/3600);
  int MM = ((int)(seconds/60))%60;
  int SS = (int)(seconds%60);
  sprintf(buffer, "%.2d:%.2d:%.2d", HH, MM, SS);
  return buffer; 
}