// Author: Philip Metzger
// SPDX-License-Identifier: Apache-2.0

#include "src/native/core/metric.h"

#include <atomic>
#include <string_view>

#include "src/native/core/check.h"

#include "absl/containers/flat_hash_set.h"

namespace phab::common {
namespace {

// A singleton which manages all recorded metrics.
class MetricManager {
 public:
  // Get's or Creates the Instance.
  static MetricManager& GetInstance();
  // Register's a new Metric. Fails if `metric` already exists.
  bool Register(const Metric& metric);
  // Check if the metric with the `name` already exists.
  bool Exists(std::string_view name) const;
  // Find the `metric`. 
  Metric& Find(const Metric& metric);

  void Add(const Metric& metric);
 private:
  MetricManager() = default;
  ~MetricManager() = default;
  // All recorded Metrics during the lifetime of the program.
  absl::flat_hash_set<Metric> metrics_;
};

MetricManager* g_manager = nullptr;

MetricManager& MetricManager::GetInstance() {
  if (!g_manager)
      g_manager = new MetricManager();
  CHECK(g_manager)
  return *g_manager;
}

bool MetricManager::Register(const Metric& metric) {
  if (metrics_.find(metric))
      return false;

  metrics_.insert(std::move(metric));
  return true;
}

bool MetricManager::Exists(std::string_view name) {
  

}

bool Exists(std::string_view name) {
  return MetricManager::GetInstance().Exists(name);
}

void Register(Metric* metric) {
  CHECK(metric);
  MetricManager::GetInstance().Register(*metric);
}
}

Metric Metric(std::string_view name) {
  if (!Exists(name))
      Register(this);
  MetricManager::GetInstance().Add(*this);
}


}
