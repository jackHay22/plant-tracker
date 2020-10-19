/*
 * (C) 2020 Jack Hay
 */

#include <string>
#include <unistd.h>
#include "impl/log.h"
#include "impl/server.h"

using namespace plant_tracker;

#define LOG_DOMAIN "main"

/**
 * Entrypoint
 */
int main(int argc, char *argv[]) {
  int c;
  int port = 8000;
  std::string api_stub = "/api/v1";
  int workers = 5;

  while ((c = getopt(argc, argv, "p:s:")) != -1) {
    if (c == 'p') {
      //parse server port
      port = atoi(optarg);
    } else if (c == 's') {
      api_stub = optarg;
    }
  }

  //log configuration
  monitor::log_info("starting server on port: " +
                    std::to_string(port) +
                    " with endpt path: " + api_stub,
                    LOG_DOMAIN);

  server::serve_endpts(port,api_stub);
  return 1;
}
