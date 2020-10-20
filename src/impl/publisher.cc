/*
 * (C) 2020 Jack Hay
 */

#include "publisher.h"
#include "monitor.h"

#define LOG_DOMAIN "plant_tracker::publisher"

namespace plant_tracker {
namespace publisher {

  /**
   * Construct a new group record
   * @param family the gauge family
   */
  publisher_t::gauge_grp_t::gauge_grp_t(prometheus::Family<prometheus::Gauge> *family)
    : family(family),
      gauges(),
      lock() {}

  /**
   * Construct the publisher
   * @param port the port to accept prometheus scrape requests on
   */
  publisher_t::publisher_t(int port)
    : gauge_families(),
      gauge_families_lock() {
    const std::string bind_addr = "0.0.0.0:" + std::to_string(port);

    monitor::log_info("serving prometheus scrape requests: " + bind_addr,LOG_DOMAIN);

    registry = std::make_shared<prometheus::Registry>();
    exposer = std::make_unique<prometheus::Exposer>(bind_addr);

    //register the registry
    exposer->RegisterCollectable(registry);
  }

  /**
   * Create a new gauge family
   * @param family_name the name of the family
   * @param help        a description of the family
   * @param labels      the labels for this gauge family
   * @param whether the family was created
   */
  bool publisher_t::mk_gauge_family(const std::string& family_name,
                                    const std::string& help,
                                    const std::map<std::string, std::string>& labels) {
    //check if the family already exists
    if (!gauge_family_exists(family_name)) {
      //write lock family lookup
      std::unique_lock<std::shared_mutex> unique_lookup_lock(gauge_families_lock);

      //create a new family
      gauge_families[family_name] = std::make_unique<gauge_grp_t>(
        &prometheus::BuildGauge()
         .Name(family_name)
         .Help(help)
         .Labels(labels)
         .Register(*registry)
      );
      return true;
    }
    return false;
  }

  /**
   * Make a gauge in some family (creates family if doesn't exist)
   * @param family_name the name of the family
   * @param gauge_name  the name of the gauge
   * @param labels      labels for this gauge
   * @return whether the gauge was created
   */
  bool publisher_t::mk_gauge(const std::string& family_name,
                             const std::string& gauge_name,
                             const std::map<std::string,std::string>& labels) {
    //check if the family exists
    if (!gauge_family_exists(family_name)) {
      monitor::log_info("automatically creating family: " + family_name,LOG_DOMAIN);
      mk_gauge_family(family_name,"<automatically generated>",labels);

    } else if (gauge_exists(family_name,gauge_name)) {
      //gauge already exists
      return false;
    }

    //get a read lock on the lookup
    std::shared_lock<std::shared_mutex> shared_lookup_lock(gauge_families_lock);

    //look for the family
    std::unordered_map<std::string,std::unique_ptr<gauge_grp_t>>::iterator it
          = gauge_families.find(family_name);

    if (it != gauge_families.end()) {
      monitor::log_info("creating gauge: " + gauge_name,LOG_DOMAIN);

      //get a unique lock on the family lookup
      std::unique_lock<std::shared_mutex> unique_family_lock(it->second->lock);

      //create a new gauge
      it->second->gauges.insert(
        std::pair<std::string,prometheus::Gauge*>(
            gauge_name,
            &it->second->family->Add(labels)));
    }
    return true;
  }

  /**
   * Check if a given family exists by name
   * @param  family_name the name of the family
   * @return             whether the family was found
   */
  bool publisher_t::gauge_family_exists(const std::string& family_name) {
    //read lock family lookup
    std::shared_lock<std::shared_mutex> shared_lookup_lock(gauge_families_lock);
    return (gauge_families.find(family_name) != gauge_families.end());
  }

  /**
   * Check if a gauge exists in some family
   * @param  family_name the name of the family
   * @param  gauge_name  the name of the gauge to look for
   * @return             whether the gauge was found in this family
   */
  bool publisher_t::gauge_exists(const std::string& family_name,
                                 const std::string& gauge_name) {
    //read lock family lookup
    std::shared_lock<std::shared_mutex> shared_lookup_lock(gauge_families_lock);

    //search for the family in the lookup
    std::unordered_map<std::string,std::unique_ptr<gauge_grp_t>>::iterator it
          = gauge_families.find(family_name);

    if (it != gauge_families.end()) {
      //get a read lock on the family
      std::shared_lock<std::shared_mutex> shared_family_lock(it->second->lock);
      return (it->second->gauges.find(gauge_name) != it->second->gauges.end());
    }
    return false;
  }

  /**
   * Set the value of some gauge (must exist)
   * @param  family_name the name of the gauge family
   * @param  gauge_name  the name of the gauge
   * @param  value       the value to set
   * @return             whether the gauge was set correctly
   */
  bool publisher_t::set_gauge(const std::string& family_name,
                              const std::string& gauge_name,
                              double value) {
    //get a shared lock on the family lookup
    std::shared_lock<std::shared_mutex> shared_lookup_lock(gauge_families_lock);

    //look for family
    std::unordered_map<std::string,std::unique_ptr<gauge_grp_t>>::iterator it
          = gauge_families.find(family_name);

    if (it != gauge_families.end()) {
      //get a unique lock on the family
      std::unique_lock<std::shared_mutex> unique_family_lock(it->second->lock);

      //look for the gauge
      std::unordered_map<std::string,prometheus::Gauge*>::iterator it_f
            = it->second->gauges.find(gauge_name);

      if (it_f != it->second->gauges.end()) {
        //set the value
        it_f->second->Set(value);
        return true;
      }
    }

    return false;
  }

}}
