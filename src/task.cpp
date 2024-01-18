#include "PolyLinkRPC/task.h"

#include <cstring>
#include <iostream>

#include "PolyLinkRPC/endian_utils.h"
#include "PolyLinkRPC/versions.h"

/**
 * Only used inside this cpp file -> template is okay
 */
template <typename T>
void serialize_integral_type(char **position, T value) {
  T big_endian_value = to_from_big_endian(value);

  T *typed_position = reinterpret_cast<T *>(*position);
  *typed_position = big_endian_value;
  (*position) += sizeof(T);
}

template <typename T>
T deserialize_integral_type(const char **position) {
  T big_endian_value = *reinterpret_cast<const T *>(*position);
  (*position) += sizeof(T);

  return to_from_big_endian(big_endian_value);
}

void serialize_str(char **position, const std::string &str) {
  // prepend size of string
  serialize_integral_type(position, static_cast<uint64_t>(str.size()));

  // Append string content
  std::memcpy(*position, str.c_str(), str.size());
  (*position) += str.size();
}
std::string deserialize_str(const char **position) {
  // read size of string
  uint64_t str_size = deserialize_integral_type<uint64_t>(position);
  std::string s(str_size, 'x');
  char *dst = &s[0];

  // Read string content
  std::memcpy(dst, *position, str_size);
  (*position) += str_size;
  return s;
}

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

Task::Task(const std::string &fname, task_id_t id, version_number_t version)
    : _id(id), _protocol_version(version), _func_name(fname) {}

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

std::vector<char> Task::serialize() {
  std::size_t header_size = sizeof(this->_id) + sizeof(VERSION_NUMBER) +
                            (this->_func_name.size() + sizeof(uint64_t)) +
                            sizeof(uint64_t);

  std::vector<char> bytes(header_size);
  char *pos = bytes.data();

  // Header
  serialize_integral_type(&pos, this->_id);
  serialize_integral_type(&pos, VERSION_NUMBER);
  serialize_str(&pos, this->_func_name);

  // Arguments

  return bytes;
}

Task Task::deserialize(std::vector<char> &vec) {
  const char *pos = vec.data();

  // Header
  Task::task_id_t id = deserialize_integral_type<Task::task_id_t>(&pos);
  version_number_t version = deserialize_integral_type<version_number_t>(&pos);
  std::string fname = deserialize_str(&pos);

  // Arguments

  return Task(fname, id, version);
}
bool Task::operator==(const Task &other) {
  return (this->_id == other._id) &&
         (this->_protocol_version == other._protocol_version) &&
         (this->_func_name == other._func_name);
}