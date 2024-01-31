#ifndef TASK_H
#define TASK_H

#include <atomic>
#include <cstdint>
#include <string>
#include <vector>

#include "bytes.h"
#include "datagram.h"
#include "value.h"
#include "versions.h"

/**
 * --- RPC Task datagram ---:
 *
 * Header:
 * | id | func-name | #args |
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

class Task : public Datagram {
 protected:  // attributes // protected instead of private due to unit-tests
  std::string _func_name;
  std::vector<Value> _args;

 public:
  Task(const std::string &func_name,
       const std::vector<std::string> &types = {});

  Task(const std::string &func_name, const std::vector<Value> &args);

  Task();
  const std::string &get_function_name() const;
  unsigned int get_num_args() const;
  Value &get_argument(std::size_t idx);
  void add_argument(const std::string &type);

  void serialize_to(BytesBuffer &buffer) const override;
  bool deserialize_from(const BytesBuffer &buffer) override;

  bool operator==(const Task &other);
};

#endif  // TASK_H