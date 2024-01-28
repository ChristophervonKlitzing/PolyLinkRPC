#ifndef LOCAL_COMMUNCATION_H
#define LOCAL_COMMUNCATION_H

#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>

#include "communication.h"
#include "result.h"
#include "task.h"

struct LocalCommunicationPair;

class LocalCommunication : public Communication {
 private:
  struct LockAndQueue {
    std::mutex lck;
    std::condition_variable cv;
    std::queue<BytesBuffer> queue;
  };

 private:
  std::shared_ptr<LockAndQueue> send_data;
  std::shared_ptr<LockAndQueue> recv_data;
  int wait_timeout_ms = -1;

 public:
  static LocalCommunicationPair create_communication_pair(
      int sender_wait_timeout_ms = -1, int receiver_wait_timeout_ms = -1);

 public:
  bool start();
  void stop();
  bool send_packet(const BytesBuffer &buffer);
  bool recv_packet(BytesBuffer &buffer);
};

struct LocalCommunicationPair {
  LocalCommunication sender_comm;
  LocalCommunication receiver_comm;
};

#endif  // LOCAL_COMMUNCATION_H