#include <iostream>

#include <empi/empi.hpp>
#include <numeric>

int main(int argc, char **argv) {

  empi::Context ctx(&argc, &argv);

  auto message_group = ctx.create_message_group(MPI_COMM_WORLD);
  std::cout << "Rank: " << ctx.rank() << "\n";

  int number[5];
  constexpr empi::Tag tag{5};
  // without message group handler
  if (ctx.rank() == 0) {
	std::iota(number, number + 5, 1);
	message_group->send<5>(number, 1, tag);
  } else if (ctx.rank() == 1) {
	MPI_Status status;
	message_group->recv<tag, 5>(std::span{number, 5}, 0, status);
	for (int i : number) {
	  std::cout << i << " ";
	}
	std::cout << "\n";
  }

  return 0;
}
