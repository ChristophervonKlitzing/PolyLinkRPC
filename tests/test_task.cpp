#include <gtest/gtest.h>

#include "../include/PolyLinkRPC/task.h"

TEST(task, basic) {
  std::vector<std::string> types =
      std::vector<std::string>({"int", "float", "string"});
  std::string fname = "test_func";
  Task t(fname, types);
  ASSERT_EQ(t.get_num_args(), types.size());

  for (int i = 0; i < types.size(); ++i) {
    ASSERT_STREQ(t.get_argument(i).get_type().c_str(), types[i].c_str());
  }

  ASSERT_STREQ(t.get_function_name().c_str(), fname.c_str());

  // Test uniqueness of task-ids
  std::set<Task::task_id_t> task_ids;
  std::size_t num_tasks = 10;
  for (int i = 0; i < num_tasks; ++i) {
    Task ti("t");
    task_ids.insert(ti.get_id());
  }
  ASSERT_EQ(task_ids.size(), num_tasks);

  // Test adding arguments
  std::string new_arg_type = "bool";
  std::size_t arg_index = types.size();
  t.add_argument(new_arg_type);
  std::string actual_arg_type = t.get_argument(arg_index).get_type();
  ASSERT_STREQ(actual_arg_type.c_str(), new_arg_type.c_str());
}

TEST(task, serialization) {
  Task original_task("test_func", {"float"});

  BytesBuffer buffer;
  original_task.serialize(buffer);

  Task reconstructed_task = Task::deserialize(buffer);
  ASSERT_TRUE(original_task == reconstructed_task);
};

TEST(task, serialization_wrong_version) {
  Task original_task("test_func", {"float"});
  original_task.__change_version(INVALID_VERSION_NUMBER);

  BytesBuffer buffer;
  original_task.serialize(buffer);

  EXPECT_THROW(Task::deserialize(buffer), std::runtime_error);
}

TEST(task, version) {
  Task t("test_func", {"float"});
  ASSERT_EQ(t.get_version_number(), VERSION_NUMBER);
  ASSERT_STREQ(t.get_version_string().c_str(), VERSION_STRING);
}
