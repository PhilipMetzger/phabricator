// Author: Philip Metzger
// SPDX-License-Identifier: Apache-2.0
#include "src/native/core/context.h"

namespace phab::core {
namespace {

Metric Create(std::string_view name) 

}

void Context::AddService(std::string_view name, grpc::Server service,
        grpc::Context context) {
  
  main_service_.Register(&service);
  registered_services_.emplace(std::move(name),std::move(service),
          std::move(context));


}


}

