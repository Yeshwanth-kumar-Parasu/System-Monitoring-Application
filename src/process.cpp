#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include "linux_parser.h"
#include "process.h"
using std::string;
using std::to_string;
using std::vector;
using std::stol;
Process::Process(int pid) 
{
Process::pid_=pid;
}
int Process::Pid() 
{ 
return pid_; 
}
float Process::CpuUtilization() 
{ 
  long Up_time = LinuxParser::UpTime();
  long Start_time = LinuxParser::UpTime(pid_);
  long Total_time = LinuxParser::ActiveJiffies(pid_);
  
  long seconds = Up_time -(Start_time/sysconf(_SC_CLK_TCK));
    
  return (Total_time/sysconf(_SC_CLK_TCK))/seconds;
}
string Process::Command() 
{ 
return LinuxParser::Command(pid_); 
}
string Process::Ram() 
{ 
return LinuxParser::Ram(pid_); 
}
string Process::User() 
{ 
return LinuxParser::User(pid_); 
}
long int Process::UpTime() 
{ 
return LinuxParser::UpTime(pid_); 
}
bool Process::operator<(Process const& a) const 
{ 
  return stol(LinuxParser::Ram(pid_)) > stol(LinuxParser::Ram(a.pid_));
}