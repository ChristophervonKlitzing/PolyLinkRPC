#include <gtest/gtest.h>

#include "PolyLinkRPC/local_communication.h"
#include "PolyLinkRPC/task_dispatcher.h"
#include "PolyLinkRPC/task_responder.h"

namespace {
class CustomExecutor : public TaskExecutor {
 public:
  std::vector<std::string> func_names = {"funcA", "funcB"};
  std::vector<std::string> return_types = {"bool", "int"};
  int access_index = 0;

 public:
  Result execute_task(const Task t) override {
    return Result(return_types[access_index]);
  }
  const std::vector<std::string> &get_supported_functions() const override {
    return this->func_names;
  }
};
}  // namespace

TEST(task_dispatcher_responder, basic) {
  auto comm_pair = LocalCommunication::create_communication_pair(100, 100);

  // Caller:
  TaskDispatcher rpc_dispatcher(&comm_pair.sender_comm);

  // Receiver:
  CustomExecutor executor;
  TaskResponder rpc_responder(&comm_pair.receiver_comm, executor);

  {
    std::string fname = executor.func_names[executor.access_index];
    std::string return_type = executor.return_types[executor.access_index];

    auto future = rpc_dispatcher.call_async_serialized(fname, {});
    Result result = future.get();
    ASSERT_STREQ(result.get_value().get_type().c_str(), return_type.c_str());
  }

  executor.access_index++;  // increase index to test with a second rpc call

  {
    std::string fname = executor.func_names[executor.access_index];
    std::string return_type = executor.return_types[executor.access_index];

    auto future = rpc_dispatcher.call_async_serialized(fname, {});
    Result result = future.get();
    ASSERT_STREQ(result.get_value().get_type().c_str(), return_type.c_str());
  }
}