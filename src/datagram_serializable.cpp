#include "PolyLinkRPC/datagram_serializable.h"

void DatagramSerializable::set_error_string(const std::string &msg) {
  this->_error_string = msg;
}

const std::string &DatagramSerializable::get_error_string() const {
  return this->_error_string;
}