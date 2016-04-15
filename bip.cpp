#include "cache.h"
#include<iostream>
#include<vector>
#include<algorithm>
#include "func.h"
#include<cstdlib>

using namespace std;


void updateToLRUorMRU(int pos, vector<vector<int> > &lrulist, search_key* key)
{
  vector<int>::iterator it;
  it = find(lrulist[key->index].begin(), lrulist[key->index].end(), pos);
  lrulist[key->index].erase(it);
  int v1 = rand() % 100;
  if(v1>=80)
    lrulist[key->index].push_back(pos);
  else
    lrulist[key->index].insert(lrulist[key->index].begin(),pos);
}


void BimodalLIP(LRU_objects* lru, int state, unsigned addr)
{
  search_key *key1, *key2, *key3;
  key1 = new search_key;
  key2 = new search_key;
  key3 = new search_key;
  
  getKey(addr, 1, key1);
  getKey(addr, 2, key2);
  getKey(addr, 3, key3);
  //  cout << key1->index << " " << key1->tag<<endl;
  int pos1, pos2, pos3;
  int result1 = search_cache(lru->L1, key1);
  if(result1 == -1)
    {
      lru->stats.L1_miss++;
      pos1 = findToReplace(lru->L1, key1->index, lru);

      if(CheckIfDirty(lru->L1, key1->index, pos1) == 1)
        {
          unsigned tempaddr = getAddr(lru->L1, key1->index, pos1);
          search_key * temp;
          getKey(tempaddr<<6, 2, temp);
          updateInNextLevel(lru->L2,temp);
          delete temp;
        }
      
      insert_cache(lru->L1, key1, state, pos1);
      updateToLRUorMRU(pos1, lru->L1_lruList, key1);
      
      int result2 =  search_cache(lru->L2, key2);
      if(result2 == -1)
        {
          lru->stats.L2_miss++;
          pos2 = findToReplace(lru->L2, key2->index, lru);

          if(CheckIfDirty(lru->L2, key2->index, pos2) == 1)
            {
              unsigned tempaddr = getAddr(lru->L2, key2->index, pos2);
              search_key *temp;
              getKey(tempaddr<<6, 3, temp);
              updateInNextLevel(lru->L3,temp);
              delete temp;
            }

          insert_cache(lru->L2, key2, state, pos2);
          updateToLRUorMRU(pos2,lru->L2_lruList,key2);
          
          int result3;
          result3 = search_cache(lru->L3, key3);
          if(result3 == -1)
            {
              lru->stats.L3_miss++;
              pos3 = findToReplace(lru->L3, key3->index, lru);

              insert_cache(lru->L3, key3, state, pos3);
              updateToLRUorMRU(pos3, lru->L3_lruList,key3);
            }
          else
            {
              lru->stats.L_hit++;
              if(state == WRITE)
                lru->L3->set[key3->index].l[result3].state = state;
              updateToLRUorMRU(result3, lru->L3_lruList,key3);

            }
        }
      else
        {
          lru->stats.L_hit++;
          if(state == WRITE)
            lru->L2->set[key2->index].l[result2].state = state;
          updateToLRUorMRU(result2, lru->L2_lruList,key2);

        }
    }
  else
    {
      lru->stats.L_hit++;
      if(state == WRITE)
        lru->L1->set[key1->index].l[result1].state = state;
      updateToLRUorMRU(result1, lru->L1_lruList,key1);
    }
  delete key1;
  delete key2;
  delete key3;
  
}
