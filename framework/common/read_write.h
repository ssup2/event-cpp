#ifndef READ_WRITE_H_
#define READ_WRITE_H_

#include <unistd.h>

class ReadWrite {
 public:
  static int ReadAll(int fd, void* buf, size_t buf_size);
  static int WriteAll(int fd, void* buf, size_t buf_size);
};

#endif  // READ_WRITE_H
