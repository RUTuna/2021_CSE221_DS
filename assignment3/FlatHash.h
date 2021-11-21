#ifndef FLATHASH_H
#define FLATHASH_H

#include <iostream>

#define TOMBSTONE 9999999

// Flag(hint) for overflow handling
enum overflow_handle {
  LINEAR_PROBING = 0,
  QUADRATIC_PROBING
};

class FlatHash
{
private:
  unsigned int* hashtable;
  // Variable for overflow handling
  enum overflow_handle flag;
  // Loading factor
  float alpha;
  // Size of hash table
  unsigned int table_size;
  // Nums of keys
  unsigned int num_of_keys;

public:
  FlatHash(enum overflow_handle _flag, float _alpha);

  ~FlatHash();

  unsigned int hashFunction(const unsigned int key) { return key % table_size; }

  unsigned int getTableSize() { return table_size; }

  unsigned int getNumofKeys() { return num_of_keys; }

  unsigned int getValue(unsigned int index) { return hashtable[index]; }

  // Return time cost
  int insert(const unsigned int key);
  void resize();

  // Remove function in lecture. Return time cost
  void shifting(const unsigned int index);
  int remove(const unsigned int key);

  // Return time cost
  int search(const unsigned int key);

  // Delete tombstones
  void clearTombstones();

  void print();
};

FlatHash::FlatHash(enum overflow_handle _flag, float _alpha)
{
  // Initial table size is 1000 
  table_size = 1000;
  num_of_keys = 0;
  flag = _flag;
  alpha = _alpha;
  
  // Write your code
  hashtable = new unsigned int[table_size]();
  hashtable[0] = -1; // 0 index 사용 안함
}

FlatHash::~FlatHash()
{
  // Write your code
  delete[] hashtable;
}

int FlatHash::insert(const unsigned int key)
{
  // You have to implement two overflow handling by using flag
  // Write your code
  int timeCost = 1;
  int searchCost = search(key);
  unsigned int index = hashFunction(key);
  bool isLinear = flag == LINEAR_PROBING ? true : false;

  if(searchCost > 0) { // hashtable 내에 존재하면 (중복)
    return -1*searchCost;
  } else if(hashtable[index]==0 || hashtable[index]==TOMBSTONE){ // useable
    hashtable[index] = key;
    num_of_keys++;
  } else { // overflow
    for(unsigned int i=1;;i++){
      timeCost++;
      index = hashFunction(isLinear ? key+i : key+(i*i));
      if(hashtable[index]==0 || hashtable[index]==TOMBSTONE){
        hashtable[index] = key;
        num_of_keys++;
        break;
      }

      if(!isLinear && (unsigned int)timeCost >= table_size){ // QUADRATIC_PROBING의 경우 linear로 변경 후 다시 시작
        i=0; // reset (i가 1부터 시작하므로) 
        isLinear = true;
        timeCost++;
      }
    }
  }

  if((double)num_of_keys/table_size >= alpha) resize(); // resize 필요 시 수행
  return timeCost;
}

void FlatHash::resize(){
  unsigned int* origintable = hashtable; // 기존 hashtable
  table_size*=2;
  num_of_keys = 0;
  hashtable = new unsigned int[table_size](); // resize된 hashtable
  hashtable[0] = -1;

  for(unsigned int i=1; i<table_size/2; i++){
    if(origintable[i]!=0 && origintable[i]!=TOMBSTONE) insert(origintable[i]); // insert로 넣어주어야 함
    else if(flag == QUADRATIC_PROBING) hashtable[i] = TOMBSTONE;
  }
  delete[] origintable;
}

void FlatHash::shifting(const unsigned int start){
  unsigned int key; // shift할 key
  unsigned int index; // shift할 key의 현재 index
  unsigned int currIndex = start; // 탐색 index
  unsigned int tmpIndex; // 바꿀 위치 탐색 index

  for(; currIndex<table_size; currIndex++){
    if(hashtable[currIndex]!=0 && hashtable[currIndex]!=TOMBSTONE){ // 사용 중이면 shift 필요 여부 확인
      index = currIndex;
      key = hashtable[index];
      for(unsigned int i=0;;i++){
        tmpIndex = hashFunction(flag == LINEAR_PROBING ? key+i : key+(i*i));
        if(tmpIndex>=index) break; // shift 불필요
        else if(hashtable[tmpIndex]==0){
          hashtable[tmpIndex] = key;
          hashtable[index] = 0;
        }
      }
    }
  }
}

int FlatHash::remove(const unsigned int key)
{
  // Write your code
  int timeCost = 0;
  int searchCost = search(key);
  unsigned int index = hashFunction(key);
  bool isLinear = flag == LINEAR_PROBING ? true : false;

  if(searchCost < 0) { // 존재하지 않음
    return searchCost;
  } else {
    for(unsigned int i=0;;i++){
      timeCost++;
      index = hashFunction(isLinear ? key+i : key+(i*i));
      if(hashtable[index] == key){
        hashtable[index] = flag == LINEAR_PROBING ? 0 : TOMBSTONE;
        num_of_keys--;
        if(flag == LINEAR_PROBING) shifting(index);
        break;
      } else if(hashtable[index]==0 || hashtable[index]==TOMBSTONE){ // 범위에서 못 찾음
        timeCost*=-1;
        break;
      }

      if(!isLinear && (unsigned int)timeCost >= table_size){ // linear로 변경 후 다시 시작
        i=-1; // reset (i가 0부터 시작하므로)
        isLinear = true;
        timeCost++;
      }
    }
  }

  return timeCost;
}

int FlatHash::search(const unsigned int key)
{
  // Write your code
  int timeCost = 0;
  unsigned int index;
  bool isLinear = flag == LINEAR_PROBING ? true : false;

  for(unsigned int i=0;;i++){
    timeCost++;
    index = hashFunction(isLinear ? key+i : key+(i*i));
    if(hashtable[index] == key){ // found
      break;
    } else if(hashtable[index]==0 || hashtable[index]==TOMBSTONE){ // empty slot
      timeCost*=-1;
      break;
    }

    if(!isLinear && (unsigned int)timeCost >= table_size){ // linear로 변경 후 다시 시작
      i=-1; // reset (i가 0부터 시작하므로)
      isLinear = true;
      timeCost++;
    }
  }
  return timeCost;
}

void FlatHash::clearTombstones()
{
  // Write your code
  if(flag == QUADRATIC_PROBING){
    unsigned int* origintable = hashtable; // 기존 hashtable
    num_of_keys = 0;
    hashtable = new unsigned int[table_size](); // TOMBSTONE 제거된 hashtable
    hashtable[0] = -1;  
    for(unsigned int i=1; i<table_size; i++){
      if(origintable[i]!=0 && origintable[i]!=TOMBSTONE) insert(origintable[i]);
    }
    delete[] origintable;
  }
}

void FlatHash::print()
{
  // Print valid key pair - (index1:key1,index2:key2)
  // Give **NO** space between each character
  // e.g., (1:3,3:7,5:1)
  std::cout << "(";

  // Write your code
  unsigned int count = 0;
  for(unsigned int i=1; i<table_size; i++){
    if(hashtable[i]!=0 && hashtable[i]!=TOMBSTONE){
      count++;
      std::cout<<i<<":"<<hashtable[i];
      if(count<num_of_keys) std::cout<<",";
    }
  }

  std::cout << ")" << std::endl;
}

#endif
