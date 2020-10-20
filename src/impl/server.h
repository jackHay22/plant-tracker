/*
 * (C) 2020 Jack Hay
 */

#ifndef _PLANT_TRACKER_SERVER_H
#define _PLANT_TRACKER_SERVER_H

#include <string>

namespace plant_tracker {
namespace server {

  /**
   * Serve new connections
   * @param port     the port to serve connections on
   * @param prom_port the port to serve prometheus requests on
   * @param workers  thread pool size
   * @param api_stub the prefix for regular endpt paths
   */
  void serve_endpts(int port,
                    int prom_port,
                    int workers,
                    const std::string& api_stub);

}}

#endif /*_PLANT_TRACKER_SERVER_H*/
