/*
 * (C) 2020 Jack Hay
 */

#include "server.h"
#include "monitor.h"
#include "../../libs/nlohmann_json.h"
#include <memory>

#define LOG_DOMAIN "plant_tracker::server"

namespace plant_tracker {
namespace server {

  typedef nlohmann::json json_t;

  /**
   * Serve new connections
   * @param port     the port to serve connections on
   * @param workers  thread pool size
   * @param api_stub the prefix for regular endpt paths
   */
  void serve_endpts(int port, int workers, const std::string& api_stub) {
    //log configuration
    monitor::log_info("starting server on port: " + std::to_string(port),
                      LOG_DOMAIN);
  }
}}
