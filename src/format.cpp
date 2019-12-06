#include <string>
#include <unistd.h>
#include "format.h"
using std::string;
string Format::ElapsedTime(long seconds) 
{ 
  
  int x;
  int y;
  int z;
  x = seconds / 3600;
  y = (seconds%3600) / 60;  
  z = (seconds%3600) % 60;
  std::string format;
  if (x <= 9) 
  { 
    format = "00"; 
    }
    else {
      format = std::to_string(x) + ":";
    }
  if (y <= 9) 
  { 
    format += "00"; 
    }
    else {
      format += std::to_string(y) + ":";
    }
  if (z <= 9) 
  { 
    format += "00"; 
    }
    else {
      format += std::to_string(z);
    }
  return format; 
}