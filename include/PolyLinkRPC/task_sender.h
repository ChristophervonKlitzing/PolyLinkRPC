#ifndef TASK_SENDER_H
#define TASK_SENDER_H

#include <atomic>
#include <functional>
#include <thread>

#include "communication.h"
#include "result.h"
#include "task.h"

class TaskSender {
 private:
  std::thread _thread;
  Communication *_comm;
  std::atomic_bool _running = ATOMIC_VAR_INIT(false);
  std::function<void(const Result &)> _on_result_callback;

 private:
  void _handle_connection();

 public:
  TaskSender(Communication *comm,
             const std::function<void(const Result &)> &on_result_callback);

  bool start();
  void stop();
  void submit_task(const Task &task);

  ~TaskSender();
};

#endif  // TASK_SENDER_H