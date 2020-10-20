/*
 * (C) 2020 Jack Hay
 */

#include <string>
#include <unistd.h>
#include "impl/server.h"

using namespace plant_tracker;

/**
 * Entrypoint
 */
int main(int argc, char *argv[]) {
  int c;
  int port = 8000;
  int prom_port = 8088;
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

  //serve endpoints
  server::serve_endpts(port,prom_port,workers,api_stub);
  return 1;
}
