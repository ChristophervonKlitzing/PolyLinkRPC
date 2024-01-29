#ifndef TASK_RESPONDER_H
#define TASK_RESPONDER_H

#include "communication.h"
#include "result.h"
#include "task.h"

class TaskExecutor {
 public:
  /**
   * Any task thrown in here will result in a TaskExecutionException
   * on the sender-side.
   * TODO: Either throw it or output it in some form.
   */
  virtual Result execute_task(const Task t) = 0;
  virtual const std::vector<std::string> &get_supported_functions() const = 0;
};

class TaskResponder {
 public:
  TaskResponder(Communication *comm, TaskExecutor &executor);
  bool start();
  void stop();
};

#endif  // TASK_RESPONDER_H