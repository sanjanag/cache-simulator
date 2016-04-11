#include<iostream>
#include<stdlib.h>
#define SHARED 0
#define MODIFIED 1
#define FALSE 0
#define TRUE 1

using namespace std;

struct cache_line
{
  int valid;
  unsigned long int tag;
  int state;
  
};


struct cache_set
{
  cache_line* line;
};
  
class cache
{
public:
  int num_sets;
  int ways;
  cache_set * set;
  
  void set_num_sets(int n)
  {
    num_sets = 4;
  }
  void set_ways(int n)
  {
    ways = 4;
  }
  void create_sets()
  {
    set =(cache_set*) malloc(num_sets*sizeof(cache_set));
    for(int i=0; i<num_sets; i++)
      {
        set[i].line  = (cache_line*)malloc(ways*sizeof(cache_line));
        
        for(int j=0; j<ways; j++)
          {
            set[i].line[j].valid = FALSE;
            set[i].line[j].tag = 0;
            set[i].line[j].state = SHARED;
          }
      
      }
  }
};

int main()
{
  cache L1;
  L1.set_num_sets(64);
  L1.set_ways(8);
  L1.create_sets();
  cache L2;
  L1.set_num_sets(512);
  L1.set_ways(8);
  L1.create_sets();
  cache L3;
  L1.set_num_sets(2048);
  L1.set_ways(16);
  L1.create_sets();

  return 0;
}
