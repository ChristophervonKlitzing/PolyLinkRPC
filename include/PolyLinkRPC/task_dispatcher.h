#ifndef TASK_DISPATCHER_H
#define TASK_DISPATCHER_H

#include <future>
#include <map>
#include <mutex>

#include "async_task_sender.h"
#include "communication.h"
#include "result.h"

class TaskDispatcher {
 private:
  AsyncTaskSender async_sender;

  std::map<Datagram::id_t, std::promise<Result>> result_dict;
  std::mutex lck;

 private:
  void on_result(const Result &return_value);

 public:
  TaskDispatcher(Communication *comm);

  std::future<Result> call_async_serialized(
      const std::string &fname, const std::vector<Value> &args,
      const std::string &expected_return_type = "");
  Result call_sync_serialized(const std::string &fname,
                              const std::vector<Value> &args,
                              const std::string &expected_return_type = "");

  void call_noreturn_serialized(const std::string &fname,
                                const std::vector<Value> &args);

  // Templated functions for actual use in C++ api
  template <typename return_t, typename... args_t>
  std::future<return_t> call_async(const std::string &fname, args_t... args);

  template <typename return_t, typename... args_t>
  return_t call_sync(const std::string &fname, args_t... args);

  template <typename... args_t>
  void call_noreturn(const std::string &fname, args_t... args);

  ~TaskDispatcher();
};

#endif  // TASK_DISPATCHER_H