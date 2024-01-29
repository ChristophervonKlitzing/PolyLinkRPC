#ifndef ASYNC_TASK_SENDER_H
#define ASYNC_TASK_SENDER_H

#include <atomic>
#include <functional>
#include <thread>

#include "communication.h"
#include "result.h"
#include "task.h"

class AsyncTaskSender {
 private:
  std::thread _thread;
  Communication *_comm;
  std::atomic_bool _running = ATOMIC_VAR_INIT(false);
  std::function<void(const Result &)> _on_result_callback;

 private:
  void _handle_connection();

 public:
  AsyncTaskSender(
      Communication *comm,
      const std::function<void(const Result &)> &on_result_callback);

  bool start();
  void stop();
  void submit_task(const Task &task);

  ~AsyncTaskSender();
};

#endif  // ASYNC_TASK_SENDER_H