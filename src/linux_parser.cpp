#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iomanip>
#include "linux_parser.h"
using std::stof;
using std::string;
using std::to_string;
using std::vector;
// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() 
{
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) 
{
    while (std::getline(filestream, line)) 
{
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) 
{
        if (key == "PRETTY_NAME") 
{
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() 
{
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) 
{
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() 
{
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) 
{
    // Is this a directory?
    if (file->d_type == DT_DIR) 
{
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) 
{
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() 
{ 
  float Total_memory = 0.0; 
  float Free_memory = 0.0;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  string line;
  string key;
  string value;
  if (stream.is_open()) 
  {
    while (std::getline(stream, line)) 
    {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Total Memory:") 
      {
        Total_memory = stof(value);
      }
      if (key == "Free Memory:") 
      {
        Free_memory = stof(value);
      }
    }
    return (Total_memory-Free_memory)/Total_memory;
  }
  return 0.0; 
  }

long LinuxParser::UpTime() 
{ 
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  string line;
  string uptime;
  if (stream.is_open()) 
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
    return stol(uptime);
  }
  return 0; 
  }

long LinuxParser::Jiffies() 
{ 
  vector<string> cpuUtilization = CpuUtilization();
  long jiffies = 0;
  for(int i = kUser_; i <= kSteal_; i++) 
  {
    jiffies += stol(cpuUtilization[i]);
  }
  return jiffies; 
  }

long LinuxParser::ActiveJiffies(int pid) 
{ 
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  long Active_jiffies = 0;
  string line;
  string value;
  if (stream.is_open()) 
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    int i = 0;
    while (linestream >> value) 
    {
      if (i > 12 && i < 17) 
      {
        Active_jiffies += stol(value);
      }
      i++;
    }
  }
  return Active_jiffies; 
  }

long LinuxParser::ActiveJiffies() 
{ 
  long jiffies = Jiffies();
  long idle_jiffies = IdleJiffies();
  return jiffies-idle_jiffies; 
  }

long LinuxParser::IdleJiffies() 
{ 
  vector<string> cpuUtilization = CpuUtilization();
  long idle_jiffies = 0;
  for(int i = kIdle_; i <= kIOwait_; i++) 
  {
    idle_jiffies += stol(cpuUtilization[i]);
  }
  return idle_jiffies; 
  }

vector<string> LinuxParser::CpuUtilization() 
{ 
  vector<string> Cpu_info;
  std::ifstream stream(kProcDirectory + kStatFilename);
  string line;
  string value;
  if (stream.is_open()) 
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value) 
    {
      if (value != "cpu") 
      {
        Cpu_info.push_back(value);
      }
    }
  }
  return Cpu_info; 
  }

string LinuxParser::GetValueFromProcStat(string keyToFind) 
{
  std::ifstream stream(kProcDirectory + kStatFilename);
  string line;
  string key;
  string value;
  if (stream.is_open()) 
  {
    while (std::getline(stream, line)) 
    {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == keyToFind) 
      {
        return value;
      }
    }
  }
  return value;
}

int LinuxParser::TotalProcesses() 
{ 
  string value = GetValueFromProcStat("processes");
  if (value != "") 
  {
    return stoi(value);
  }
  return 0; 
  }

int LinuxParser::RunningProcesses() 
{ 
  string value = GetValueFromProcStat("procs_running");
  if (value != "") 
  {
    return stoi(value);
  }
  return 0; 
  }

string LinuxParser::Command(int pid) 
{ 
  string command;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) 
  {
    std::getline(stream, command);
    return command;
  }
  return string(); 
  }

string LinuxParser::Ram(int pid) 
{ 
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  string line;
  string key;
  string value;
  std::stringstream ram;
  if (stream.is_open()) 
  {
    while (std::getline(stream, line)) 
    {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmSize:") 
      {
       ram << std::fixed << std::setprecision(3) << stof(value) / 1000; 
       return ram.str();
      }
    }
  }
  return "0"; 
  }

string LinuxParser::Uid(int pid) 
{ 
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  string line;
  string key;
  string value;
  if (stream.is_open()) 
  {
    while (std::getline(stream, line)) 
    {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid:") 
      {
        return value;
      }
    }
  }
  return string(); 
  }

string LinuxParser::User(int pid) 
{ 
  string uid = Uid(pid);
  std::ifstream stream(kPasswordPath);
  string line;
  string key;
  string x;
  string value;
  if (stream.is_open()) 
  {
   while (std::getline(stream, line)) 
   {
     std::replace(line.begin(), line.end(), ':', ' ');
     std::istringstream linestream(line);
     linestream >> key >> x >> value;
     if (value == uid) 
     {
       return key;
     }
   }
  }
  return string(); 
  }

long LinuxParser::UpTime(int pid) 
{ 
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  string line;
  string value;
  if (stream.is_open()) 
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
	int i = 0;
    while (linestream >> value) 
    {
      if (i == 21) {
        return stol(value)/sysconf(_SC_CLK_TCK);
      }
      i++;
    }
  }
  return 0; 
  }