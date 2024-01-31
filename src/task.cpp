#include "PolyLinkRPC/task.h"

#include <cstring>

#include "PolyLinkRPC/datastream.hpp"
#include "PolyLinkRPC/endian_utils.h"
#include "PolyLinkRPC/versions.h"

Task::Task(const std::string &func_name, const std::vector<std::string> &types)
    : Datagram(), _func_name(func_name) {
  this->_args.reserve(types.size());
  for (const std::string &type : types) {
    this->_args.emplace_back(Value(type));
  }
}

Task::Task(const std::string &func_name, const std::vector<Value> &args)
    : Datagram(), _func_name(func_name), _args(args) {}

Task::Task() : Datagram() {}

const std::string &Task::get_function_name() const { return this->_func_name; }

unsigned int Task::get_num_args() const { return this->_args.size(); }

Value &Task::get_argument(std::size_t idx) { return this->_args.at(idx); }

void Task::add_argument(const std::string &type) {
  this->_args.emplace_back(Value(type));
}

bool Task::operator==(const Task &other) {
  return (this->get_id() == other.get_id()) &&
         (this->_func_name == other._func_name) && (this->_args == other._args);
}

void Task::serialize_to(BytesBuffer &buffer) const {
  WritableDataStream write_stream(buffer);

  // ======== HEADER ==========:
  write_stream << this->get_id() << this->_func_name
               << static_cast<uint16_t>(this->get_num_args());

  // ======== ARGUMENTS ==========:
  for (const Value &arg : this->_args) {
    write_stream << arg;
  }

  // ======== EXTENSIONS =========:
  // For now use 0 extensions as they are not supported
  write_stream << static_cast<uint16_t>(0);
}

bool Task::deserialize_from(const BytesBuffer &buffer) {
  ReadableDataStream read_stream(buffer);

  // ======== HEADER ==========:
  Datagram::id_t id;
  read_stream >> id;
  this->set_id(id);
  uint16_t num_args;
  read_stream >> this->_func_name >> num_args;
  this->_args.reserve(num_args);

  // ======== ARGUMENTS ==========:
  for (unsigned int i = 0; i < num_args; ++i) {
    Value v("");
    read_stream >> v;
    this->_args.push_back(v);
  }

  // ======== EXTENSIONS =========:
  uint16_t num_extensions;
  read_stream >> num_extensions;
  if (num_extensions > 0) {
    this->set_error_string("Extensions are not supported yet");
    return false;
  }

  return true;
}
