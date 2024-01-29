#ifndef TASK_DISPATCHER_H
#define TASK_DISPATCHER_H

#include "communication.h"
#include "result.h"

template <typename T>
class Future {};

class TaskDispatcher {
 public:
  TaskDispatcher(Communication *comm);

  Future<Result> call_async_serialized(
      const std::string &fname, const std::vector<Value> &args,
      const std::string &expected_return_type = "");
  Result call_sync_serialized(const std::string &fname,
                              const std::vector<Value> &args,
                              const std::string &expected_return_type = "");

  void call_noreturn_serialized(const std::string &fname,
                                const std::vector<Value> &args);

  // Templated functions for actual use in C++ api
  template <typename return_t, typename... args_t>
  Future<return_t> call_async(const std::string &fname, args_t... args);

  template <typename return_t, typename... args_t>
  return_t call_sync(const std::string &fname, args_t... args);

  template <typename... args_t>
  void call_noreturn(const std::string &fname, args_t... args);
};

#endif  // TASK_DISPATCHER_H