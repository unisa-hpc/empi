#include <empi.h>  
#include <stdio.h>

int main(int argc, char **argv){

  empi::Context ctx(&argc, &argv);
  
  int number = -1;
  const tag = 0;
  const type = MPI_INT;
  
  // without message group handler
  if (ctx.rank() == 0) {
    ctx.send<tag,type>(&number, 1, 1);
  else if (ctx.rank() == 1) {
    ctx.recv<tag,type>(&number, 1, 0);
    printf("Process 1 received number %d from process 0\n", number);
  }
    
  // with fixed-tag and type message group handler
  ctx.run<tag,type>([](empi::MessageGroupHandler &mgh){  
    if (ctx.rank() == 0) {
      mgh.send(&number, 1, 1); // CTAD
    else if (ctx.rank() == 1) {
      mgh.recv(&number, 1, 0);
      printf("Process 1 received number %d from process 0\n", number);
    }   
  });    
    
  // with fixed-tag, fixed-type and fixed-size message group handler
  const int size = 1;
  ctx.run<tag,type,size>([](empi::MessageGroupHandler &mgh){  
    if (ctx.rank() == 0) {
      mgh.send(&number, 1); // CTAD
    else if (ctx.rank() == 1) {
      mgh.recv(&number, 0);
      printf("Process 1 received number %d from process 0\n", number);
    }   
  });       
    
  return 0;
}
