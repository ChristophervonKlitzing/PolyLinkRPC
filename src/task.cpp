#include "PolyLinkRPC/task.h"

#include <cstring>
#include <iostream>

#include "PolyLinkRPC/datastream.h"
#include "PolyLinkRPC/endian_utils.h"
#include "PolyLinkRPC/versions.h"

std::size_t Argument::get_size() const { return this->_data.size(); }

std::atomic<Task::task_id_t> Task::_next_id = ATOMIC_VAR_INIT(0);

Task::Task(const std::string &func_name, const std::vector<std::string> &types)
    : _id(Task::get_unique_id()),
      _protocol_version(VERSION_NUMBER),
      _func_name(func_name) {
  this->_args.reserve(types.size());
  for (const std::string &type : types) {
    this->_args.emplace_back(Argument(type));
  }
}

Task::Task() {}

const std::string &Task::get_function_name() const { return this->_func_name; }

Task::task_id_t Task::get_id() const { return this->_id; }

std::string Task::get_version_string() const {
  return versionNumberToString(this->_protocol_version);
}

version_number_t Task::get_version_number() const {
  return this->_protocol_version;
}

unsigned int Task::get_num_args() const { return this->_args.size(); }

Argument &Task::get_argument(std::size_t idx) { return this->_args.at(idx); }

void Task::add_argument(const std::string &type) {
  this->_args.emplace_back(Argument(type));
}

void Task::serialize(BytesBuffer &buffer) {
  DataStream stream(buffer);

  // Header:
  stream << this->_id << VERSION_NUMBER << this->_func_name
         << static_cast<uint16_t>(this->get_num_args());
}
Task Task::deserialize(BytesBuffer &buffer) {
  DataStream stream(buffer);
  Task task;

  stream >> task._id;
  stream >> task._protocol_version;

  // TODO: Check if datagram_version is compatible with current VERSION_NUMBER

  uint16_t num_args;
  stream >> task._func_name >> num_args;

  return task;
}

bool Task::operator==(const Task &other) {
  return (this->_id == other._id) &&
         (this->_protocol_version == other._protocol_version) &&
         (this->_func_name == other._func_name);
}