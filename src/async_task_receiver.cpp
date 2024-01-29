#include "../include/PolyLinkRPC/async_task_receiver.h"

void AsyncTaskReceiver::_handle_connection() {
  BytesBuffer buffer;
  while (this->_running) {
    bool success = this->_comm->recv_packet(buffer);
    if (!success) {
      buffer.clear();
      continue;
    }

    Task t;
    t.deserialize_from(buffer);
    this->_on_task_callback(t, *this);
  }
}

AsyncTaskReceiver::AsyncTaskReceiver(
    Communication *comm,
    const std::function<void(const Task &, AsyncTaskReceiver &)>
        &on_task_callback)
    : _comm(comm), _on_task_callback(on_task_callback) {}

bool AsyncTaskReceiver::start() {
  bool success = this->_comm->start();
  if (!success) {
    return false;
  }
  this->_running = true;
  this->_thread = std::thread(&AsyncTaskReceiver::_handle_connection, this);
  return true;
}

void AsyncTaskReceiver::stop() {
  if (this->_running) {
    this->_running = false;
    this->_comm->stop();
    this->_thread.join();
  }
}

void AsyncTaskReceiver::submit_result(const Result &res) {
  BytesBuffer buffer;
  res.serialize_to(buffer);
  this->_comm->send_packet(buffer);
}

AsyncTaskReceiver::~AsyncTaskReceiver() { this->stop(); }