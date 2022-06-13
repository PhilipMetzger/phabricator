// Author: Philip Metzger
// SPDX-License-Identifier: Apache-2.0
#ifndef SRC_NATIVE_NET_SOCKET_H_
#define SRC_NATIVE_NET_SOCKET_H_

#include <cstdint>
#include <net/inet.h>

#include "absl/status/status.h"
#include "absl/status/statusor.h"

#include "src/native/net/io_buffer.h"

namespace phab::net {
// A better Socket abstracts over the classical Unix interface.
// A IOBuffer is needed to read and write data.
// See io_buffer.h for the interface.
//
// Only used in net::Server.
// These API's are synchronous, but upper layers queue `write()` and `read()`
// with the `ThreadPool::PostJob()` API's.
//
// Example: 
// using phab::net::Socket;
// bool WriteToAddress(std::string_view ip_address, ByteBuffer buffer) {
//   auto sock = Socket::Create(myfd);
//   std::string my_string;
//   auto buff = IOBuffer::Create(std::move(my_string));
//   while(buff.size() > written) {
//     if (auto nwritten = sock.Write(buff,100); nwritten.ok()) { 
//         written += *nwritten;
//      } else {
//        switch(nwritten.code()) { 
//        //... 
//        }
//      }
//   }
//   return true;
// }
class Socket {
 public:   
   // Create a Socket for `fd`. 
   static Socket Create(int fd);
   ~Socket();

   // Read from the Socket up to `size` into the `buffer`.
   absl::StatusOr<int32_t> Read(IOBuffer& buffer, size_t size);

   // Try to read the whole sk_sockbuff into `buffer`.
   absl::StatusOr<int32_t> ReadAll(IOBuffer& buffer);

   // Write up to `size` into the socket.   
   absl::StatusOr<int32_t> Write(IOBuffer& buffer, size_t size);

   // Try to write all of `buffer` into the socket.
   absl::StatusOr<int32_t> WriteAll(IOBuffer& buffer);

   // Bind the Socket to the passed File Descriptor.
   absl::Status Bind(int32_t fd);

   // Make the Socket blocking, by default off.
   void SetBlocking();

   // Enable the Nagle algorithm. 
   void set_nagle(bool on) { nagle_ = on; }

   // Is the Nagle algorithm enabled.
   bool nagle() const { return nagle_; }

 private:
   // TODO: Use StatusOr<Socket>;
   // The invalid Socket.
   static constexpr int kInvalidSocket = -1;

   Socket(int fd) : fd_(fd) {}

   // Is the socket useable for network communication.
   // Must only be used in `CHECK()` calls.
   bool valid() const { return fd_ != kInvalidSocket; }

   // Underlying file descriptor.
   int fd_;
   // Is the Nagle algorithm on. 
   // See Julia Evans post and Rachel's post.
   bool nagle_ = false;

};

class ServerSocket : public Socket {
 public: 
  // Create a ServerSocket.
  using Socket::Create;

  // Write to a ServerSocket.
  using Socket::Write;

  // Read from a ServerSocket.
  using Socket::Read;

  absl::Status Listen(int backlog);
 private:

}
} // phab::net
#endif // SRC_NATIVE_NET_SOCKET_H_
