#include<iostream>
#include<stdlib.h>
#include "cache.h"
#define SHARED 0
#define MODIFIED 1
#define FALSE 0
#define TRUE 1

  
using namespace std;

cache::cache()
{
  num_sets=0;
}
void cache::set_num_sets(int n)
{
  num_sets = n;
}
void cache::set_ways(int n)
{
  ways = n;
}
void cache::create_sets()
{
  set =(cache_set*) malloc(num_sets*sizeof(cache_set));
  for(int i=0; i<num_sets; i++)
    {
      set[i].l  = (cache_line*)malloc(ways*sizeof(cache_line));
      
        for(int j=0; j<ways; j++)
          {
            set[i].l[j].valid = FALSE;
            set[i].l[j].tag = 0;
            set[i].l[j].state = SHARED;
          }
        
    }
}
