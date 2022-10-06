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
  if(ctx.rank() == 0)
  	std::fill(src.begin(),src.end(),10);

  // with fixed-tag and type message context
  message_group->run_and_wait([&](empi::MessageGroupHandler<double, empi::NOTAG, size> &mgh){
  	//mgh.Bcast(src,0);
  	auto event = mgh.Ibcast(src,0);
	mgh.Allreduce(src,dest,MPI_SUM);
  }); // Waitall is implicit here

	message_group->Allreduce<size>(src, dest, MPI_SUM);

  for(auto& e : dest){
	if(e != (10 * ctx.size())){
	  std::cerr << "TEST FAILED\n";
	  return -1;
	}
  }
  std::cout << "TEST PASSED\n";
  return 0; // Finalize is implicit here
}
