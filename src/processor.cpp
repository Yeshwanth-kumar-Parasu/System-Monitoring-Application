#include "processor.h"
#include "linux_parser.h"
#include <unistd.h>
float Processor::Utilization() 
{ 
  Active_jiffies_start = LinuxParser::ActiveJiffies();
  Active_jiffies_end = LinuxParser::ActiveJiffies();
  Total_jiffies_start = LinuxParser::Jiffies();
  Total_jiffies_end = LinuxParser::Jiffies();
  usleep(100000); 
  long Active_delta = Active_jiffies_end - Active_jiffies_start;
  long Total_delta = Total_jiffies_end - Total_jiffies_start;
  if (Total_delta == 0) 
{
    return 0.0;
  }
  
  return float(Active_delta)/float(Total_delta); 
}