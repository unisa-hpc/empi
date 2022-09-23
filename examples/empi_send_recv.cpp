#include <iostream>

#include <empi/empi.hpp>
#include <numeric>

int main(int argc, char **argv){

  empi::Context ctx(&argc, &argv);
  int number[5];
  // without message group handler
  if (ctx.rank() == 0) {
    std::iota(number, number+5,1);
    ctx.send<int, empi::Tag{0}>(std::span{number, 5}, 1);
  } else if (ctx.rank() == 1) {
    ctx.recv<int, empi::Tag{0}>(std::span{number, 5}, 0);

    for (int i: number) {
          std::cout << i << " ";
    }
    std::cout << "\n";
  }
    
  // // with fixed-tag and type message group handler
  // ctx.run<tag,type>([](empi::MessageGroupHandler &mgh){  
  //   if (ctx.rank() == 0) {
  //     mgh.send(&number, 1, 1); // CTAD
  //   else if (ctx.rank() == 1) {
  //     mgh.recv(&number, 1, 0);
  //     printf("Process 1 received number %d from process 0\n", number);
  //   }   
  // });    
    
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
