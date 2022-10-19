#include <empi/empi.hpp>
#include <iostream>
#include <numeric>

int main(int argc, char** argv) {

  empi::Context ctx(&argc, &argv);

  constexpr empi::Tag tag1(1);
  constexpr empi::Tag tag2(2);
  constexpr int size = 200;

  auto message_group = ctx.create_message_group(MPI_COMM_WORLD);

  std::vector<double> src(size);
  std::vector<double> dest(size);
//  std::fill(src.begin(),src.end(),message_group->rank());

  std::cout << "RANK: " << message_group->rank() << " - prev: " << message_group->prec() << " - succ: " << message_group->next() << "\n";
  // with fixed-tag and type message context
  message_group->run_and_wait([&](empi::MessageGroupHandler<double, empi::NOTAG, size> &mgh){
      mgh.Isend(src, message_group->next(), tag1);
      mgh.Irecv(dest, message_group->prec(), tag1);
  }); // Waitall is implicit here

  for(auto& e : dest){
	if(e != message_group->prec()){
	  std::cerr << "TEST FAILED\n";
	  return -1;
	}
  }
  std::cout << "TEST PASSED\n";
  return 0; // Finalize is implicit here
}
