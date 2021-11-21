#ifndef HIERARCHYHASH_H
#define HIERARCHYHASH_H

#include <iostream>
#include "FlatHash.h"

#define TOMBSTONE 9999999

class HierarchyHash
{
private:
  unsigned int** hashtable;
  // Variable for overflow handling
  enum overflow_handle flag;
  // Loading factor
  float alpha;
  // Size(range) of full hash table. Initially 1000
  unsigned int table_size;
  // Size of subhash table. Fixed by 100
  unsigned int sub_table_size;
  // Size of main hash table
  unsigned int main_table_size;
  // Nums of keys
  unsigned int num_of_keys;
  // Nums of subhash table's keys
  unsigned int* num_of_subkeys;

public:
  HierarchyHash(enum overflow_handle _flag, float _alpha);

  ~HierarchyHash();

  unsigned int hashFunction(const unsigned int key) { return key % table_size; }

  unsigned int getTableSize() { return table_size; }

  unsigned int getNumofKeys() { return num_of_keys; }

  // Return the size of allocated sub hash table
  unsigned int getAllocatedSize();

  // Return time cost
  int insert(const unsigned int key);
  void resize();

  // Return time cost
  void shifting(const unsigned int startMain, const unsigned int startSub);
  int remove(const unsigned int key);
  void deleteSub();

  // Return time cost
  int search(const unsigned int key);

  // Delete tombstones
  void clearTombstones();

  void print();
};

HierarchyHash::HierarchyHash(enum overflow_handle _flag, float _alpha)
{
  // Initial map size is 1000
  table_size = 1000;
  // Table size is fixed to 100
  sub_table_size = 100;
  num_of_keys = 0;
  flag = _flag;
  alpha = _alpha;

  // Write your code
  main_table_size = table_size/sub_table_size;
	hashtable = new unsigned int *[main_table_size];
  num_of_subkeys = new unsigned int [main_table_size]();
  for(unsigned int i=0; i<main_table_size; i++){
    hashtable[i] = NULL;
	}
}

HierarchyHash::~HierarchyHash()
{
  // Write your code
  for(unsigned int i=0; i<main_table_size; i++){
    if(hashtable[i]) delete[] hashtable[i];
	}
  delete hashtable;
  delete[] num_of_subkeys;
}

unsigned int HierarchyHash::getAllocatedSize()
{
  // Write your code
	unsigned int allocatedSize=0;
	for(unsigned int i=0; i<main_table_size; i++){
    if(hashtable[i]) allocatedSize+=sub_table_size;
  }
  return allocatedSize;
}

int HierarchyHash::insert(const unsigned int key)
{
  // Write your code
  int timeCost = 1;
  int searchCost = search(key);
  unsigned int index = hashFunction(key); // real index
  unsigned int mainIndex = index / sub_table_size; // maintable 내의 subtable 위치
  unsigned int subIndex = index % sub_table_size; // subtable 내의 index
  bool isLinear = flag == LINEAR_PROBING ? true : false;

  if(searchCost > 0) { // 중복
    return -1*searchCost;
  } else if(hashtable[mainIndex]){ // subtable이 할당되어 있으면
    if(hashtable[mainIndex][subIndex]==0 || hashtable[mainIndex][subIndex]==TOMBSTONE){ // useable
      hashtable[mainIndex][subIndex] = key;
      num_of_keys++;
      num_of_subkeys[mainIndex]++;
    } else { //overflow
      for(unsigned int i=1;;i++){
        timeCost++;
        index = hashFunction(isLinear ? key+i : key+(i*i));
        mainIndex = index / sub_table_size;
        subIndex = index % sub_table_size;

        if(hashtable[mainIndex]){ // 새로 바뀐 index에 대하여 subtable이 할당되어 있으면
          if(hashtable[mainIndex][subIndex]==0 || hashtable[mainIndex][subIndex]==TOMBSTONE){
            hashtable[mainIndex][subIndex] = key;
            num_of_keys++;
            num_of_subkeys[mainIndex]++;
            break;
          }
        } else { // 새로 바뀐 index에 대하여 subtable이 할당되어 있지않으면
            hashtable[mainIndex] = new unsigned int [sub_table_size]();
            hashtable[mainIndex][subIndex] = key;
            num_of_keys++;
            num_of_subkeys[mainIndex]++;
            break;
        }

        if(!isLinear && (unsigned int)timeCost >= table_size){ // linear로 변경 후 다시 시작
          i=0; // reset
          isLinear = true;
          timeCost++;
        }
      }
    }
  } else { // subtable이 할당되어 있지않으면
    hashtable[mainIndex] = new unsigned int [sub_table_size]();
    hashtable[mainIndex][subIndex] = key;
    num_of_keys++;
    num_of_subkeys[mainIndex]++;
  }

  if((double)num_of_keys/table_size >= alpha) resize(); // resize 필요 여부 확인
  return timeCost;
}

void HierarchyHash::resize(){
  unsigned int** origintable = hashtable;
  table_size*=2;

  // 초기화
  num_of_keys = 0;
  main_table_size = table_size/sub_table_size;
	hashtable = new unsigned int *[main_table_size];
  num_of_subkeys = new unsigned int [main_table_size]();
  for(unsigned int i=0; i<main_table_size; i++){
    hashtable[i] = NULL;
	}

  for(unsigned int i=0; i<main_table_size/2; i++){
    if(origintable[i] == NULL) continue;
    for(unsigned int j=0; j<sub_table_size; j++){
      if(origintable[i][j]!=0 && origintable[i][j]!=TOMBSTONE) insert(origintable[i][j]);
      else if(flag == QUADRATIC_PROBING) hashtable[i][j] = TOMBSTONE;
    }
	}
  delete origintable;
}

void HierarchyHash::shifting(const unsigned int startMain, const unsigned int startSub){
  unsigned int key; // shift할 key
  unsigned int mainIndex; // shift할 key의 현재 index
  unsigned int subIndex; // shift할 key의 현재 index
  unsigned int currMainIndex = startMain; // 탐색 index
  unsigned int currSubIndex = startSub; // 탐색 index
  unsigned int tmpMainIndex; // 바꿀 위치 탐색 index
  unsigned int tmpSubIndex; // 바꿀 위치 탐색 index

  for(; currMainIndex < main_table_size; currMainIndex++){
    if(hashtable[currMainIndex]==NULL) continue;
    for(; currSubIndex < sub_table_size; currSubIndex++){
      if(hashtable[currMainIndex][currSubIndex]!=0 && hashtable[currMainIndex][currSubIndex]!=TOMBSTONE){
        mainIndex = currMainIndex;
        subIndex = currSubIndex;
        key = hashtable[mainIndex][subIndex];

        for(unsigned int i=0;;i++){
          tmpSubIndex = hashFunction(flag == LINEAR_PROBING ? key+i : key+(i*i));
          tmpMainIndex = tmpSubIndex / sub_table_size;
          tmpSubIndex = tmpSubIndex % sub_table_size;
          if(tmpMainIndex > mainIndex) break;
          else if(tmpMainIndex == mainIndex && tmpSubIndex >= subIndex) break;
          else if(hashtable[tmpMainIndex][tmpSubIndex] == 0){
            hashtable[tmpMainIndex][tmpSubIndex] = key;
            hashtable[mainIndex][subIndex] = 0;
          }
        }
      }
    }
    currSubIndex=0;
  }
}

int HierarchyHash::remove(const unsigned int key)
{
  // Write your code
  int timeCost = 0;
  int searchCost = search(key);
  unsigned int index; // real index
  unsigned int mainIndex; // maintable 내의 subtable 위치
  unsigned int subIndex ; // subtable 내의 index
  bool isLinear = flag == LINEAR_PROBING ? true : false;

  if(searchCost < 0) { // 존재하지 않음
    return searchCost;
  } else {
    for(unsigned int i=0;;i++){
      timeCost++;
      index = hashFunction(isLinear ? key+i : key+(i*i));
      mainIndex = index / sub_table_size;
      subIndex = index % sub_table_size;

      if(hashtable[mainIndex] == NULL){
        timeCost*=-1;
        break;
      } else if(hashtable[mainIndex][subIndex] == key){
        hashtable[mainIndex][subIndex] = flag == LINEAR_PROBING ? 0 : TOMBSTONE;
        num_of_keys--;
        num_of_subkeys[mainIndex]--;
        if(flag == LINEAR_PROBING) shifting(mainIndex, subIndex);
        deleteSub();
        break;
      } else if(hashtable[mainIndex][subIndex]==0 || hashtable[mainIndex][subIndex]==TOMBSTONE){ // 새로 바뀐 index에 대하여 subtable이 할당되어 있으면
        timeCost*=-1;   
        break;
      } 

      if(!isLinear && (unsigned int)timeCost >= table_size){ // linear로 변경 후 다시 시작
        i=-1; // reset
        isLinear = true;
        timeCost++;
      }
    }
  }
  return timeCost;
}

void HierarchyHash::deleteSub(){
  bool isEmpty = true;
  for(unsigned int i=0; i<main_table_size; i++){
    if(hashtable[i] != NULL){
      for(unsigned int j=0; j<sub_table_size; j++){
        if(hashtable[i][j]!=0){
          isEmpty=false;
          break;
        }
      }
      if(isEmpty){
        delete[] hashtable[i];
        hashtable[i]=NULL;
      }
      isEmpty=true;
    }
  }
}

int HierarchyHash::search(const unsigned int key)
{
  // Write your code
  int timeCost = 0;
  unsigned int index; // real index
  unsigned int mainIndex; // maintable 내의 subtable 위치
  unsigned int subIndex; // subtable 내의 index
  bool isLinear = flag == LINEAR_PROBING ? true : false;

  for(unsigned int i=0;;i++){
    timeCost++;
    index = hashFunction(isLinear ? key+i : key+(i*i));
    mainIndex = index / sub_table_size;
    subIndex = index % sub_table_size;
    if(!hashtable[mainIndex]){
      timeCost*=-1;
      break;
    } else if(hashtable[mainIndex][subIndex] == key){
      break;
    } else if(hashtable[mainIndex][subIndex]==0 || hashtable[mainIndex][subIndex]==TOMBSTONE){ // 새로 바뀐 index에 대하여 subtable이 할당되어 있으면
      timeCost*=-1;   
      break;
    } 

    if(!isLinear && (unsigned int)timeCost >= table_size){ // linear로 변경 후 다시 시작
      i=-1; // reset
      isLinear = true;
      timeCost++;
    }
  }
  
  return timeCost;
}

void HierarchyHash::clearTombstones()
{
  // Write your code
  unsigned int** origintable = hashtable;
  num_of_keys = 0;
	hashtable = new unsigned int *[main_table_size];
  num_of_subkeys = new unsigned int [main_table_size]();
  for(unsigned int i=0; i<main_table_size; i++){
    hashtable[i] = NULL;
	}

  for(unsigned int i=0; i<main_table_size; i++){
    if(origintable[i] == NULL) continue;
    for(unsigned int j=0; j<sub_table_size; j++){
      if(origintable[i][j]!=0 && origintable[i][j]!=TOMBSTONE) insert(origintable[i][j]);
    }
	}
  delete origintable;
}

void HierarchyHash::print()
{
  // Print valid key pair for each sub hash table - subtable_id:(index1:key1,index2:key2)
  // Seperate each sub table by endl
  // Give **NO** space between each character
  // e.g., 0:(1:3,3:7,5:1)
  //       1:(101:2,192:10)
  //       9:(902:90,938:82)

  // Exceptionally, keep this code only for the case there is no key in the table
  if( getNumofKeys() == 0){
    std::cout << "()" << std::endl;
    return;
  }

  // Write your code
  unsigned int count = 0;
  for(unsigned int i=0; i<main_table_size; i++){
    if(hashtable[i]){
      count = 0;
      std::cout << i << ":(";
      for(unsigned int j=0; j<sub_table_size; j++){
        if(hashtable[i][j]!=0 && hashtable[i][j]!=TOMBSTONE){
          count++;
          std::cout << (i*sub_table_size)+j << ":" << hashtable[i][j];
          if(count<num_of_subkeys[i]) std::cout<<",";
        }
      }
      std::cout << ")" << std::endl;
    }
  }
}

#endif
