#include "PolyLinkRPC/datagram.h"

std::atomic<Datagram::id_t> Datagram::_next_id = ATOMIC_VAR_INIT(0);
Datagram::id_t Datagram::create_unique_id() { return Datagram::_next_id++; }

Datagram::Datagram() : _id(Datagram::create_unique_id()) {}

void Datagram::set_id(id_t new_id) { this->_id = new_id; }
Datagram::id_t Datagram::get_id() const { return this->_id; }

void Datagram::set_error_string(const std::string &msg) {
  this->_error_string = msg;
}

const std::string &Datagram::get_error_string() const {
  return this->_error_string;
}