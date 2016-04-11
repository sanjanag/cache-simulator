#include<iostream>

#define SHARED 0
#define MODIFIED 1
#define FALSE 0
#define TRUE 1

using namespace std;

struct cache_line
{
  int valid;
  ADDRINT tag;
  int state;
};


struct cache_set
{
  cache_line* line;
};
  
class L1_cache
{
 public:
  int num_sets = 9;
  int ways = 8;
  cache_set set[num_sets];

  L1_cache()
    {
      for(int i=0; i<num_sets; i++)
        {
          cache_line line[ways];
          set[i].line = line;
          for(int i=0;i<ways;i++)
            {
              line[i].valid = FALSE;
              line[i].tag = 0;
              line[i].state = SHARED;
            }
        }
      
    }
};

int main()
{
  cache *L1 = new cache();
  cout << L1->set[0].state<<endl;
  
    return 0;
}
