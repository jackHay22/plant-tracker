/*
 * (C) 2020 Jack Hay
 */

#include "server.h"
#include "monitor.h"
#include "endpts"
#include <memory>
#include <served/served.hpp>
#include <boost/algorithm/string.hpp>

#define LOG_DOMAIN "plant_tracker::server"
#define APPLICATION_JSON "application/json"
#define CONTENT_TYPE "Content-Type"
#define SERVER "Server"
#define SERVER_NAME "plant-tracker"

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
                    const std::string& api_stub) {
    //log configuration
    monitor::log_info("starting server on port: " + std::to_string(port),
                      LOG_DOMAIN);

    //create publisher
    std::unique_ptr<publisher::publisher_t> prom_publisher =
      std::make_unique<publisher::publisher_t>(prom_port);

    //create the soil moisture family
    prom_publisher.mk_gauge_family("soil_moisture",
                                   "tracks soil moisture for different plants",
                                   {{}});

    //create a new multiplexer
    served::multiplexer multiplexer;

    /*
     * Register endpoints
     */
    MULTIPLEXER.handle(api_stub + "/push").post(
     [&prom_publisher]
       (served::response& res,
        const served::request& req) {
      //check content type of request
      if (!boost::iequals(req.header(CONTENT_TYPE),APPLICATION_JSON)) {
        served::response::stock_reply(400, res);
        return;
      }

      //attempt to parse body
      json_t body;
      try {
        body = json_t::parse(req.body());
      } catch (...) {
        served::response::stock_reply(400, res);
        return;
      }

      //set the server name, call implementation
      res.set_header(SERVER,SERVER_NAME);
      res.set_status(endpts::ingest_data(*prom_publisher,body));
    });


    //create a new server
    served::net::server server("0.0.0.0",
                               std::to_string(port),
                               multiplexer);
    server->run(workers);
  }
}}
