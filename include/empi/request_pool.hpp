#ifndef INCLUDE_EMPI_REQUEST_POOL
#define INCLUDE_EMPI_REQUEST_POOL

#include "empi/async_event.hpp"
#include "mpi.h"
#include <memory>
#include <vector>
#include <iostream>

namespace empi {

// TODO:
class request_pool {
public:
  explicit request_pool(size_t size)
      : data(size), window(default_windows_size) {
    for (auto &req : data)
      req = std::make_shared<async_event>();
    head = 0;
    tail = size - 1;
  }

  explicit request_pool() : request_pool(default_pool_size) {}

  // TODO: clean req before return?
  std::shared_ptr<async_event>& get_req() {
    auto& req = data.at(head);
    head = (head + 1) % data.size();
    if (tail == head && move_tail() == 0) {
      // Expand
      const size_t old_size = data.size();
      const auto new_size = default_pool_size * window;
      data.resize(new_size); // TODO: Maybe reserve will be more optimized
      window = window << 2;
      auto &&end = data.begin() + static_cast<long>(new_size);
      for (auto &&iter = data.begin() + static_cast<long>(old_size);
           iter != end; iter++) {
        *iter = std::make_shared<async_event>();
      }
      tail = new_size;
    }
    return req;
  }

  auto compact() -> void {
    // TODO: Compact request and free memory
  }

  void waitall(){
    int err;
    tail = (tail + 1) % data.size();
    while(tail != head){
      err = MPI_Wait(data[tail]->get_request(), MPI_STATUS_IGNORE);
      if(err == MPI_ERR_REQUEST)
        throw std::runtime_error("Wait on invalid request within request_pool. This should never happen");
      tail = (tail + 1) % data.size();
    }
  }

  constexpr static size_t default_pool_size = 1000;
  constexpr static size_t default_windows_size = 2;

private:

  auto move_tail() -> int {
    int flag = 0;
    int mov = 0;
    tail = (tail + 1) % data.size();
    while (tail != head) {
      int err = MPI_Test(data[tail]->get_request(), &flag, MPI_STATUS_IGNORE);
      if (err != MPI_ERR_REQUEST && !flag)
        break;
      tail = (tail + 1) % data.size();
      mov++;
    }
    return mov;
  }

  std::vector<std::shared_ptr<async_event>> data;
  size_t head;
  size_t tail;
  size_t window;
};

} // namespace empi

#endif /* INCLUDE_EMPI_REQUEST_POOL */
