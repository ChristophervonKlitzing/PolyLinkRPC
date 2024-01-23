#ifndef TASK_H
#define TASK_H

#include <atomic>
#include <cstdint>
#include <string>
#include <vector>

#include "bytes.h"
#include "datagram_serializable.h"
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

class Task : public DatagramSerializable {
 public:  // type-defs
  using task_id_t = std::uint64_t;

 private:  // static attributes
  static std::atomic<task_id_t> _next_id;

 private:  // static functions
  static task_id_t get_unique_id() { return Task::_next_id++; }

 protected:  // attributes // protected instead of private due to unit-tests
  task_id_t _id;
  version_number_t _protocol_version;
  std::string _func_name;
  std::vector<Value> _args;

 private:
  bool is_compatible() const;

 public:
  Task(const std::string &func_name,
       const std::vector<std::string> &types = {});

  Task();

  task_id_t get_id() const;
  std::string get_version_string() const;
  version_number_t get_version_number() const;

  const std::string &get_function_name() const;
  unsigned int get_num_args() const;
  Value &get_argument(std::size_t idx);
  void add_argument(const std::string &type);

  void serialize_to(BytesBuffer &buffer) const override;
  bool deserialize_from(BytesBuffer &buffer) override;

  /**
   * Do not use this function if you are not sure what you are doing. This
   * function overwrites the version of the underlying task instance which
   * may result which may result in wrong deserialization. This function is
   * mainly for unit-tests.
   */
  void __change_version(version_number_t new_version);

  bool operator==(const Task &other);
};

#endif  // TASK_H