#ifndef TASK_H
#define TASK_H

#include <atomic>
#include <cstdint>
#include <string>
#include <vector>

#include "bytes.h"
#include "value.h"
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
 * | length-ext-1 | <extension-key> | <extension-payload of arg-1> | ...
 */

class RPCBuffer {
 public:
  virtual BytesReference to_bytes() = 0;
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
  std::vector<Value> _args;

 private:
  Task();

 private:
  bool has_valid_version() const;

 public:
  Task(const std::string &func_name,
       const std::vector<std::string> &types = {});

  task_id_t get_id() const;
  std::string get_version_string() const;
  version_number_t get_version_number() const;

  const std::string &get_function_name() const;
  unsigned int get_num_args() const;
  Value &get_argument(std::size_t idx);
  void add_argument(const std::string &type);

  void serialize(BytesBuffer &buffer);
  static Task deserialize(BytesBuffer &buffer);

  bool operator==(const Task &other);
};

#endif  // TASK_H