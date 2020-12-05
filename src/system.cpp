#include <unistd.h>
#include <chrono>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"

#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu() { 
    if(!cpuInitilized_) {
        cpu_ = Processor();
    }
    return cpu_; 
}

vector<Process>& System::Processes() {
    vector<int> pids = LinuxParser::Pids();
    vector<Process> processes;
    for(int pid: pids) {
        processes.push_back(Process(pid));
    }
    std::sort(processes.begin(), processes.end(), [](Process a, Process b) {
        return b < a;        
    });
    processes_ = processes;
    return processes_; 
}

std::string System::Kernel() { 
    if(this->kernel_.empty()) {
        this->kernel_ = LinuxParser::Kernel();
    }

    return this->kernel_;
}


float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }


std::string System::OperatingSystem() {
    if(this->os_.empty()) {
        this->os_ = LinuxParser::OperatingSystem();
    }

    return this->os_;
}

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }