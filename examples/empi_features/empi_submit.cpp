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
  message_group->run([&](empi::MessageGroupHandler<int, tag, 0> &mgh) {
	if (ctx.rank() == 0) {
	  number = 15;
	  mgh.send(&number, 1, 1);
	} else if (ctx.rank() == 1) {
	  MPI_Status status;
	  mgh.recv(v, 0, 1, status);
	  printf("Process 1 received number %d from process 0\n", v[0]);
	}
  });



  // // with fixed-tag, fixed-type and fixed-size message group handler
  // const int size = 1;
  // ctx.run<tag,type,size>([](empi::MessageGroupHandler &mgh){  
  //   if (ctx.rank() == 0) {
  //     mgh.send(&number, 1); // CTAD
  //   else if (ctx.rank() == 1) {
  //     mgh.recv(&number, 0);
  //     printf("Process 1 received number %d from process 0\n", number);
  //   }   
  // });       

  return 0;
}
