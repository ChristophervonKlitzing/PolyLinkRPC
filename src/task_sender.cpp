#include "PolyLinkRPC/task_sender.h"

void TaskSender::_handle_connection() {
  BytesBuffer buffer;
  while (this->_running) {
    bool success = this->_comm->recv_packet(buffer);
    if (!success) {
      buffer.clear();
      continue;
    }

    Result t;
    t.deserialize_from(buffer);
    this->_on_task_callback(t);
  }
}

TaskSender::TaskSender(
    Communication *comm,
    const std::function<void(const Result &)> &on_task_callback)
    : _comm(comm), _on_task_callback(on_task_callback) {}

bool TaskSender::start() {
  bool success = this->_comm->start();
  if (!success) {
    return false;
  }
  this->_running = true;
  this->_thread = std::thread(&TaskSender::_handle_connection, this);
  return true;
}

void TaskSender::stop() {
  this->_running = false;
  this->_comm->stop();
  this->_thread.join();
}

void TaskSender::submit_task(const Task &t) {
  BytesBuffer buffer;
  t.serialize_to(buffer);
  this->_comm->send_packet(buffer);
}