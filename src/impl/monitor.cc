/*
 * (C) 2020 Jack Hay
 */

#include "monitor.h"
#include <stdio.h>
#include <ctime>

namespace plant_tracker {
namespace monitor {

  /*
   * Get the unix epoch timestamp in seconds
   */
  int seconds_since_epoch() {
    std::time_t result = std::time(nullptr);
    std::asctime(std::localtime(&result));
    return result;
  }


  /**
   * Write an info log message
   * @param msg the message
   * @param domain the domain for the message (namespace)
   */
  void log_info(const std::string& msg,
                const std::string& domain) {
    fprintf(stderr,"lvl=INFO msg=\"%s\" domain=%s time=%d\n",
            msg.c_str(),
            domain.c_str(),
            seconds_since_epoch());
  }

  /**
   * Write an error message
   * @param msg    the message
   * @param domain the namespace the error occured in
   */
  void log_err(const std::string& msg,
               const std::string& domain) {
    fprintf(stderr,"lvl=ERR msg=\"%s\" domain=%s time=%d\n",
            msg.c_str(),
            domain.c_str(),
            seconds_since_epoch());
  }
}}
