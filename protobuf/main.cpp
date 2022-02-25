#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <string.h>
#include <utility>
#include <fstream>
#include <sstream>

#include <google/protobuf/message.h>

#include "serialized_graph.pb.h"

std::shared_ptr<tutorial::SerializedGraph> Serialize() {
  auto msg = std::make_shared<tutorial::SerializedGraph>();
  for (int i = 0 ; i < 3 ; i++) {
    msg->add_tensors();
    auto ii = i + 'a';
    auto tensor = msg->mutable_tensors(i);
    tensor->add_shape(ii);
    tensor->add_shape(ii);
    tensor->add_shape(ii);
    tensor->add_stride(ii);
    tensor->add_stride(ii);
    tensor->add_stride(ii);
    tensor->set_offset(ii);
  }
  std::string s{"Hi my name is jw"};
  for (int i = 0 ; i < 4 ; i++) {
    msg->add_points();
    auto point_msg = msg->mutable_points(msg->points_size() - 1);
    point_msg->set_y(i + 100);
    point_msg->set_x(i + 200);
  }
  msg->set_data(0);
  return msg;
}

void Deserialize(std::shared_ptr<tutorial::SerializedGraph> msg) {
  for (auto tensor_msg: msg->tensors()) {
    for (auto shape: tensor_msg.shape()) {
      std::cout << shape << std::endl;
    }
  }
}

template <typename T>
void print_vector(std::vector<T>& v) {
  for (auto it: v) {
    std::cout << v << std::endl;
  }
}

int main() {
  auto msg = Serialize();
  std::ofstream fout;
  fout.open("serialized_graph.bin", std::ofstream::binary | std::ofstream::out);
  msg->SerializeToOstream(&fout);
  fout.close();


  std::ifstream fin;
  fin.open("serialized_graph.bin", std::ifstream::binary | std::ifstream::in);
  tutorial::SerializedGraph new_msg;
  std::stringstream buffer;
  buffer << fin.rdbuf();
  std::string new_string = buffer.str();
  new_msg.ParseFromString(new_string);
  
  std::cout << msg->DebugString() << std::endl;
  std::cout << "------------" << std::endl;
  std::cout << new_msg.DebugString() << std::endl;
  std::cout << "------------" << std::endl;
  {
    auto wrapper = std::make_shared<tutorial::Wrapper>();
    wrapper->set_allocated_graph(&new_msg);
    std::cout << wrapper->DebugString() << std::endl;
    wrapper->release_graph();
  }
  return 0;
}
