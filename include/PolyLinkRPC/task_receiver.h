#ifndef TASK_RECEIVER_H
#define TASK_RECEIVER_H

#include <atomic>
#include <functional>
#include <thread>

#include "communication.h"
#include "result.h"
#include "task.h"

class TaskReceiver {
 private:
  std::thread _thread;
  Communication *_comm;
  std::atomic_bool _running = ATOMIC_VAR_INIT(false);
  std::function<void(const Task &, TaskReceiver &)> _on_task_callback;

 private:
  void _handle_connection();

 public:
  TaskReceiver(Communication *comm,
               const std::function<void(const Task &, TaskReceiver &)>
                   &on_task_callback);

  bool start();
  void stop();
  void submit_result(const Result &res);

  ~TaskReceiver();
};

#endif  // TASK_RECEIVER_H