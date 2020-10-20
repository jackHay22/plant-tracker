/*
 * (C) 2020 Jack Hay
 */

#include "endpts.h"
#include "monitor.h"

#define VALUE_KEY "value"
#define NAME_KEY "name"
#define FAMILY_KEY "family"
#define LOG_DOMAIN "plant_tracker::endpts"

namespace plant_tracker {
namespace endpts {

  /*
   * Determine if a given json element contains
   * a string with given key
   */
  bool contains_string(const json_t& json, const std::string& key) {
    return json.count(key) && json[key].is_string();
  }

  /*
   * Determine if a given json element contains
   * a number with given key
   */
  bool contains_number(const json_t& json, const std::string& key) {
    return json.count(key) && json[key].is_number();
  }

  /**
   * Ingest new data and add to publisher
   * @param  prom_publisher the publisher to add to
   * @param  body           the body of the request
   * @return            the http status code to return
   */
  int ingest_data(publisher::publisher_t& prom_publisher,
                  const json_t& body) {

    //verify the payload
    if (contains_number(body,VALUE_KEY) &&
        contains_string(body,NAME_KEY) &&
        contains_string(body,FAMILY_KEY)) {
      const std::string name = body[NAME_KEY].get<std::string>();
      const std::string family = body[FAMILY_KEY].get<std::string>();
      double value = body[VALUE_KEY].get<double>();

      //check if the gauge family has been created
      if (!prom_publisher.gauge_family_exists(family)) {
        prom_publisher.mk_gauge_family(family,"tracks " + family,{{family,"TOTAL"}});
      }

      //check if a gauge has been created
      if (!prom_publisher.gauge_exists(family,name)) {
        prom_publisher.mk_gauge(family,name,{{family,name}});
      }

      //set the value of the gauge
      if (prom_publisher.set_gauge(family,name,value)) {
        return 200;
      }

      monitor::log_info("failed to value for: " +
                        name + " in " + family +
                        " family",LOG_DOMAIN);
      return 500;
    }
    monitor::log_info("invalid ingest payload",LOG_DOMAIN);
    return 400;
  }

}}
