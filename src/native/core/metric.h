// Author: Philip Metzger
// SPDX-License-Identifier: Apache-2.0
#ifndef SRC_NATIVE_CORE_METRIC_H_
#define SRC_NATIVE_CORE_METRIC_H_

#include <atomic>
#include <string_view>

namespace phab::common {

// TODO: Add Histogram, Counter and Gauge.
// A Metric is a named counter, this is used to query the API usage of the 
// different components. 
//
// All Metric's must be namespaced. E.G `Metric("phab.core.Healthcheck")`
class Metric {
 public:
  // Record a Metric with the given `name`
  Metric Metric(std::string_view name);

  ~Metric() = default;

  Metric(const Metric&) = delete;
  Metric operator=(const Metric&) = delete;
  Metric(Metric&&) = delete;
  Metric operator=(Metric&&) = delete;

 private:
  friend class MetricManager;

  std::atomic<int64_t> counter_;
};

}


#endif
