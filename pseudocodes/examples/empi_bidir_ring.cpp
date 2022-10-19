#include "empi.h"
#include <iostream>

int main(argc,argv) {  
  empi::Context ctx(&argc, &argv);
  
  int prev = message_group->rank()-1;
  int next = message_group->rank()+1;
  if (message_group->rank() == 0) prev = message_group->size() - 1;
  if (message_group->rank() == (message_group->size() - 1)) next = 0;

  const tag1 = 1;
  const tag2 = 2;
  const int size = 1
  
   // with fixed-tag and type message context
  ctx.run_and_wait<type,size>([](empi::MessageGroupHandler &mgh){  
      mgh.irecv<tag1>(&buf[0], prev);
      mgh.irecv<tag2>(&buf[1], next);
      mgh.isend<tag1>(&buf[1], prev);
      mgh.isend<tag2>(&buf[0], next);
  }); // Waitall is implicit here
  
  return 0; // Finalize is implicit here
}
