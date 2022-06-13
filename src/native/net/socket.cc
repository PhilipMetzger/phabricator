// Author: Philip Metzger
// SPDX-License-Identifier: Apache-2.0
#include "src/net/socket.h"

#include <net/inet.h>

#include "absl/status/status.h"
#include "absl/status/statusor.h"

#include "src/native/net/io_buffer.h"
namespace phab::net {
namespace {

// A wrapper to allow the usage of `send(1)` instead of `write(1)`.
// See the man page for the details, why send and sendfile are more efficient. 
int32_t Write(int sock, void* data, int size) {
#if defined(HAS_SEND)
    return ::send(sock,data,size);
#else 
    return ::write(sock,data,size);
#endif
}

}

Socket::~Socket() {
  int res = ::close(fd_,0);
  CHECK_EQ(res,0) << "Failed to close socket";
} 

Socket Socket::Open(int fd) {
  CHECK_GT(0,fd) << "Passed an invalid file descriptor";
  int sock = ::open(fd);
  return Socket(std::move(sock)); 
}

absl::StatusOr<int32_t> Socket::Write(IOBuffer& buffer, size_t size) {
  int32_t written = 0;
  if (written = Write(bound_fd_,buffer.data(),size); written == -1) {
      return absl::ErrnoToStatus(errno);
  }
  return written;
}

absl::StatusOr<int32_t> Socket::Read(IOBuffer& buffer, size_t size) { 
  int32_t nread = 0;
  if (nread = ::read(fd_,buffer.data(),size); nread == -1) {
    return absl::ErrnoToStatus(errno);
  }
  return nread;
}

absl::StatusOr<int32_t> Socket::ReadAll(IOBuffer& buffer) {
  int32_t all_read = 0;
  do {
    auto read = Read(buffer,remaining_size);
    all_read += *read;

  } while(read.ok());

  return all_read;
}
}
