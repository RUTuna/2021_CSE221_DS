#ifndef FLATHASH_H
#define FLATHASH_H

#include <iostream>

#define TOMBSTONE 9999999
using namespace std;
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
  hashtable = new unsigned int[table_size+1]();
  hashtable[0] = -1; // 0 index 사용 안함
}

FlatHash::~FlatHash()
{
  // Write your code
  delete hashtable;
}

int FlatHash::insert(const unsigned int key)
{
  // You have to implement two overflow handling by using flag
  // Write your code
  int timeCost = 1;
  int searchCost = search(key);
  unsigned int index = hashFunction(key);

  if(searchCost > 0) { // 중복
    return -1*searchCost;
  } else if(hashtable[index]==0 || hashtable[index]==TOMBSTONE){ // useable
    hashtable[index] = key;
    num_of_keys++;
  } else { // overflow
    for(int i=1;;i++){
      timeCost++;
      index = hashFunction(flag == LINEAR_PROBING ? key+i : key+(i*i));
      if(hashtable[index]==0 || hashtable[index]==TOMBSTONE){
        hashtable[index] = key;
        num_of_keys++;
        break;
      }
      
      if((unsigned int)timeCost >= table_size){
        index=hashFunction(key);
        timeCost++;

        while(true){
          index++;
          timeCost++;
          if(index==table_size) index=0;
          if(hashtable[index]==0 || hashtable[index]==TOMBSTONE){
            hashtable[index] = key;
            num_of_keys++;
            break;
          }
        }
        break;
      }
    }
  }

  if((double)num_of_keys/table_size >= alpha) resize(); // resize 필요 여부 확인
  return timeCost;
}

void FlatHash::resize(){
  unsigned int* origintable = hashtable;
  hashtable = new unsigned int[table_size*2 +1]();
  hashtable[0] = -1;
  for(unsigned int i=1;i<=table_size/2;i++){
      if(origintable[i]!=0 && origintable[i]!=TOMBSTONE) insert(origintable[i]);
      else if(origintable[i]!=TOMBSTONE) hashtable[i] = TOMBSTONE;

  }
}

void FlatHash::shifting(const unsigned int start){
  unsigned int key; // shift할 key
  unsigned int index; // shift할 key의 현재 index
  unsigned int currIndex = start; // 탐색 index
  unsigned int tmpIndex; // 바꿀 위치 탐색 index

  for(; currIndex<table_size; currIndex++){
    if(hashtable[currIndex]!=0){ // 사용 중이면 shift 유무 확인
      index = currIndex;
      key = hashtable[index];
      for(int i=0;;i++){
        tmpIndex = hashFunction(key+i);
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

  if(searchCost < 0) { // 존재하지 않음
    return searchCost;
  } else{
    for(int i=0;;i++){
      timeCost++;
      index = hashFunction(flag == LINEAR_PROBING ? key+i : key+(i*i));
      if(hashtable[index]==key){
        hashtable[index] = flag == LINEAR_PROBING ? 0 : TOMBSTONE;
        num_of_keys--;
        break;
      } else if(hashtable[index]==0 || hashtable[index]==TOMBSTONE || index > table_size){ // 못 찾음
        timeCost*=-1;
        break;
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

  for(int i=0;;i++){
    timeCost++;
    index = hashFunction(flag == LINEAR_PROBING ? key+i : key+(i*i));
    if(hashtable[index]==key){
      break;
    } else if(hashtable[index]==0 || index > table_size || hashtable[index]==TOMBSTONE){ // 못 찾음
      timeCost*=-1;
      break;
    }
  }
  return timeCost;
}

void FlatHash::clearTombstones()
{
  // Write your code
  if(flag == QUADRATIC_PROBING){
    for(unsigned int i=1; i<=table_size; i++){
      if(hashtable[i] == TOMBSTONE) hashtable[i] = 0;
    }
    shifting(0);
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
  for(unsigned int i=1; i<=table_size; i++){
    if(hashtable[i]!=0 && hashtable[i]!=TOMBSTONE){
      count++;
      cout<<i<<":"<<hashtable[i];
      if(count<num_of_keys) cout<<",";
    }
  }

  std::cout << ")" << std::endl;
}

#endif
