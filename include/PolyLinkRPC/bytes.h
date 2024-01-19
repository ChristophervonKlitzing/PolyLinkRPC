#ifndef BYTES_H
#define BYTES_H

#include <cstdint>

class BytesReference {
 private:
  const char *memory;
  const std::size_t length;

 public:
  BytesReference(const char *memory, std::size_t length)
      : memory(memory), length(length) {}

  std::size_t get_length() const { return this->length; }
  const char *get_memory() const { return this->memory; }
};

class BytesBuffer {
 private:
  std::vector<char> _buffer;

 public:
  BytesBuffer() {}

 public:
  inline void reserve(std::size_t size) { this->_buffer.reserve(size); }
  inline std::size_t size() { return this->_buffer.size(); }
  inline char *get_range(std::size_t start, std::size_t length) {
    if (start + length > this->_buffer.size()) {
      this->_buffer.resize(start + length);
    }
    return &this->_buffer[start];
  }
  inline const char *get_range(std::size_t start, std::size_t length) const {
    return &this->_buffer[start];
  }
};

#endif  // BYTES_H