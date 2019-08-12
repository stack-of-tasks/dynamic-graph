/* Copyright LAAS, CNRS
 * Author: O. Stasse, 2019
 * See LICENSE file in the root directory of this repository.
 */
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <dynamic-graph/process-list.hh>

using namespace dynamicgraph::CPU;
CPUData::CPUData():
  user_mode_time_(0),
  nice_time_(0),
  system_time_(0),
  idle_time_(0),
  iowait_time_(0),
  irq_time_(0),
  softirq_time_(0),
  steal_time_(0),
  guest_time_(0),
  guest_nice_time_(0),
  percent_(0.0)
{
}

void CPUData::ProcessLine(std::istringstream &aCPULine)
{
  unsigned long long int luser_mode_time=0,
    lnice_time=0, lsystem_time=0, lidle_time=0,
    liowait_time=0, lirq_time=0, lsoftirq_time=0,
    lsteal_time=0,lguest_time=0, lguest_nice_time;
  
  aCPULine >> luser_mode_time;
  aCPULine >> lnice_time;
  aCPULine >> lsystem_time;
  aCPULine >> lidle_time;
  aCPULine >> liowait_time;
  aCPULine >> lirq_time;
  aCPULine >> lsoftirq_time;
  aCPULine >> lsteal_time;
  aCPULine >> lguest_time;
  aCPULine >> lguest_nice_time;

  // Remove guest time already in user_time:
  luser_mode_time -= lguest_time;
  lnice_time -= lguest_nice_time;

  // Compute cumulative time
  unsigned long long int lidle_all_time=0,
    lsystem_all_time=0, lguest_all_time=0, ltotal_time=0;

  lidle_all_time = lidle_time + liowait_time;
  lsystem_all_time = lsystem_time + lirq_time + lsoftirq_time;
  lguest_all_time = lguest_time + lguest_nice_time;
  ltotal_time = luser_mode_time + lnice_time + lsystem_all_time +
    lidle_all_time + lsteal_time + lguest_all_time;


  // Update periodic computation.
  user_mode_period_  = computePeriod(luser_mode_time, user_mode_time_);
  nice_period_       = computePeriod(lnice_time, nice_time_);
  system_period_     = computePeriod(lsystem_time, system_time_);
  system_all_period_ = computePeriod(lsystem_all_time,system_all_time_);
  idle_period_       = computePeriod(lidle_time, idle_time_);
  idle_all_period_   = computePeriod(lidle_all_time, idle_all_time_);
  iowait_period_     = computePeriod(liowait_time, idle_time_);
  irq_period_        = computePeriod(lirq_time, irq_time_);
  softirq_period_    = computePeriod(lsoftirq_time, softirq_time_);
  steal_period_      = computePeriod(lsteal_time, steal_time_);
  guest_period_      = computePeriod(lguest_all_time, guest_time_);
  total_period_      = computePeriod(ltotal_time, total_time_);

  /// Update time.
  user_mode_time_  = luser_mode_time;
  nice_time_       = lnice_time;
  system_time_     = lsystem_time;
  system_all_time_ = lsystem_all_time;
  idle_time_       = lidle_time;
  idle_all_time_   = lidle_all_time;
  iowait_time_     = liowait_time;
  irq_time_        = lirq_time;
  softirq_time_    = lsoftirq_time;
  steal_time_      = lsteal_time;
  guest_time_      = lguest_all_time;
  total_time_      = ltotal_time;

  if (total_period_!=0.0)
    {
      percent_ = (double)(user_mode_period_)/(double)(total_period_) * 100.0;
      percent_ += (double)( nice_period_)/(double)(total_period_) * 100.0;
      percent_ += (double)( system_period_)/(double)(total_period_) * 100.0;
      percent_ += (double)( irq_period_)/(double)(total_period_) * 100.0;
      percent_ += (double)( softirq_period_)/(double)(total_period_) * 100.0;
      percent_ += (double)( steal_period_)/(double)(total_period_) * 100.0;
      percent_ += (double)( iowait_period_)/(double)(total_period_) * 100.0;
    }
  std::cout << cpu_id_ << " " << percent_ << std::endl;  
}

System::System()
{
  vCPUData_.clear();
  init();
}

void System::init()
{
  init_ = false;
  readProcStat();
  init_ = true;
}

void System::ProcessCPULine(unsigned int cpunb,
			    std::istringstream &aCPULine)
{
  vCPUData_[cpunb].ProcessLine(aCPULine);
}

void System::readProcStat()
{
  std::ifstream aif;
  cpuNb_ = 1;
  
  aif.open("/proc/stat",std::ifstream::in);
  std::string aline;
  aline.clear();
  while (std::getline(aif,aline))
    {
      // Read on line of the file
      std::istringstream anISSLine(aline);
      std::string line_hdr;
      anISSLine >> line_hdr;

      // Check if the line start with cpu
      std::size_t pos = line_hdr.find("cpu");
      std::string str_cpunbr = line_hdr.substr(pos+3);

      // Check if this is the  first line
      if (pos==0 and str_cpunbr.empty())
	{
	  gCPUData_.ProcessLine(anISSLine);
	  gCPUData_.cpu_id_=-1;
	}
      else
	{
	  // If not then check if there is a CPU number
	  if (pos==0)
	    {
	      std::istringstream iss(str_cpunbr);
	      unsigned int lcpunb;
	      iss >> lcpunb;
	      // If we did not initialize
	      if (!init_)
		{
		  // Count the number of CPU.
		  if (lcpunb>cpuNb_)
		    cpuNb_ = lcpunb;
		}
	      else
		// Otherwise process the line.
		ProcessCPULine(lcpunb,anISSLine);
	    }
	}
    }

  if (!init_)
    {
      /// The number of CPU has been detected by going through /proc/stat.
      vCPUData_.resize(cpuNb_+1);
      for(int i=0;i<(int)cpuNb_;i++)
	vCPUData_[i].cpu_id_ = i;
    }
  aif.close();
}
