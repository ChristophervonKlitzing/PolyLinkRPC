#ifndef ASYNC_TASK_RECEIVER_H
#define ASYNC_TASK_RECEIVER_H

#include <atomic>
#include <functional>
#include <thread>

#include "communication.h"
#include "result.h"
#include "task.h"

class AsyncTaskReceiver {
 private:
  std::thread _thread;
  Communication *_comm;
  std::atomic_bool _running = ATOMIC_VAR_INIT(false);
  std::function<void(const Task &, AsyncTaskReceiver &)> _on_task_callback;

 private:
  void _handle_connection();

 public:
  AsyncTaskReceiver(Communication *comm,
                    const std::function<void(const Task &, AsyncTaskReceiver &)>
                        &on_task_callback);

  bool start();
  void stop();
  void submit_result(const Result &res);

  ~AsyncTaskReceiver();
};

#endif  // ASYNC_TASK_RECEIVER_H