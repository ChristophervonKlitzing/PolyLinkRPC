#include "PolyLinkRPC/value.h"

#include <cstring>

std::size_t Value::get_size() const { return this->_data.size(); }

void Value::append_to_value(const char *data, std::size_t len) {
  if (len > 0) {
    std::size_t current_size = this->_data.size();
    this->_data.resize(current_size + len);
    std::memcpy(&_data[current_size], data, len);
  }
}

BytesReference Value::get_value() const {
  return BytesReference(this->_data.data(), this->_data.size());
}

bool Value::operator==(const Value &other) const {
  return this->_type == other._type && this->_data == other._data;
}