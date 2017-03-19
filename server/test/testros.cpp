#include "testros.h"
#include <chrono>
#include <cinttypes>
#include <memory>
//#include "example_interfaces/srv/add_two_ints.hpp"
//#include "rclcpp/rclcpp.hpp"
//using AddTwoInts = example_interfaces::srv::AddTwoInts;

testROS::testROS()
{

}

int testROS::AddTwoInts(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  auto node = rclcpp::node::Node::make_shared("minimal_client");
  auto client = node->create_client<AddTwoInts>("add");
  while (!client->wait_for_service(std::chrono::seconds(1))) {
    if (!rclcpp::ok()) {
      printf("client interrupted while waiting for service to appear.\n");
      return 1;
    }
    printf("waiting for service to appear...\n");
  }
  auto request = std::make_shared<AddTwoInts::Request>();
  request->a = 41;
  request->b = 1;
  auto result_future = client->async_send_request(request);
  if (rclcpp::spin_until_future_complete(node, result_future) !=
    rclcpp::executor::FutureReturnCode::SUCCESS)
  {
    printf("service call failed :(\n");
    return 1;
  }
  auto result = result_future.get();
  printf("result of %" PRId64 " + %" PRId64 " = %" PRId64 "\n",
    request->a, request->b, result->sum);
  return 0;
}

}
