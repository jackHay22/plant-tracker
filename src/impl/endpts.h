/*
 * (C) 2020 Jack Hay
 */

#ifndef _PLANT_TRACKER_ENDPTS_H
#define _PLANT_TRACKER_ENDPTS_H

#include <string>
#include "publisher.h"
#include "../../libs/nlohmann_json.h"

namespace plant_tracker {

typedef nlohmann::json json_t;

namespace endpts {

  /**
   * Ingest new data and add to publisher
   * @param  prom_publisher the publisher to add to
   * @param  body           the body of the request
   * @return            the http status code to return
   */
  int ingest_data(publisher::publisher_t& prom_publisher,
                  const json_t& body);

}}

#endif /*_PLANT_TRACKER_ENDPTS_H*/
