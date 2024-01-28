#include <gtest/gtest.h>

#include <functional>

#include "PolyLinkRPC/local_communication.h"
#include "PolyLinkRPC/task_receiver.h"
#include "PolyLinkRPC/task_sender.h"

TEST(local_communication, basic) {
  // Initialize data variables for testing
  std::vector<Datagram::id_t> task_send_order = {1, 2, 3, 4};
  std::vector<Datagram::id_t> result_receive_order;

  // use timeout to ensure not to block the unit-tests
  auto comm_pair = LocalCommunication::create_communication_pair(100, 100);

  // Create task sender
  std::function<void(const Result &)> on_res_callback = [&](const Result &res) {
    result_receive_order.push_back(res.get_id());
  };
  TaskSender sender(&comm_pair.sender_comm, on_res_callback);

  // Create task receiver
  auto on_task_callback = [](const Task &t, TaskReceiver &receiver) {
    Result res;
    res.set_id(t.get_id());
    receiver.submit_result(std::move(res));
  };
  TaskReceiver receiver(&comm_pair.receiver_comm, on_task_callback);

  ASSERT_TRUE(sender.start());
  ASSERT_TRUE(receiver.start());

  for (auto id : task_send_order) {
    Task t;
    t.set_id(id);
    sender.submit_task(t);
  }
  while (result_receive_order.size() < task_send_order.size()) {
    // wait for all results
  }

  ASSERT_TRUE(task_send_order == result_receive_order);
}