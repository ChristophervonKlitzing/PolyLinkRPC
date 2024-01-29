#include <gtest/gtest.h>

#include "PolyLinkRPC/async_task_receiver.h"
#include "PolyLinkRPC/datastream.hpp"

namespace {  // prevent external linkage to other translation units
class MockCommunicationWorking : public Communication {
 private:
  std::atomic<bool> receive_task_set = ATOMIC_VAR_INIT(false);
  const Task *receive_task = nullptr;

 public:
  std::atomic<bool> sent_submitted_result = ATOMIC_VAR_INIT(false);
  Result submitted_reconstructed_result;

 public:
  bool start() override { return true; }
  void stop() override {}

  bool send_packet(const BytesBuffer &buffer) override {
    this->submitted_reconstructed_result.deserialize_from(buffer);
    this->sent_submitted_result = true;
    return true;
  }
  bool recv_packet(BytesBuffer &buffer) override {
    if (!this->receive_task_set) {
      return false;
    }
    this->receive_task->serialize_to(buffer);
    this->receive_task_set = false;  // only simulate receiving one task
    return true;
  }

  void set_recv_task(const Task &t) {
    this->receive_task = &t;
    receive_task_set = true;
  }
};
}  // anonymous namespace

TEST(task_receiver, send_receive_callback) {
  std::atomic<bool> received = ATOMIC_VAR_INIT(false);

  Task received_task;

  std::function<void(const Task &, AsyncTaskReceiver &)> task_callback =
      [&](const Task &t, AsyncTaskReceiver &) {
        received_task = t;
        received = true;
      };

  MockCommunicationWorking comm;
  AsyncTaskReceiver receiver(&comm, task_callback);

  bool receiver_started = receiver.start();
  ASSERT_TRUE(receiver_started);

  Task task_to_receive;
  task_to_receive.set_id(42);
  comm.set_recv_task(task_to_receive);

  // wait for result
  while (!received) {
    // wait for callback
  }

  ASSERT_TRUE(task_to_receive == received_task);

  Result res_to_submit;
  res_to_submit.set_id(task_to_receive.get_id());
  receiver.submit_result(res_to_submit);

  while (!comm.sent_submitted_result) {
    // wait for result being sent via the comm object
  }
  ASSERT_TRUE(comm.submitted_reconstructed_result == res_to_submit);

  receiver.stop();
}

namespace {  // prevent external linkage to other translation units
class MockCommunicationFailingStartup : public Communication {
 public:
  bool start() override { return false; }
  void stop() override {}

  bool send_packet(const BytesBuffer &buffer) override { return false; }
  bool recv_packet(BytesBuffer &buffer) override { return false; }
};
}  // anonymous namespace

TEST(task_receiver, failing_startup) {
  std::function<void(const Task &, AsyncTaskReceiver &)> task_callback =
      [](const Task &t, AsyncTaskReceiver &) {
        // should never be called
      };

  MockCommunicationFailingStartup comm;
  AsyncTaskReceiver sender(&comm, task_callback);

  bool sender_started = sender.start();
  EXPECT_FALSE(sender_started)
      << "When the communication fails to start, the sender should also fail";
}

namespace {  // prevent external linkage to other translation units
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
}  // anonymous namespace

TEST(task_receiver, failing_receive) {
  int task_callback_count = 0;
  std::function<void(const Task &, AsyncTaskReceiver &)> task_callback =
      [&](const Task &t, AsyncTaskReceiver &) {
        // should never be called
        task_callback_count++;
      };

  MockCommunicationFailingReceive comm;
  AsyncTaskReceiver sender(&comm, task_callback);
  sender.start();

  while (comm.recv_call_count < 2) {
    // Wait until received got called at least twice. This guarantees that the
    // sender's receive loop handled a failing receive at least once.
  }

  sender.stop();
  ASSERT_EQ(task_callback_count, 0);
}
