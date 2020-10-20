/*
 * (C) 2020 Jack Hay
 */

#ifndef _PLANT_TRACKER_MONITOR_H
#define _PLANT_TRACKER_MONITOR_H

#include <string>

namespace plant_tracker {
namespace monitor {

  /**
   * Write an info log message
   * @param msg the message
   * @param domain the domain for the message (namespace)
   */
  void log_info(const std::string& msg,
                const std::string& domain);

  /**
   * Write an error message
   * @param msg    the message
   * @param domain the namespace the error occured in
   */
  void log_err(const std::string& msg,
               const std::string& domain);

}}

#endif /*_PLANT_TRACKER_MONITOR_H*/
