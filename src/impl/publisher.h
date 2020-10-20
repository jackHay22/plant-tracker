/*
 * (C) 2020 Jack Hay
 */

#ifndef _PLANT_TRACKER_PUBLISHER_H
#define _PLANT_TRACKER_PUBLISHER_H

#include <string>
#include <shared_mutex>
#include <memory>
#include <unordered_map>
#include <prometheus/counter.h>
#include <prometheus/gauge.h>
#include <prometheus/exposer.h>
#include <prometheus/registry.h>

namespace plant_tracker {
namespace publisher {

  /**
   * Class that manages the prometheus client
   * (Thread safe)
   */
  class publisher_t {
  private:
    //defines a gauge family
    struct gauge_grp_t {
      //the gauge family
      prometheus::Family<prometheus::Gauge> *family;
      //name to gauge mapping
      std::unordered_map<std::string,prometheus::Gauge*> gauges;
      //the lock on the gauge lookup
      std::shared_mutex lock;

      gauge_grp_t(prometheus::Family<prometheus::Gauge> *family);
    };

    //name to gauge group mapping
    std::unordered_map<std::string, std::unique_ptr<gauge_grp_t>> gauge_families;

    //the lock on the gauge family lookup
    std::shared_mutex gauge_families_lock;

    //prometheus client members
    std::unique_ptr<prometheus::Registry> registry;
    std::unique_ptr<prometheus::Exposer> exposer;

  public:
    publisher_t(int port);

    publisher_t(const publisher_t& other) = delete;
    publisher_t& operator=(const publisher_t& other) = delete;

    /**
     * Create a new gauge family
     * @param family_name the name of the family
     * @param help        a description of the family
     * @param labels      the labels for this gauge family
     * @return whether the family was created
     */
    bool mk_gauge_family(const std::string& family_name,
                         const std::string& help,
                         const std::map<std::string, std::string>& labels);

    /**
     * Make a gauge in some family (creates family if doesn't exist)
     * @param family_name the name of the family
     * @param gauge_name  the name of the gauge
     * @param labels      labels for this gauge
     * @return whether the gauge was created
     */
    bool mk_gauge(const std::string& family_name,
                  const std::string& gauge_name,
                  const std::map<std::string,std::string>& labels);

    /**
     * Check if a given family exists by name
     * @param  family_name the name of the family
     * @return             whether the family was found
     */
    bool gauge_family_exists(const std::string& family_name);

    /**
     * Check if a gauge exists in some family
     * @param  family_name the name of the family
     * @param  gauge_name  the name of the gauge to look for
     * @return             whether the gauge was found in this family
     */
    bool gauge_exists(const std::string& family_name,
                      const std::string& gauge_name);

    /**
     * Set the value of some gauge (must exist)
     * @param  family_name the name of the gauge family
     * @param  gauge_name  the name of the gauge
     * @param  value       the value to set
     * @return             whether the gauge was set correctly
     */
    bool set_gauge(const std::string& family_name,
                   const std::string& gauge_name,
                   double value);
  };

}}

#endif /*_PLANT_TRACKER_PUBLISHER_H*/
