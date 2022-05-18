#include <string>

#include "format.h"

using std::string;
using std::to_string;

string Format::ElapsedTime(long seconds) { 
  long hh = seconds / 3600;
  long mm = (seconds % 3600) / 60;
  long ss = (seconds % 3600) % 60;
  
  string sec = to_string(ss);
  sec.insert(0, 2 - sec.length(), '0');
  
  string mins = to_string(mm);
  mins.insert(0, 2 - mins.length(), '0');
  
  string hrs = to_string(hh);
  hrs.insert(0, 2 - hrs.length(), '0');
  
  return hrs + ":" + mins + ":" + sec; 
}