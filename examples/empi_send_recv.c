#include <mpi.h>  
#include <stdio.h>

int main(int argc, char **argv){

  EMPI_Context ctx(&argc, &argv);
  
  int number = -1;
  const tag = 0;
  const type = MPI_INT;
  
  // without message context
  if (ctx.rank() == 0) {
    ctx.send<tag,type>(&number, 1, 1);
  else if (ctx.rank() == 1) {
    ctx.recv<tag,type>(&number, 1, 0);
    printf("Process 1 received number %d from process 0\n", number);
  }
    
  // with fixed-tag and type message context
  ctx.submit<tag,type>([](EMPI_TagTypeContext &tt_ctx){  
    if (ctx.rank() == 0) {
      tt_ctx.send(&number, 1, 1);
    else if (ctx.rank() == 1) {
      tt_ctx.recv(&number, 1, 0);
      printf("Process 1 received number %d from process 0\n", number);
    }   
  });    
    
  return 0;
}
