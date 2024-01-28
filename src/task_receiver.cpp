#include "PolyLinkRPC/task_receiver.h"

void TaskReceiver::_handle_connection() {
  BytesBuffer buffer;
  while (this->_running) {
    bool success = this->_comm->recv_packet(buffer);
    if (!success) {
      buffer.clear();
      continue;
    }

    Task t;
    t.deserialize_from(buffer);
    this->_on_task_callback(t);
  }
}

TaskReceiver::TaskReceiver(
    Communication *comm,
    const std::function<void(const Task &)> &on_task_callback)
    : _comm(comm), _on_task_callback(on_task_callback) {}

bool TaskReceiver::start() {
  bool success = this->_comm->start();
  if (!success) {
    return false;
  }
  this->_running = true;
  this->_thread = std::thread(&TaskReceiver::_handle_connection, this);
  return true;
}

void TaskReceiver::stop() {
  this->_running = false;
  this->_comm->stop();
  this->_thread.join();
}

void TaskReceiver::submit_result(const Result &res) {
  BytesBuffer buffer;
  res.serialize_to(buffer);
  this->_comm->send_packet(buffer);
}