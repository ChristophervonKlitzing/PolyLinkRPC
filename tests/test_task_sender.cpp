#include <gtest/gtest.h>

#include <condition_variable>
#include <mutex>

#include "PolyLinkRPC/datastream.hpp"
#include "PolyLinkRPC/task_sender.h"

class MockCommunicationWorking : public Communication {
 private:
  std::atomic<bool> received = ATOMIC_VAR_INIT(false);
  std::atomic<Datagram::id_t> task_id = ATOMIC_VAR_INIT(0);

 public:
  bool start() override { return true; }
  void stop() override {}

  bool send_packet(const BytesBuffer &buffer) override {
    Task t;
    t.deserialize_from(buffer);
    this->task_id = t.get_id();
    this->received = true;
    return true;
  }
  bool recv_packet(BytesBuffer &buffer) override {
    if (this->received == false) {
      return false;
    }
    Result res;
    res.set_id(this->task_id);
    res.serialize_to(buffer);
    return true;
  }
};

TEST(task_sender, send_receive_callback) {
  std::atomic<bool> received = ATOMIC_VAR_INIT(false);
  std::atomic<Datagram::id_t> result_id = ATOMIC_VAR_INIT(0);

  Datagram::id_t task_id = 42;

  std::function<void(const Result &)> result_callback = [&](const Result &res) {
    result_id = task_id;
    received = true;
  };

  MockCommunicationWorking comm;
  TaskSender sender(&comm, result_callback);

  bool sender_started = sender.start();
  ASSERT_TRUE(sender_started);

  Task t;
  t.set_id(task_id);
  sender.submit_task(t);

  // wait for result
  while (!received) {
    // TODO: Change into waiting with a condition variable as it allows
    // for a timeout
  }

  sender.stop();
  ASSERT_EQ(task_id, result_id);
}

class MockCommunicationFailingStartup : public Communication {
 public:
  bool start() override { return false; }
  void stop() override {}

  bool send_packet(const BytesBuffer &buffer) override { return false; }
  bool recv_packet(BytesBuffer &buffer) override { return false; }
};

TEST(task_sender, failing_startup) {
  std::function<void(const Result &)> result_callback = [](const Result &res) {
    // should never be called
  };

  MockCommunicationFailingStartup comm;
  TaskSender sender(&comm, result_callback);

  bool sender_started = sender.start();
  EXPECT_FALSE(sender_started)
      << "When the communication fails to start, the sender should also fail";
}

class MockCommunicationFailingReceive : public Communication {
 public:
  std::atomic<int> recv_call_count = ATOMIC_VAR_INIT(0);
  bool start() override { return true; }
  void stop() override {}

  bool send_packet(const BytesBuffer &buffer) override { return false; }
  bool recv_packet(BytesBuffer &buffer) override {
    this->recv_call_count++;
    return false;
  }
};

TEST(task_sender, failing_receive) {
  int result_callback_count = 0;
  std::function<void(const Result &)> result_callback = [&](const Result &res) {
    // should never be called
    result_callback_count++;
  };

  MockCommunicationFailingReceive comm;
  TaskSender sender(&comm, result_callback);
  sender.start();

  while (comm.recv_call_count < 2) {
    // Wait until received got called at least twice. This guarantees that the
    // sender's receive loop handled a failing receive at least once.
  }

  sender.stop();
  ASSERT_EQ(result_callback_count, 0);
}