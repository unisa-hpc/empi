#include <iostream>

#include <empi/empi.hpp>
#include <empi/tag.hpp>
#include <numeric>

int main(int argc, char **argv){

  empi::Context ctx(&argc, &argv);

    int number;
	std::vector<int> v(1);
  	constexpr empi::Tag tag{0};
     // with fixed-tag and type message group handler
    ctx.run([&](empi::MessageGroupHandler<int,tag,0> &mgh){
       if (ctx.rank() == 0) {
           number = 10;
           mgh.send(std::span{&number, 1}, 1); // CTAD
       }else if (ctx.rank() == 1) {
         mgh.recv(v, 0);
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
