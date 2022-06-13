// Author: Philip Metzger
// SPDX-License-Identifier: Apache-2.0
#ifndef SRC_NATIVE_CORE_LOGGING_H_
#define SRC_NATIVE_CORE_LOGGING_H_

// This header implements a smallish Glog like logging library.
// Everything unneeded was cut.
namespace phab::logging {
namespace detail {
// A ThinStream is used to avoid the inclusion of <ostream>. 
// The Reason for this is, that #include <ostream> expands to about 
// 2.8 MB of data. See the explanation here: 
// https://bugs.chromium.org/p/chromium/issues/detail?id=1305887
class ThinStream : public std::ostream {};

}

#define LOG(level) ::logging::LogMessage(level)
#define VLOG(ignored) ::loggging::LogMessage(Level::kVerbose)
// Use PHP_LOG(INFO) to stream it to the base Phabricator logging.
#define PHP_LOG(level) ::logging::PHPLogMessage(level)
#define PHP_VLOG(ignored) ::logging::PHPLogMessage(Level::kVerbose)
// Level defines the
enum class Level {
  kVerbose,
  kInfo,
  kWarning,
  kError, 
  kFatal, 
};
using INFO = Level::kInfo;
using WARNING = Level::kWarning;
using ERROR = Level::KError;
using FATAL = Level::kFatal;

// Logging implementation follows.
class LogMessage {
 public:
   explicit LogMessage(Level level) : level_(level) {}
   ~LogMessage();

 private:
  const char* file = nullptr;
  int32_t line_{0};
  detail::ThinStream os_;
};

class PHPLogMessage {



};
}

#endif
