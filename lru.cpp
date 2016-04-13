#include "cache.h"
#include<iostream>
#include<vector>
#include<algorithm>
#include "func.h"

using namespace std;


void create_L(cache* L,int sets, int ways)
{
  //  cout << "//" << sets << "//" << ways << endl;
  L->set_num_sets(sets);
  L->set_ways(ways);
  L->create_sets();
}

void initL_lruList(vector<vector<int> > &L_lruList, int sets, int ways)
{
  for(int i = 0; i<sets; i++)
    {
      vector<int> myvector;
      for(int j = 0; j<ways; j++)
        myvector.push_back(j);
      L_lruList.push_back(myvector);
    }
}

void init_LRU(LRU_objects * lru)
{
  create_L(lru->L1, L1_SETS, L1_WAYS);
  create_L(lru->L2, L2_SETS, L2_WAYS);
  create_L(lru->L3,L3_SETS, L3_WAYS);
  lru->stats.L_hit = 0;
  lru->stats.L1_miss = 0;
  lru->stats.L2_miss = 0;
  lru->stats.L3_miss = 0;
  initL_lruList(lru->L1_lruList, L1_SETS, L1_WAYS);
  initL_lruList(lru->L2_lruList, L2_SETS, L2_WAYS);
  initL_lruList(lru->L3_lruList, L3_SETS, L3_WAYS);
}

void getKey(unsigned addr, int level, search_key* key)
{
 
  key->tag =0;
  key->index = 0;
  unsigned tag_index = addr >> 6;
  if(level==1)
    {
      key->index = tag_index & 63;
      key->tag = tag_index >> 6;
    }
  else if(level == 2)
    {
      key->index = tag_index & 511;
      key->tag = tag_index >> 9;
    }
  else if (level == 3)
    {
      key->index = tag_index & 2047;
      key->tag = tag_index >> 11;
    }
}

int search_cache(cache* L, search_key* key)
{
  int i;
  unsigned index = key->index;
  unsigned tag = key->tag;
  
  for(i=0;i<L->ways;i++)
    {
      if(L->set[index].l[i].tag == tag && L->set[index].l[i].valid == TRUE)
        return i;
    }
  
  return -1;
}

int find_invalid(cache* L,unsigned index)
{
  int i;
  for(i = 0; i<L->ways; i++)
    if(L->set[index].l[i].valid == FALSE)
      return i;
  if(i == L->ways)
    return -1;
}

int findLRU(cache *L, int index, LRU_objects* lru)
{
  if(L->num_sets == L1_SETS)
      return lru->L1_lruList[index].back();
  else if(L->num_sets == L2_SETS)
      return lru->L2_lruList[index].back();
  else if(L->num_sets == L3_SETS)
      return lru->L3_lruList[index].back();
}

int findToReplace(cache* L,unsigned index, LRU_objects* lru)
{
  int pos = find_invalid(L, index);
  if(pos == -1)
    {
      return findLRU(L,index,lru);
    }
  else
    return pos;
}

void insert_cache(cache* L, search_key* key, int state, int pos )
{
  cache_line *l = L->set[key->index].l;
  l[pos].state = state;
  l[pos].tag = key->tag;
  l[pos].valid = TRUE;
}

void updateToMRU(int pos, vector<vector<int> > &lrulist, search_key* key)
{
  vector<int>::iterator it;
  it = find(lrulist[key->index].begin(), lrulist[key->index].end(), pos);
  lrulist[key->index].erase(it);
  lrulist[key->index].insert(lrulist[key->index].begin(), pos);
}

int CheckIfDirty(cache* L, unsigned index, int pos)
{
  if(L->set[index].l[pos].valid == TRUE && L->set[index].l[pos].state == WRITE)
    return 1;
  else
    return -1;
}

unsigned getAddr(cache* L,unsigned index, int pos)
{
  unsigned addr;
  if(L->num_sets == L1_SETS)
    addr = (L->set[index].l[pos].tag <<6) + index;
  else if(L->num_sets == L2_SETS)
    addr = (L->set[index].l[pos].tag << 9) + index;
  else if(L->num_sets == L3_SETS)
    addr = (L->set[index].l[pos].tag << 11) + index;
  return addr;
}

void updateInNextLevel(cache* L,search_key * key )
{
  int  pos = search_cache(L, key);
  L->set[key->index].l[pos].state = WRITE;
}

void LeastRecentlyUsed(LRU_objects* lru, int state, unsigned addr)
{
  search_key *key1, *key2, *key3;
  key1 = new search_key;
  key2 = new search_key;
  key3 = new search_key;
  
  getKey(addr, 1, key1);
  getKey(addr, 2, key2);
  getKey(addr, 3, key3);
  cout << key1->index << " " << key1->tag<<endl;
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
      updateToMRU(pos1, lru->L1_lruList, key1);
      
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
          updateToMRU(pos2,lru->L2_lruList,key2);
          
          int result3;
          result3 = search_cache(lru->L3, key3);
          if(result3 == -1)
            {
              lru->stats.L3_miss++;
              pos3 = findToReplace(lru->L3, key3->index, lru);

              insert_cache(lru->L3, key3, state, pos3);
              updateToMRU(pos3, lru->L3_lruList,key3);
            }
          else
            {
              lru->stats.L_hit++;
              if(state == WRITE)
                lru->L3->set[key3->index].l[result3].state = state;
              updateToMRU(result3, lru->L3_lruList,key3);

            }
        }
      else
        {
          lru->stats.L_hit++;
          if(state == WRITE)
            lru->L2->set[key2->index].l[result2].state = state;
          updateToMRU(result2, lru->L2_lruList,key2);

        }
    }
  else
    {
      lru->stats.L_hit++;
      if(state == WRITE)
        lru->L1->set[key1->index].l[result1].state = state;
      updateToMRU(result1, lru->L1_lruList,key1);
    }
  delete key1;
  delete key2;
  delete key3;
  
}

void print_stats(statistics stats)
{
  cout << "hits " << stats.L_hit << endl;
  cout << "L1 misses " << stats.L1_miss << endl;
  cout << "L2 misses " << stats.L2_miss << endl;
  cout << "L3 misses " << stats.L3_miss << endl;
}

