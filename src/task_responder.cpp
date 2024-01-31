#include "PolyLinkRPC/task_responder.h"

TaskResponder::TaskResponder(Communication *comm, TaskExecutor &executor)
    : async_receiver(
          comm, std::bind(&TaskResponder::on_task, this, std::placeholders::_1,
                          std::placeholders::_2)),
      executor(executor) {
  this->async_receiver.start();
}

void TaskResponder::on_task(const Task &task, AsyncTaskReceiver &receiver) {
  Result result = executor.execute_task(task);
  result.set_id(task.get_id());
  receiver.submit_result(std::move(result));
}

TaskResponder::~TaskResponder() { this->async_receiver.stop(); }