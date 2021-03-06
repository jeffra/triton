﻿#include "triton/driver/backend.h"
#include "triton/driver/stream.h"
#include "dot.h"
#include "util.h"

int main() {
  // initialize default compute device
  auto context = triton::driver::backend::contexts::get_default();
  triton::driver::stream* stream = triton::driver::stream::create(context);
  // shapes to test
  typedef std::tuple<dtype_t, bool, bool, int, int, int, int, int, int, int> config_t;
  std::vector<config_t> configs;
  for(int TM: std::vector<int>{32, 64, 128})
  for(int TN: std::vector<int>{32, 64, 128})
  for(int TK: std::vector<int>{16})
  for(int nwarps: std::vector<int>{4})
  for(bool AT: std::array<bool, 2>{false, true})
  for(bool BT: std::array<bool, 2>{false, true}){
    configs.push_back(config_t{FLOAT, AT, BT, TM, TN, TK, TM, TN, TK, nwarps});
  }
  // test
  dtype_t dtype;
  bool AT, BT;
  int M, N, K, TM, TN, TK, nwarp;
  for(const auto& c: configs){
    std::tie(dtype, AT, BT, M, N, K, TM, TN, TK, nwarp) = c;
    std::cout << "Testing " << c << " ... " << std::flush;
    if(test_dot(stream, dtype, AT, BT, M, N, K, {0, 1}, {0, 1}, TM, TN, TK, (size_t)nwarp))
      std::cout << " Pass! " << std::endl;
    else{
      std::cout << " Fail! " << std::endl;
    }
  }
}
