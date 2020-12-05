#include <string>
#include <vector>

#include "linux_parser.h"
#include "processor.h"

using std::stol;
using std::string;
using std::vector;

float Processor::Utilization() {
    vector<string> cpu_stat = LinuxParser::CpuUtilization();

    long int idle = stol(cpu_stat[3]) + stol(cpu_stat[4]);
    long int total = idle +  stol(cpu_stat[0]) + stol(cpu_stat[1]) + stol(cpu_stat[2]) + stol(cpu_stat[5]) + stol(cpu_stat[6]) + stol(cpu_stat[7]);

    long int totald = total - prev_total_;
    long int idled = idle - prev_idle_;


    prev_total_ = total;
    prev_idle_ = idle;

    return (float)(totald - idled)/totald;

}