#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>

#include "linux_parser.h"

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                               
  std::string User();                     
  std::string Command();                  
  float CpuUtilization() const;                 
  std::string Ram() const;                       
  long int UpTime();                       
  bool operator<(Process const& a) const;  
  
  Process(int pid);
  void User(std::string user);
  void Command(std::string command);
  void CpuUtilization(std::vector<long int>& times);
  void Ram(std::string ram);
  void UpTime(long int upTime);

 private:
  int pid_;
  std::string user_;
  std::string command_;
  float cpuUtilization_;
  std::string ram_;
  long int upTime_;
};

#endif