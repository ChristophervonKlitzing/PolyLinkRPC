#include "PolyLinkRPC/task_dispatcher.h"

#include <future>

TaskDispatcher::TaskDispatcher(Communication *comm)
    : async_sender(comm, std::bind(&TaskDispatcher::on_result, this,
                                   std::placeholders::_1)) {
  this->async_sender.start();
}

void TaskDispatcher::on_result(const Result &return_value) {
  std::promise<Result> promise =
      std::move(this->result_dict.at(return_value.get_id()));
  this->result_dict.erase(return_value.get_id());
  promise.set_value(return_value);
}

std::future<Result> TaskDispatcher::call_async_serialized(
    const std::string &fname, const std::vector<Value> &args,
    const std::string &expected_return_type) {
  // TODO: expected_return_type not used
  Task task(fname, args);
  this->async_sender.submit_task(task);

  this->result_dict[task.get_id()] = {};
  return this->result_dict[task.get_id()].get_future();
}

Result TaskDispatcher::call_sync_serialized(
    const std::string &fname, const std::vector<Value> &args,
    const std::string &expected_return_type) {
  // TODO: expected_return_type not used
  return this->call_async_serialized(fname, args, expected_return_type).get();
}

TaskDispatcher::~TaskDispatcher() { this->async_sender.stop(); }