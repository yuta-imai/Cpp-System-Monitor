#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
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

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, fixed_string, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> fixed_string >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
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
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() { 
  string line;
  string key;
  string value;
  float mem_total{0};
  float mem_free{0};
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {

      if(mem_total != 0 && mem_free != 0) {
        break;
      }
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "MemTotal:") {
        mem_total = std::stof(value);
      }

      if (key == "MemFree:") {
        mem_free = std::stof(value); 
      }
    }
  }
  return (mem_total - mem_free)/mem_total;
}

long LinuxParser::UpTime() {
  string line;
  string value;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  std::getline(filestream, line);
  std::istringstream linestream(line);
  linestream >> value;
  return std::stol(value);
 }

vector<string> LinuxParser::CpuUtilization() {
  string line;
  string key, user, nice, system, idle, iowait, irq, softirq, steal;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open()) {
    while(std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> user >> nice >> system >> idle >> iowait >>  irq >> softirq >> steal;
      if(key == "cpu") {
        break;
      }
    }
  }
  return vector<string>{user, nice, system, idle, iowait, irq, softirq, steal};
}

int LinuxParser::TotalProcesses() {
  string line;
  string key;
  string value;
  int num_processes{0};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {

      if(num_processes != 0) {
        break;
      }
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes") {
        num_processes = std::stoi(value);
      }

    }
  }
  return num_processes;
}

int LinuxParser::RunningProcesses() {
  string line;
  string key;
  string value;
  int num_processes{0};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {

      if(num_processes != 0) {
        break;
      }
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "procs_running") {
        num_processes = std::stoi(value);
      }

    }
  }
  return num_processes;
}

float LinuxParser::CpuUtilization(int pid) {
  string line;
  string pid_in_file, comm, state,ppid, pgrp, session, tty_nr;
  string tpgid, flags, minflt, cminflt, majflt, cmaflt;
  string utime, stime, cutime, cstime;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if(filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> pid_in_file >> comm >> state >>ppid >> pgrp >> session >> tty_nr >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmaflt >> utime >> stime >> cutime >> cstime;
    float total_time = std::stof(utime) + std::stof(stime);
    return 100 * ((total_time/CLOCKS_PER_SEC)/UpTime(pid));
  } else {
    return 0.0;
  }
}

string LinuxParser::Command(int pid) { 
  string line;
  string value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + "/cmdline");
  if(filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> value;
    return value;
  } else {
    return string();
  }
}

string LinuxParser::Ram(int pid) {
  string line;
  string key;
  string value{};
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + "/status");
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmSize:") {
        break;
      }

    }
  }
  return std::to_string((int)std::stof(value)/1024);
}

string LinuxParser::Uid(int pid) {
  string line;
  string key;
  string value{0};
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + "/status");
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid:") {
        break;
      }

    }
  }
  return value;
}

string LinuxParser::User(int pid) {
  string uid = Uid(pid);

  string line;
  string username{};
  string password;
  string uid_in_file;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> username >> password >> uid_in_file;
      if (uid_in_file == uid) {
        break;
      }
    }
  }
  return username;

}
 
long int LinuxParser::UpTime(int pid) { 
  string line;
  string pid_in_file, comm, state,ppid, pgrp, session, tty_nr;
  string tpgid, flags, minflt, cminflt, majflt, cmaflt;
  string utime, stime, cutime, cstime, priority, nice;
  string num_threads, itrealvalue, starttime;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if(filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> pid_in_file >> comm >> state >>ppid >> pgrp >> session >> tty_nr >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmaflt >> utime >> stime >> cutime >> cstime >> priority >> nice >> num_threads >> itrealvalue >> starttime;
    return UpTime() - std::stol(starttime)/sysconf(_SC_CLK_TCK);
  } else {
    return 0;
  }
}