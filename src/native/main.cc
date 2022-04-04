// Entrypoint for PhabNative, this justs runs a http server. 
// gRPC Servers will be registered here.
//
#include <cstdint>
#include <functional>
#include <string>
#include <string_view>

struct Options {
    // Enable debug endpoints.
    bool debug;
    // Current host, when empty localhost:5554
    std::optional<std::string> host;
    // Called when the server shutdown is started.
    // Must be threadsafe, as it can be called from any thread.
    std::function<void(const phab::Server::Context&) on_shutdown;
    // Number of background threads to use. Only used for debugging.
    size_t max_concurrency;
};

// This is just starting the gRPC Server, on which Phabricator (in PHP) runs.
// the Server can respond to healthchecks and other stats via url rewriting.
int main(int,char**) {
    Options options;
    options.debug = true;
    options.host = std::nullopt; 

    // phab::Server server = phab::Server::Create(std::move(options));
    // auto& ctx = server.Context();
    // ctx.SetMetricRetention(kOneDay);
    // server.Register("/healthz", [](auto& context,auto* response) {
    //   auto is_http = context.request().IsHttp();
    //   auto is_text = context.request().ContentType() == "application/text"; 
    //   auto metric = histogram::Metric("phab.core.HealthCheck");
    //   CHECK(context.Deadline() <= phab::base::Seconds(5));
    //   if (is_http) {
    //      LOG(VERBOSE) << "received http healthcheck";
    //      std::string content = "HTTP 1/1";
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
    //  phab::common::Stats all_stats = phab::common::Stats::Create(
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
    // return server.Run();    
}
