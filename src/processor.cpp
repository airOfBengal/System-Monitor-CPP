#include "processor.h"
#include <vector>
#include <string>

using std::stof;
using std::string;
using std::vector;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  float cpuUtilization = 0.0;
  
  vector<string> cpuTimes = LinuxParser::CpuUtilization();
  float user = stof(cpuTimes[0]);
  float nice = stof(cpuTimes[1]);
  float system = stof(cpuTimes[2]);
  float idle = stof(cpuTimes[3]);
  float iowait = stof(cpuTimes[4]);
  float irq = stof(cpuTimes[5]);
  float softirq = stof(cpuTimes[6]);
  float steal = stof(cpuTimes[7]);
  //float guest = stof(cpuTimes[8]);
  //float guestNice = stof(cpuTimes[9]);
  
  float Idle = idle + iowait;
  float NonIdle = user + nice + system + irq + softirq + steal;
  float Total = Idle + NonIdle;
  
  cpuUtilization = (Total - Idle) / Total;
  
  return cpuUtilization; 
}