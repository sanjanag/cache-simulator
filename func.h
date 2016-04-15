#ifndef FUNC_H
#define FUNC_H

#include <vector>
#include "cache.h"

using namespace std;

extern cache * create_L1(int sets, int ways);
extern void initL_lruList(vector<vector<int> > &L_lruList, int sets, int ways);
extern void init_LRU(LRU_objects * lru);
extern void getKey(unsigned addr, int level, search_key* key);
extern int search_cache(cache* L, search_key* key);
extern int find_invalid(cache* L,unsigned index);
extern int findLRU(cache *L, int index, LRU_objects* lru);
extern int findToReplace(cache* L,unsigned index, LRU_objects* lru);
extern void insert_cache(cache* L, search_key* key, int state, int pos );
extern void updateToMRU(int pos, vector<vector<int> > &lrulist, search_key* key);
extern int CheckIfDirty(cache* L, unsigned index, int pos);
extern unsigned getAddr(cache* L,unsigned index, int pos);
extern void updateInNextLevel(cache* L,search_key * key );
extern void LeastRecentlyUsed(LRU_objects* lru, int state, unsigned addr);
extern void print_stats(statistics stats);
extern void LIP(LRU_objects* lru, int state, unsigned addr);
extern void BimodalLIP(LRU_objects* lru, int state, unsigned addr);
extern int findRandom(cache *L);
extern void RandomReplace(LRU_objects* lru, int state, unsigned addr);
#endif
