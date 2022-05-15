#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// Returns the system's CPU
Processor& System::Cpu() { return cpu_; }

bool Compare(Process a, Process b){
  if(a < b){
    return true;
  }
  return false;
}

// Returns a container composed of the system's processes
vector<Process>& System::Processes() { 
  processes_.clear();
  vector<int> pIds = LinuxParser::Pids();
  for(int pid : pIds){
    Process process(pid);
    process.User(LinuxParser::User(pid));
    process.Command(LinuxParser::Command(pid));
    process.Ram(LinuxParser::Ram(pid));
    vector<long int> cpuTimes = LinuxParser::ProcessCpuTimes(pid);
    process.UpTime(cpuTimes[cpuTimes.size()-1]);
    process.CpuUtilization(cpuTimes);
    
    processes_.emplace_back(process);
  }
  
  sort(processes_.begin(), processes_.end(), Compare);
  reverse(processes_.begin(), processes_.end());
  return processes_; 
}

// Returns the system's kernel identifier (string)
std::string System::Kernel() { 
  return LinuxParser::Kernel(); 
}

// Returns the system's memory utilization
float System::MemoryUtilization() { 
  return LinuxParser::MemoryUtilization(); 
}

// Returns the operating system name
std::string System::OperatingSystem() { 
  return LinuxParser::OperatingSystem(); 
}

// Returns the number of processes actively running on the system
int System::RunningProcesses() { 
  return LinuxParser::RunningProcesses(); 
}

// Returns the total number of processes on the system
int System::TotalProcesses() { 
  return LinuxParser::TotalProcesses(); 
}

// Returns the number of seconds since the system started running
long int System::UpTime() { 
  return LinuxParser::UpTime(); 
}
