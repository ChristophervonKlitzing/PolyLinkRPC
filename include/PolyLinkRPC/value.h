#ifndef VALUE_H
#define VALUE_H

#include <string>
#include <vector>

#include "PolyLinkRPC/bytes.h"

/**
 * Mutable buffer for storing a typed value.
 * It allows for adding bytes
 */
class Value {
 private:
  std::string _type;
  std::vector<char> _data;

 public:
  Value(const std::string &type) : _type(type) {}

  const std::string &get_type() const { return this->_type; }
  BytesReference get_value() const;
  void append_to_value(const char *data, std::size_t len);
  std::size_t get_size() const;

  bool operator==(const Value &other) const;
};

#endif  // VALUE_H