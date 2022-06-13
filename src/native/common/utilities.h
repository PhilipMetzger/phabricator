// Author: Philip Metzger
// SPDX-License-Identifier: Apache-2.0
#ifndef SRC_COMMON_UTILITIES_H_
#define SRC_COMMON_UTILITIES_H_
#include <string>
#include <vector>

#include "absl/status/status.h"
#include "absl/container/flat_hash_map.h"

#include "src/core/context.h"
#include "src/core/check.h"
#include "src/core/logging.h"
#include "src/core/threadpool.h"
#include "src/core/metric.h"

namespace phab::common {

absl::Status ReadLogo(std::string& output_buffer);

// Calculates the Server Uptime as microseconds since the Epoch.
// Users should format the value, before printing it.
// 
// Example: 
//   using phab::common::CalculateUptime;
//   auto uptime = CalculateUptime();
//   if (uptime <= absl::Hour(2))
//     return UpdateLowRuntime();
//   else
//     return UpdateMediumToLongRuntime();
absl::Duration CalculateUptime();

// Extract the Http Query values, which are passed to `Profiler`.
// This searches for `time`, `duration`, `symbol` or `rpc`.
//
// Example:
//   using phab::common::ExtractProfilezQuery;
//   auto request = server.request();
//   auto pass_to_profiler = ExtractProfilezQuery(request.data());
//   CHECK(pass_to_profiler.find("symbol") != pass_to_profiler.end() &&
//       IsAllowedProfilezArgs(pass_to_profiler); 
std::vector<std::string_view> ExtractProfilezQuery(
        const http::Request& request);

class StatusPage {
 public:
  StatusPage Create();
  ~StatusPage() = default;


  std::string Render() &&;
 private:
  StatusPage(const absl::flat_hash_map<std::string,std::string_view>& substs,
          std::string_view template_prefix) :
      substituitions_(std::move(substs)),
      prefix_(std::move(template_prefix) {}

  // `BasePage()` creates a simple html page.
  // The Page already contains the base substituitions for `Author`, 
  // `created_at` and `title`.
  std::string BasePage() const;

  // `RenderCpuUsage` replaces the `Cpu` div section with `stats` values.
  void RenderCpuUsage(const CpuStats& stats);

  // `RenderMemUsage` fills the Memory subsection of the Page with `mem_stats`
  // values.
  void RenderMemUsage(const MemStats& mem_stats);
  

  // `substituitions` contains all variables, which will be replaced on the 
  // final render.
  absl::flat_hash_map<std::string,std::string_view> substituitions_;
  std::string prefix_;
};

} // namespace phab::common
#endif  // SRC_COMMON_UTILITIES_H_
