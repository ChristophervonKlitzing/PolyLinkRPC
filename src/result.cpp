#include "PolyLinkRPC/result.h"

#include "PolyLinkRPC/datastream.hpp"

Result::Result(const std::string &type) : Datagram(), _value(type) {}
Result::Result() : Datagram(), _value("") {}

void Result::serialize_to(BytesBuffer &buffer) const {
  DataStream d(buffer);
  d << this->get_id() << this->_value;
}

bool Result::deserialize_from(BytesBuffer &buffer) {
  DataStream d(buffer);
  Datagram::id_t id;
  d >> id >> this->_value;
  this->set_id(id);
  return true;
}