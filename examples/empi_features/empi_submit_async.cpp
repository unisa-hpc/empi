#include "empi/datatype.hpp"
#include <iostream>

#include <empi/empi.hpp>
#include <empi/tag.hpp>
#include <numeric>

int main(int argc, char **argv) {

  empi::Context ctx(&argc, &argv);
  auto message_group = ctx.create_message_group(MPI_COMM_WORLD);
  int number;
  std::vector<int> v(1);
  constexpr empi::Tag tag{0};

  // with fixed-tag and type message group handler
  message_group->run([&](empi::MessageGroupHandler<int, tag, 1> &mgh) {
	if (message_group->rank() == 0) {
	  number = 15;
	  auto event = mgh.Isend(&number,1);
	  event->wait<empi::details::no_status>();
	  std::cout << "Message arrived!\n";
	} else if (message_group->rank() == 1) {
	  MPI_Status status;
	  mgh.recv(v,0,status);

	  printf("Process 1 received number %d from process 0\n", v[0]);
	}
  });

  return 0;
}
