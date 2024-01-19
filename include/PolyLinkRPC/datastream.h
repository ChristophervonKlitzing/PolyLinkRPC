#ifndef DATASTREAM_H
#define DATASTREAM_H

#include <cstring>
#include <string>

#include "bytes.h"
#include "endian_utils.h"

/**
 * This class provides system independent (e.g. endian-ness)
 * (de-)serialization implementations for common data types.
 */
class DataStream {
 private:
  BytesBuffer &_buffer;
  std::size_t _read_position = 0;
  std::size_t _write_position = 0;
  EndianFormat _byte_order = EndianFormat::BigEndian;  // network standard

 public:
  DataStream(BytesBuffer &buffer) : _buffer(buffer) {}
  inline void set_read_position(std::size_t position) {
    this->_read_position = position;
  }
  inline void set_write_position(std::size_t position) {
    this->_write_position = position;
  }

  inline void set_byte_order(EndianFormat byte_order) {
    this->_byte_order = byte_order;
  }
  inline EndianFormat get_byte_order() const { return this->_byte_order; }

  inline void write_data(const char *data, std::size_t len) {
    char *array = this->_buffer.get_range(this->_write_position, len);
    std::memcpy(array, data, len);
    this->_write_position += len;
  }

  inline const char *read_data(std::size_t len) {
    char *array = this->_buffer.get_range(this->_read_position, len);
    this->_read_position += len;
    return array;
  }
};

// Add functions here
template <typename T, typename std::enable_if<(std::is_integral<T>::value),
                                              bool>::type = true>
DataStream &operator<<(DataStream &stream, const T &value) {
  T reordered_value = to_from_endian_format(value, stream.get_byte_order());
  stream.write_data(reinterpret_cast<const char *>(&reordered_value),
                    sizeof(T));
  return stream;
}

template <typename T, typename std::enable_if<(std::is_integral<T>::value),
                                              bool>::type = true>
DataStream &operator>>(DataStream &stream, T &value) {
  const T *read_value =
      reinterpret_cast<const T *>(stream.read_data(sizeof(T)));
  value = to_from_endian_format(*read_value, stream.get_byte_order());
  return stream;
}

inline DataStream &operator<<(DataStream &stream, const std::string &str) {
  // prepend size of string
  stream << static_cast<uint64_t>(str.size());

  // Append string content
  stream.write_data(str.c_str(), str.size());
  return stream;
}

inline DataStream &operator>>(DataStream &stream, std::string &str) {
  uint64_t str_size;
  // prepend size of string
  stream >> str_size;

  const char *str_data = stream.read_data(str_size);

  str = std::string(str_data, str_size);
  return stream;
}

#endif  // DATASTREAM_H