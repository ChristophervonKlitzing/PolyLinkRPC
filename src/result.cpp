#include "PolyLinkRPC/result.h"

#include "PolyLinkRPC/datastream.hpp"

Result::Result(const std::string &type) : Datagram(), _value(type) {}
Result::Result() : Datagram(), _value("") {}

void Result::serialize_to(BytesBuffer &buffer) const {
  WritableDataStream write_stream(buffer);
  write_stream << this->get_id() << this->_value;
}

bool Result::deserialize_from(const BytesBuffer &buffer) {
  ReadableDataStream read_stream(buffer);
  Datagram::id_t id;
  read_stream >> id >> this->_value;
  this->set_id(id);
  return true;
}