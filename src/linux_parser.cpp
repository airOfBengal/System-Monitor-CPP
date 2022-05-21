#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::stol;

string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}


vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.emplace_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Reads and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float memUsed = 0.0, memTotal, memFree;
  string line, key, value;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
	  if(key == "MemTotal:"){
        try{
      	  memTotal = stof(value);
        }catch(...){
          memTotal = 1.0;
        }
      }else if(key == "MemFree:"){
        try{
      	  memFree = stof(value);
        }catch(...){
          memFree = 0.0;
        }
        break;
      }
    }
  }
  memUsed = (memTotal - memFree) / memTotal;
  return memUsed; 
}

// Reads and return the system uptime
long LinuxParser::UpTime() {
  long upTime = 0;
  string line, upTimeString;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if(stream.is_open()){
    std::getline(stream, line); 
    std::istringstream linestream(line);
    linestream >> upTimeString;
    try{
      upTime = std::stol(upTimeString);
    }catch(...){
      upTime = 0;
    }
  }
  return upTime; 
}


// Reads and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> cpuTimes;
  string line, cpu;
  
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu;
    if(cpu == "cpu"){
      while(linestream >> cpu){
         cpuTimes.emplace_back(cpu);
      }
    }
  }
  
  return cpuTimes; 
}

// Reads and return the total number of processes
int LinuxParser::TotalProcesses() { 
  int totalProcesses = 0;
  string line, key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
	  if(key == "processes"){
        try{
      	  totalProcesses = std::stoi(value);
        }catch(...){
          totalProcesses = 0;
        }
        return totalProcesses;
      }
    }
  }
  return totalProcesses; 
}

// Reads and return the number of running processes
int LinuxParser::RunningProcesses() { 
  int runningProcesses = 0;
  string line, key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
	  if(key == "procs_running"){
        try{
      	  runningProcesses = std::stoi(value);
        }catch(...){
          runningProcesses = 0;
        }
        return runningProcesses;
      }
    }
  }
  return runningProcesses; 
}

// Reads and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    return line;
  }
  return string(); 
}

// Reads and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string line, key, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
	  if(key == "VmSize:"){
        try{
      	  return to_string((long)(stol(value) * 0.001));
        }catch(...){
          return "0";
        }
      }
    }
  }
  return string(); 
}

// Reads and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line, key, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
	  if(key == "Uid:"){
      	return value;
      }
    }
  }
  return string();  
}

// Reads and return the user associated with a process
string LinuxParser::User(int pid) { 
  string line;
  string key;
  string value, value2;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    string uid = Uid(pid);
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value >> value2) {
        if (value2 == uid) {
          return key;
        }
      }
    }
  }
  return key;
}


// #14: utime, #15: stime, #16: cutime, #17: cstime, #22: starttime.
vector<long int> LinuxParser::ProcessCpuTimes(int pid){
  vector<long int> cpuTimes; 
  string line, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    for(int i=1;i<=22;i++){
       linestream >> value;
       if(i == 14 || i == 15 || i == 16 || i == 17 || i == 22) {
         try{
           cpuTimes.emplace_back(stol(value));
         }catch(...){
           cpuTimes.emplace_back(0);
         }
       }
    }
  }
  return cpuTimes;
}
