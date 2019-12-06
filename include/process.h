#ifndef PROCESS_H
#define PROCESS_H
#include <string>
using std::string;
class Process 
{
 public:
  int Pid();                               
  string User();                      
  string Command();                   
  float CpuUtilization();                  
  string Ram();                       
  long int UpTime();                       
  bool operator<(Process const& a) const; 
  Process(int pid);
 
 private:
  int pid_;
};
#endif