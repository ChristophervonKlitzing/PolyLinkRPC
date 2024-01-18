#ifndef BYTES_H
#define BYTES_H

#include <cstdint>

class Bytes {
 private:
  const char *memory;
  const std::size_t length;

 public:
  Bytes(const char *memory, std::size_t length)
      : memory(memory), length(length) {}

  std::size_t get_length() const { return this->length; }
  const char *get_memory() const { return this->memory; }
};

#endif  // BYTES_H