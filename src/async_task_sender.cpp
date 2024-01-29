#include "../include/PolyLinkRPC/async_task_sender.h"

void AsyncTaskSender::_handle_connection() {
  BytesBuffer buffer;
  while (this->_running) {
    bool success = this->_comm->recv_packet(buffer);
    if (!success) {
      buffer.clear();
      continue;
    }

    Result res;
    res.deserialize_from(buffer);
    this->_on_result_callback(res);
  }
}

AsyncTaskSender::AsyncTaskSender(
    Communication *comm,
    const std::function<void(const Result &)> &on_result_callback)
    : _comm(comm), _on_result_callback(on_result_callback) {}

bool AsyncTaskSender::start() {
  bool success = this->_comm->start();
  if (!success) {
    return false;
  }
  this->_running = true;
  this->_thread = std::thread(&AsyncTaskSender::_handle_connection, this);
  return true;
}

void AsyncTaskSender::stop() {
  if (this->_running) {
    this->_running = false;
    this->_comm->stop();
    this->_thread.join();
  }
}

void AsyncTaskSender::submit_task(const Task &t) {
  BytesBuffer buffer;
  t.serialize_to(buffer);
  this->_comm->send_packet(buffer);
}

AsyncTaskSender::~AsyncTaskSender() { this->stop(); }