#include "read_write.h"

int ReadWrite::ReadAll(int fd, void* buf, size_t buf_size) {
  int read_size;
  unsigned int read_total = 0;

  while (read_total < buf_size) {
    read_size = read(fd, (char*)buf + read_total, buf_size - read_total);

    if (read_size == 0)
      break;
    else if (read_size == -1)
      return -1;

    read_total += read_size;
  }

  return read_total;
}

int ReadWrite::WriteAll(int fd, void* buf, size_t buf_size) {
  int write_size;
  unsigned int write_total = 0;

  while (write_total < buf_size) {
    write_size = write(fd, (char*)buf + write_total, buf_size - write_total);

    if (write_size == 0)
      break;
    else if (write_size == -1)
      return -1;

    write_total += write_size;
  }

  return write_total;
}
