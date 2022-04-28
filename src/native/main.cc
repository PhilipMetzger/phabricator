// Author: Philip Metzger
// SPDX-License-Identifier: Apache-2.0
//
// Entrypoint for PhabNative, this justs runs a http server. 
// gRPC Servers will be registered here.
//
#include <cstdint>
#include <functional>
#include <string>
#include <string_view>
#include <thread>

#include "absl/time/time.h"

#include "src/native/core/logging.h"
#include "src/native/core/check.h"
#include "src/native/server.h"

static constexpr auto kOneDay = absl::Day(1);

// This is just starting the gRPC Server, on which Phabricator (in PHP) runs.
// the Server can respond to healthchecks and other stats via url rewriting.
int main(int,char**) {
    phab::Server::Options options;
    options.debug = true;
    options.on_shutdown = [](auto& context) {
      LOG(INFO) << "Shutdown called on" << std::thread::current_id();
      CHECK(context.Metrics().MetricProto());
      phab::base::WriteTextProto("end_of_run",
              context.Metrics().MetricProto());

    };
    options.host = std::nullopt; 

    phab::Server server = phab::Server::Create(std::move(options));
    auto& ctx = server.Context();
    ctx.SetMetricRetention(kOneDay);
    ctx.SetCrashHandler(absl::bind_front(Server::Crash,&server));

    // TODO(T1): Move to debug endpoints to server.
    // TODO: Move these into //net/grpc/base/.
    // 
    // // Stubby ships these endpoints out of the box.
    // server.Register("/flagz", [](auto& context,auto* response) {
    // // TODO: Enumerate enviroment and commandline for flags and print their 
    // // state.
    // }
    // server.Register("/streamz", [](auto& context,auto* response) {});
    // server.Register("/varz", [](auto& context,auto* response) {});
    // server.Register("/healthz", [](auto& context,auto* response) {
    //   auto is_http = context.request().IsHttp();
    //   auto is_text = context.request().ContentType() == "application/text"; 
    //   auto metric = histogram::Metric("phab.core.HealthCheck");
    //   CHECK_LE(context.Deadline(),absl::Second(5));
    //   if (is_http) {
    //      LOG(VERBOSE) << "received http healthcheck";
    //      std::string content = net::DefaultHeader(); 
    //      for (const auto& server : context.RegisteredServices()) {
    //          auto rpc_metric = histogram::Metric(
    //              absl::StrFormat("phab.$1.HealthCheck",server.name());
    //          absl::StrAppend(&content, "Server: ", server.name(),"Health:",
    //          server.health() ? "ok" : "unhealthy","\n");
    //
    //      }
    //      response->Write(content);
    //      return;
    //   }
    //   phab::common::healthcheck healthpb = MakeHealthProto(context);
    //   response->Write(healthpb);
    // });
    // server.Register("/statz",[](auto& context,auto* response) {
    //  auto is_http = context.request().IsHttp();
    //  phab::common::Stats all_stats = common::Stats::Create(
    //      context.stats().file());
    //  if (is_http) {
    //      for (auto& req_in : all_stats.Requests())
    //
    //      return;
    //  }
    //  response->Write(all_stats);
    //
    // });
    // server.Register("/metricz",[](auto& context,auto* response) {
    //  auto is_http = context.request().IsHttp(); 
    //  if (is_http) { 
    //    response->Write(context.metric().File());
    //    return;
    //  }
    //  auto metrics_pb = metric::FromJson(context.metric().File());
    //  response->Write(metrics_pb); 
    // });
    // // Tries to imitate Stubby's /rpcz, for rpc stats.
    // // Trillian has a matching example.
    // server.Register("/rpcz",[](auto& context,auto* response) {
    //  auto is_http = context().request().IsHttp();
    //  auto& metric = context.Metrics();
    //  std::vector<common::Metric> metrics = metric.All();
    //  // Sort the rpcs, from most active 
    //
    //  if (is_http) {
    //    std::string content = net::DefaultHeader();
    //    if (context().request().Header().is_text()) {
    //      absl::StrAppend(&content,"Stats for this server.");
    //      for (const auto& server_data : metricpb.servers()) {
    //
    //
    //      }
    //    }
    //
    //  }
    //  CHECK(context.request().IsRPC());
    //  // Handle rpc GetRpcz(google.protobuf.Empty) returns (Rpcz)
    // }
    
    // Start the http server on the main thread.
    return server.Run();    
}
