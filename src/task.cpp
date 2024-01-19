#include "PolyLinkRPC/task.h"

#include <cstring>
#include <iostream>

#include "PolyLinkRPC/datastream.h"
#include "PolyLinkRPC/endian_utils.h"
#include "PolyLinkRPC/versions.h"

std::atomic<Task::task_id_t> Task::_next_id = ATOMIC_VAR_INIT(0);

Task::Task(const std::string &func_name, const std::vector<std::string> &types)
    : _id(Task::get_unique_id()),
      _protocol_version(VERSION_NUMBER),
      _func_name(func_name) {
  this->_args.reserve(types.size());
  for (const std::string &type : types) {
    this->_args.emplace_back(Value(type));
  }
}

Task::Task() {}

bool Task::has_valid_version() const {
  // TODO: Replace with proper implementation which compares the task's version
  // number with the library version.
  return true;
}

const std::string &Task::get_function_name() const { return this->_func_name; }

Task::task_id_t Task::get_id() const { return this->_id; }

std::string Task::get_version_string() const {
  return versionNumberToString(this->_protocol_version);
}

version_number_t Task::get_version_number() const {
  return this->_protocol_version;
}

unsigned int Task::get_num_args() const { return this->_args.size(); }

Value &Task::get_argument(std::size_t idx) { return this->_args.at(idx); }

void Task::add_argument(const std::string &type) {
  this->_args.emplace_back(Value(type));
}

void Task::serialize(BytesBuffer &buffer) {
  DataStream stream(buffer);

  // ======== HEADER ==========:
  stream << this->_id << VERSION_NUMBER << this->_func_name
         << static_cast<uint16_t>(this->get_num_args());

  // ======== ARGUMENTS ==========:
  for (const Value &arg : this->_args) {
    stream << arg;
  }

  // ======== EXTENSIONS =========:
  // For now use 0 extensions as they are not supported
  stream << static_cast<uint16_t>(0);
}
Task Task::deserialize(BytesBuffer &buffer) {
  DataStream stream(buffer);
  Task task;

  // ======== HEADER ==========:
  stream >> task._id;
  stream >> task._protocol_version;

  if (!task.has_valid_version()) {
    std::string msg =
        std::string("The decoded Task-datagram with version '") +
        versionNumberToString(task._protocol_version) +
        "' is not compatible with the version of this library which is '" +
        VERSION_STRING + "'";
    throw std::runtime_error(msg);
  }

  uint16_t num_args;
  stream >> task._func_name >> num_args;
  task._args.reserve(num_args);

  // ======== ARGUMENTS ==========:
  for (unsigned int i = 0; i < num_args; ++i) {
    Value v("");
    stream >> v;
    task._args.push_back(v);
  }

  // ======== EXTENSIONS =========:
  uint16_t num_extensions;
  stream >> num_extensions;
  if (num_extensions > 0) {
    throw std::invalid_argument("Extensions are not supported yet");
  }

  return task;
}

bool Task::operator==(const Task &other) {
  return (this->_id == other._id) &&
         (this->_protocol_version == other._protocol_version) &&
         (this->_func_name == other._func_name) && (this->_args == other._args);
}