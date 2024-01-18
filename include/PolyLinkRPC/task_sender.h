#ifndef TASK_SENDER_H
#define TASK_SENDER_H

#include "task.h"

class TaskSender {
 public:
  void start();
  void stop();
  void submit_task(const Task &task);
};

#endif  // TASK_SENDER_H