/* Copyright LAAS, CNRS
 * Author: O. Stasse, 2019
 * See LICENSE file in the root directory of this repository.
 */

#ifndef DYNAMIC_GRAPH_PROCESS_LIST_H_
#define DYNAMIC_GRAPH_PROCESS_LIST_H

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>

#include <dynamic-graph/dynamic-graph-api.h>
#include <dynamic-graph/fwd.hh>

namespace dynamicgraph {
namespace CPU {
class DYNAMIC_GRAPH_DLLAPI ProcessData {};

class DYNAMIC_GRAPH_DLLAPI ProcessList {
public:
  ProcessList();
};

/// This class gather information on a specific CPU.
///
class DYNAMIC_GRAPH_DLLAPI CPUData {
public:
  CPUData();
  int cpu_id_;

  inline unsigned long long int computePeriod(unsigned long long int &a,
                                              unsigned long long int &b) {
    return (a > b) ? a - b : 0;
  }

  /// \brief Various classes of time spend by the CPU
  /// @{
  /// Total time
  unsigned long long int total_time_;
  /// Time spend in user mode
  unsigned long long int user_mode_time_;
  /// Time spend in user mode with low priority (nice mode)
  unsigned long long int nice_time_;
  /// Time spend in system mode
  unsigned long long int system_time_;
  /// Time spend in system mode
  unsigned long long int system_all_time_;
  /// Time spend in doing nothing.
  unsigned long long int idle_time_;
  /// Time spend in doing nothing.
  unsigned long long int idle_all_time_;
  /// Time spend in waiting an input/output to complete.
  unsigned long long int iowait_time_;
  /// Time spend in servicing hardware interrupts.
  unsigned long long int irq_time_;
  /// Time spend in servicing software interrupts.
  unsigned long long int softirq_time_;
  /// Time spend in other operating systems in a virtualized environments
  /// Never doing this for control !
  unsigned long long int steal_time_;
  /// Time spend running a virtual CPU for guest operating systems
  /// under the control of the Linux kernel
  unsigned long long int guest_time_;
  /// Time spent running a niced guest
  /// (virtual CPU for guest operating systems under the
  /// control of the Linux kernel)
  unsigned long long int guest_nice_time_;
  /// @}

  /// \brief Various classes of time spend by the CPU by period
  /// @{
  /// Total time
  unsigned long long int total_period_;
  /// Time spend in user mode
  unsigned long long int user_mode_period_;
  /// Time spend in user mode with low priority (nice mode)
  unsigned long long int nice_period_;
  /// Time spend in system mode
  unsigned long long int system_period_;
  /// Time spend in all system mode
  unsigned long long int system_all_period_;
  /// Time spend in doing nothing.
  unsigned long long int idle_period_;
  /// Time spend in doing nothing.
  unsigned long long int idle_all_period_;
  /// Time spend in waiting an input/output to complete.
  unsigned long long int iowait_period_;
  /// Time spend in servicing hardware interrupts.
  unsigned long long int irq_period_;
  /// Time spend in servicing software interrupts.
  unsigned long long int softirq_period_;
  /// Time spend in other operating systems in a virtualized environments
  /// Never doing this for control !
  unsigned long long int steal_period_;
  /// Time spend running a virtual CPU for guest operating systems
  /// under the control of the Linux kernel
  unsigned long long int guest_period_;
  /// @}

  double percent_;
  void ProcessLine(std::istringstream &aCPULine);

  friend class boost::serialization::access;

  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    ar &version;
    ar &total_time_;
    ar &user_mode_time_;
    ar &nice_time_;
    ar &system_time_;
    ar &system_all_time_;
    ar &idle_time_;
    ar &idle_all_time_;
    ar &iowait_time_;
    ar &irq_time_;
    ar &softirq_time_;
    ar &steal_time_;
    ar &guest_time_;
    ar &guest_nice_time_;
    ar &percent_;
  }
};

/// This class gathers information on a computer.
/// This includes a list of CPU
class DYNAMIC_GRAPH_DLLAPI System {
private:
  bool init_;

public:
  System();

  /// Read /proc/state file to extract CPU count.
  void init();

  /// Update CPU data information from /proc/stat
  void readProcStat();

  /// Friend class for serialization.
  friend class boost::serialization::access;

  /// Number of CPU.
  unsigned int cpuNb_;

  void ProcessCPULine(unsigned int cpunb, std::istringstream &aCPULine);

  /// \brief Vector of CPU informations.
  std::vector<CPUData> vCPUData_;

  /// \brief Global CPU information.
  CPUData gCPUData_;

  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    ar &version;
    ar &cpuNb_;
    ar &gCPUData_;
    ar &vCPUData_;
  }
};
} // namespace CPU
} // namespace dynamicgraph

#endif /* DYNAMIC_GRAPH_PROCESS_LIST_H_ */
