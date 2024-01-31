#include "PolyLinkRPC/local_communication.h"

LocalCommunicationPair LocalCommunication::create_communication_pair(
    int sender_wait_timeout_ms, int receiver_wait_timeout_ms) {
  LocalCommunication sender;
  LocalCommunication receiver;

  sender.wait_timeout_ms = sender_wait_timeout_ms;
  receiver.wait_timeout_ms = receiver_wait_timeout_ms;

  sender.send_data = std::make_shared<LockAndQueue>();
  sender.recv_data = std::make_shared<LockAndQueue>();

  receiver.send_data = sender.recv_data;
  receiver.recv_data = sender.send_data;

  return {/*.sender_comm=*/sender, /*.receiver_comm=*/receiver};
}

bool LocalCommunication::start() {
  std::lock_guard<std::mutex> lck_recv(this->recv_data->lck);
  this->recv_data->running = true;
  return true;
}

void LocalCommunication::stop() {
  std::lock_guard<std::mutex> lck_recv(this->recv_data->lck);
  this->recv_data->running = false;
  this->recv_data->cv.notify_one();
}

bool LocalCommunication::send_packet(const BytesBuffer &buffer) {
  {
    std::lock_guard<std::mutex> lck(this->send_data->lck);
    if (!this->send_data->running) {
      return false;  // receiver has stopped -> no send
    }
    this->send_data->queue.push(buffer);
  }
  this->send_data->cv.notify_one();  // wake-up potentially waiting recv call
  return true;
}

bool LocalCommunication::recv_packet(BytesBuffer &buffer) {
  std::unique_lock<std::mutex> lck(this->recv_data->lck);
  bool timed_out = !this->recv_data->cv.wait_for(
      lck, std::chrono::milliseconds(this->wait_timeout_ms), [this] {
        return !this->recv_data->queue.empty() || !this->recv_data->running;
      });

  if (!this->recv_data->running) {
    return false;
  }

  if (timed_out) {
    return false;
  }

  // timed_out==false -> queue not empty any more
  buffer = std::move(this->recv_data->queue.front());
  this->recv_data->queue.pop();
  return true;
}