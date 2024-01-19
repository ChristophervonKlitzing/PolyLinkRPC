#ifndef TASK_H
#define TASK_H

#include <atomic>
#include <cstdint>
#include <string>
#include <vector>

#include "bytes.h"
#include "versions.h"

/**
 * --- RPC Task datagram ---:
 *
 * Header: // id and protocol-version should never change after first release!
 * | id | protocol-version | func-name | #args |
 *
 * # Argument Payload:
 * | length-arg-1 | arg-1-type | arg-1-value | ...
 *
 * --- Extensions ---
 *
 * Extension Header:
 * | #extensions |
 *
 * Extension Payload:
 * | length-ext-1 | <extension-payload of arg-1> | ...
 */

class RPCBuffer {
 public:
  virtual BytesReference to_bytes() = 0;
};

/**
 * Mutable buffer for storing a typed value.
 * It allows for adding bytes
 */
class Argument {
 private:
  const std::string _type;
  std::vector<char> _data;

 public:
  Argument(const std::string &type) : _type(type) {}

  const std::string &get_type() const { return this->_type; }
  BytesReference get_value() const;
  void append_to_value(const BytesReference &bytes);
  std::size_t get_size() const;
};

class Task {
 public:  // type-defs
  using task_id_t = std::uint64_t;

 private:  // static attributes
  static std::atomic<task_id_t> _next_id;

 private:  // static functions
  static task_id_t get_unique_id() { return Task::_next_id++; }

 private:  // attributes
  task_id_t _id;
  version_number_t _protocol_version;
  std::string _func_name;
  std::vector<Argument> _args;

 private:
  Task();

 public:
  Task(const std::string &func_name,
       const std::vector<std::string> &types = {});

  task_id_t get_id() const;
  std::string get_version_string() const;
  version_number_t get_version_number() const;

  const std::string &get_function_name() const;
  unsigned int get_num_args() const;
  Argument &get_argument(std::size_t idx);
  void add_argument(const std::string &type);

  void serialize(BytesBuffer &buffer);
  static Task deserialize(BytesBuffer &buffer);

  bool operator==(const Task &other);
};

#endif  // TASK_H