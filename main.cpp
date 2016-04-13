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
  fp = fopen("pinatrace.out","r");
  int i = 0;
  LRU_objects* lru = new LRU_objects;
  lru->L1 = new cache;
  lru->L2 = new cache;
  lru->L3 = new cache;
  init_LRU(lru);
  
  while((fscanf(fp,"%c %u\n",&s, &addr))!= EOF)
    {
      if(state == 'R')
        state = READ;
      else if (state == 'W')
        state = WRITE;
      else
        int t =1;
        
      printf("%c %u\n", s, addr);
      LeastRecentlyUsed(lru,state,addr);
      i++;
    }
  cout << i<<endl;
  print_stats(lru->stats);
  fclose(fp);
  delete lru->L1;
  delete lru->L2;
  delete lru->L3;
  delete lru;
  
  return 0;
}
