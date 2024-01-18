

#include <iostream>

#include "PolyLinkRPC/task.h"
#include "PolyLinkRPC/versions.h"

int main() {
  std::cout << "Hello world" << std::endl;
  Task t("foo", {"int", "float"});
  std::cout << t.get_function_name() << t.get_num_args()
            << t.get_argument(1).get_type() << std::endl;

  uint16_t vn = versionStringToNumber("31.31.63");

  std::cout << versionNumberToString(vn) << std::endl;
}
