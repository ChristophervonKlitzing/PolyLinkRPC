#include "PolyLinkRPC/value.h"

#include <cstring>

std::size_t Value::get_size() const { return this->_data.size(); }

void Value::append_to_value(const BytesReference &bytes) {
  std::size_t current_size = this->_data.size();
  this->_data.resize(current_size + bytes.get_length());
  std::memcpy(&_data[current_size], bytes.get_memory(), bytes.get_length());
}

BytesReference Value::get_value() const {
  return BytesReference(this->_data.data(), this->_data.size());
}

bool Value::operator==(const Value &other) const {
  return this->_type == other._type && this->_data == other._data;
}