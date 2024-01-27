#ifndef DATASTREAM_H
#define DATASTREAM_H

#include <cstring>
#include <stdexcept>
#include <string>

#include "bytes.h"
#include "endian_utils.h"
#include "value.h"

/**
 * This class provides system independent (e.g. endian-ness)
 * serialization implementations for common data types.
 */
class WritableDataStream {
 private:
  std::size_t _write_position = 0;
  BytesBuffer &_write_buffer;
  EndianFormat _byte_order = EndianFormat::BigEndian;  // network standard

 public:
  inline WritableDataStream(BytesBuffer &buffer) : _write_buffer(buffer){};

  inline void set_byte_order(EndianFormat byte_order) {
    this->_byte_order = byte_order;
  }
  inline EndianFormat get_byte_order() const { return this->_byte_order; }

  inline void set_write_position(std::size_t position) {
    this->_write_position = position;
  }
  inline void write_data(const char *data, std::size_t len) {
    if (len > 0) {
      char *array =
          this->_write_buffer.get_write_range(this->_write_position, len);
      std::memcpy(array, data, len);
      this->_write_position += len;
    }
  }
};

/**
 * This class provides system independent (e.g. endian-ness)
 * deserialization implementations for common data types.
 */
class ReadableDataStream {
 private:
  std::size_t _read_position = 0;
  const BytesBuffer &_read_buffer;
  EndianFormat _byte_order = EndianFormat::BigEndian;  // network standard

 public:
  inline ReadableDataStream(const BytesBuffer &buffer) : _read_buffer(buffer){};

  inline void set_byte_order(EndianFormat byte_order) {
    this->_byte_order = byte_order;
  }
  inline EndianFormat get_byte_order() const { return this->_byte_order; }

  inline void set_read_position(std::size_t position) {
    this->_read_position = position;
  }

  inline const char *read_data(std::size_t len) {
    const char *array =
        this->_read_buffer.get_read_range(this->_read_position, len);
    this->_read_position += len;
    return array;
  }
};

// Add functions here
template <typename T, typename std::enable_if<(std::is_integral<T>::value),
                                              bool>::type = true>
WritableDataStream &operator<<(WritableDataStream &stream, const T &value) {
  T reordered_value = to_from_endian_format(value, stream.get_byte_order());
  stream.write_data(reinterpret_cast<const char *>(&reordered_value),
                    sizeof(T));
  return stream;
}

template <typename T, typename std::enable_if<(std::is_integral<T>::value),
                                              bool>::type = true>
ReadableDataStream &operator>>(ReadableDataStream &stream, T &value) {
  const T *read_value =
      reinterpret_cast<const T *>(stream.read_data(sizeof(T)));
  value = to_from_endian_format(*read_value, stream.get_byte_order());
  return stream;
}

inline WritableDataStream &operator<<(WritableDataStream &stream,
                                      const std::string &str) {
  // prepend size of string
  stream << static_cast<uint64_t>(str.size());

  // Append string content
  stream.write_data(str.c_str(), str.size());
  return stream;
}

inline ReadableDataStream &operator>>(ReadableDataStream &stream,
                                      std::string &str) {
  uint64_t str_size;
  // prepend size of string
  stream >> str_size;

  const char *str_data = stream.read_data(str_size);

  str = std::string(str_data, str_size);
  return stream;
}

inline WritableDataStream &operator<<(WritableDataStream &stream,
                                      const Value &value) {
  stream << value.get_type();

  BytesReference bref = value.get_value();
  stream << static_cast<uint64_t>(bref.get_length());
  stream.write_data(bref.get_memory(), bref.get_length());
  return stream;
}

inline ReadableDataStream &operator>>(ReadableDataStream &stream,
                                      Value &value) {
  std::string type;
  uint64_t size;
  stream >> type >> size;

  value = Value(type);
  const char *data = stream.read_data(size);
  value.append_to_value(data, size);
  return stream;
}

#endif  // DATASTREAM_H