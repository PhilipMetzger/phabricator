// Author: Philip Metzger
// SPDX-License-Identifier: Apache-2.0
#ifndef SRC_NATIVE_NET_SOCKET_H_
#define SRC_NATIVE_NET_SOCKET_H_

#include <cstdint>
#include <net/inet.h>

#include "absl/status/status.h"

namespace phab::net {
// A better Socket abstracts over the classical Unix interface.
// Usually passing a IOBuffer is needed to read and write data.
//
// Only used in net::Server.
class Socket {
 public:
   static Socket Open(int fd);
   ~Socket();
   // Read from the Socket up to `size` into the `buffer`
   int32_t Read(IOBuffer& buffer, size_t size);
   //  
   int32_t Write(IOBuffer& buffer, size_t size);
   // Bind the Socket to the passed File Descriptor.
   absl::Status Bind(int32_t fd);
   // Make the Socket blocking, by default off.
   void SetBlocking();
   void set_nagle(bool on) { nagle_ = on; }
   bool nagle() const { return nagle_; }
 private:
   Socket(int fd) : fd_(fd) {}
   // Underlying file descriptor.
   int fd_;
   // Is the Nagle algorithm on. 
   // See Julia Evans post and Rachel's post.
   bool nagle_ = false;

};

} // phab::net
#endif // SRC_NATIVE_NET_SOCKET_H_
