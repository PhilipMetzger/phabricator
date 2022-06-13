// Author: Philip Metzger
// SPDX-License-Identifier: Apache-2.0
#ifndef SRC_NATIVE_IO_BUFFER_H_
#define SRC_NATIVE_IO_BUFFER_H_

// This file implements the `IOBuffer` which is the backing datastructure for 
// all network `Read()` and `Write()` calls. 
namespace phab::net {

// IOBuffer
class IOBuffer {
 public:
  IOBuffer Create(std::string backing_store);
  ~IOBUffer();

  const char& data() const { return mem_.data(); }
  char& data() { return mem_.data(); }

 private:

  std::string mem_;
};


}
#endif // SRC_NATIVE_IO_BUFFER_H_
