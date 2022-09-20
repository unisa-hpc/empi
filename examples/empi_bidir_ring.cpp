#include "empi.h"
#include <iostream>

int main(argc,argv) {  
  empi::Context ctx(&argc, &argv);
  
  int prev = ctx.rank()-1;
  int next = ctx.rank()+1;
  if (ctx.rank() == 0) prev = ctx.size() - 1;
  if (ctx.rank() == (ctx.size() - 1)) next = 0;

  const tag1 = 1;
  const tag2 = 2;
  
   // with fixed-tag and type message context
  ctx.run_and_wait<type>([](empi::MessageGroupHandler &mgh){  
      mgh.irecv<tag1>(&buf[0], 1, prev);
      mgh.irecv<tag2>(&buf[1], 1, next);
      mgh.isend<tag1>(&buf[1], 1, prev);
      mgh.isend<tag2>(&buf[0], 1, next);
  }); // Waitall is implicit here
  
  return 0; // Finalize is implicit here
}
