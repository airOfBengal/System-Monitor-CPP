#include <string>

#include "format.h"

using std::string;
using std::to_string;

string Format::ElapsedTime(long seconds) { 
  long hh = seconds / 3600;
  long mm = (seconds % 3600) / 60;
  long ss = (seconds % 3600) % 60;
  
  return to_string(hh) + ":" + to_string(mm) + ":" + to_string(ss); 
}