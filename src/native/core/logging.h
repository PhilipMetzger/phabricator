// Author: Philip Metzger
// SPDX-License-Identifier: Apache-2.0
#ifndef SRC_NATIVE_CORE_LOGGING_H_
#define SRC_NATIVE_CORE_LOGGING_H_

// Glog like logging. 
namespace logging {
namespace detail {
class ThinStream : public std::ostream {};

}

#define LOG(level) ::logging::LogMessage(level)
enum class Level {
  kInfo,
  kVerbose,
  kError,

};

}

#endif
