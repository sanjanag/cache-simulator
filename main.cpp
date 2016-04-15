#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <typeinfo>
#include <vector>
#include "cache.h"
#include "func.h"

using namespace std;


int main()
{
  //read input
  char s;
  unsigned addr;
  int state;
  FILE* fp;
  fp = fopen("./mem_trace_rw/xalanbmk.diffmail.out3","r");
  long long int i = 0;
  LRU_objects* lru1 = new LRU_objects;
  lru1->L1 = new cache;
  lru1->L2 = new cache;
  lru1->L3 = new cache;
  init_LRU(lru1);
  LRU_objects* lru2 = new LRU_objects;
  lru2->L1 = new cache;
  lru2->L2 = new cache;
  lru2->L3 = new cache;
  init_LRU(lru2);
  LRU_objects* lru3 = new LRU_objects;
  lru3->L1 = new cache;
  lru3->L2 = new cache;
  lru3->L3 = new cache;
  init_LRU(lru3);

  while((fscanf(fp,"%c%x , ",&s, &addr))!= EOF)
    {
      if(state == 'r')
        state = READ;
      else if (state == 'w')
        state = WRITE;
      LeastRecentlyUsed(lru1,state,addr);
      LIP(lru2, state, addr);
      BimodalLIP(lru3,state,addr);
      i++;
    }

  cout << i<<endl;
  print_stats(lru1->stats);
	print_stats(lru2->stats);
print_stats(lru3->stats);
  fclose(fp);
  
  delete lru1->L1;
  delete lru1->L2;
  delete lru1->L3;
  delete lru1;
  delete lru2->L1;
  delete lru2->L2;
  delete lru2->L3;
  delete lru2;
  delete lru3->L1;
  delete lru3->L2;
  delete lru3->L3;
  delete lru3;

  return 0;
}
