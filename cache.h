#include<bits/stdc++.h>
#ifndef CACHE_H
#define CACHE_H
#define READ 0
#define WRITE 1
#define L1_SETS 64
#define L1_WAYS 8
#define L2_SETS 512
#define L2_WAYS 8
#define L3_SETS 2048
#define L3_WAYS 16
#define TRUE 1
#define FALSE 0

using namespace std;

struct statistics
{
  int L1_hit;
  int L1_miss;
  int L2_hit;
  int L2_miss;
  int L3_hit;
  int L3_miss;
};

struct cache_line
{
  int valid;
  int state;
  unsigned tag;
};


 
struct cache_set
{
  cache_line *l;
};

class cache
{
 public:  

  int num_sets;
  int ways;
  cache_set *set;

  cache();
  void set_num_sets(int n);
  void set_ways(int n);
  void create_sets();
};

struct LRU_objects
{
  cache *L1,*L2,*L3;
  vector<vector<int> > L1_lruList, L2_lruList, L3_lruList;
  statistics stats;
};
  struct search_key
{
  unsigned index;
  unsigned tag;
};



#endif
