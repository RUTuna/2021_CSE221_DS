#ifndef NRKFLAT_H
#define NRKFLAT_H

#include "FlatHash.h"
#include <iostream>
#include <stdint.h>

#define INT2VOIDP(i) (void*)(uintptr_t)(i)

class NRKFlat : public FlatHash
{
private:
  // Counter array
  unsigned int* counters;
  // Size of NRK filter (the number of counters)
  unsigned int filter_size;

public:
  NRKFlat(enum overflow_handle _flag, float _alpha, unsigned int _filter_size);

  ~NRKFlat();
  
  unsigned int hashFunction(const unsigned int key) { return key % filter_size; }
  
  // Hash function
  unsigned int murmurHash2(const void* key);

  void getMMHashValue(const unsigned int key, unsigned int& h1, unsigned int& h2, unsigned int& h3);

  bool filter(const unsigned int key);

  // Overwriting
  int insert(const unsigned int key);
  
  // Overwriting
  int remove(const unsigned int key);

  // Overwriting
  int search(const unsigned int key);
};

NRKFlat::NRKFlat(enum overflow_handle _flag, float _alpha, unsigned int _filter_size) : FlatHash(_flag, _alpha)
{
  filter_size = _filter_size;
  // Write your code
  counters = new unsigned int [filter_size]();
}

NRKFlat::~NRKFlat()
{
  // Write your code
  filter_size = 0;
  delete counters;
}

unsigned int NRKFlat::murmurHash2(const void* key){
  int len = 4;
  unsigned int seed = 2019;

  const unsigned int m = 0x5bd1e995;
  const int r = 24;

  unsigned int h = seed ^ len;

  const unsigned char * data = (const unsigned char *)key;

  while(len >= 4)
  {
    unsigned int k = *(unsigned int *)data;

    k *= m; 
    k ^= k >> r; 
    k *= m; 
    
    h *= m; 
    h ^= k;

    data += 4;
    len -= 4;
  }

  switch(len)
  {
  case 3: h ^= data[2] << 16;
  case 2: h ^= data[1] << 8;
  case 1: h ^= data[0];
          h *= m;
  };

  h ^= h >> 13;
  h *= m;
  h ^= h >> 15;

  return h;
}

void NRKFlat::getMMHashValue(const unsigned int key, unsigned int& h1, unsigned int& h2, unsigned int& h3)
{ 
  // You can use h1, h2 and h3 as hashing results which you have to use to decide counter locations
  h1 = murmurHash2(INT2VOIDP(&key));
  h2 = murmurHash2(INT2VOIDP(&h1));
  h3 = murmurHash2(INT2VOIDP(&h2));
}

bool NRKFlat::filter(const unsigned int key)
{
  // You can use h1, h2 and h3 as hashing results which you have to use to decide counter locations
  unsigned int h1, h2, h3;
  getMMHashValue(key, h1, h2, h3);

  // Write your code
  h1=hashFunction(h1);
  h2=hashFunction(h2);
  h3=hashFunction(h3);

  if(counters[h1]&&counters[h2]&&counters[h3]) return true; // 3개 모두 0이 아닐 경우
  else return false; // 하나라도 0일 경우
}

int NRKFlat::insert(const unsigned int key)
{
  // Write your code
  unsigned int h1, h2, h3;
  getMMHashValue(key, h1, h2, h3);
  h1=hashFunction(h1);
  h2=hashFunction(h2);
  h3=hashFunction(h3);

  counters[h1]++;
  counters[h2]++;
  counters[h3]++;

  if(filter_size != getTableSize()){ // table이 resize 되었으면 counters도 resize
    unsigned int value;
    unsigned int* originCounter = counters; // 기존 counters
    filter_size = getTableSize();
    counters = new unsigned int [filter_size](); // resize된 counters

    for(unsigned int i=0; i<filter_size/2; i++){
      value = getValue(i);
      if(value){
        getMMHashValue(value, h1, h2, h3);
        h1=hashFunction(h1);
        h2=hashFunction(h2);
        h3=hashFunction(h3);

        counters[h1]++;
        counters[h2]++;
        counters[h3]++;
      }
    }

    delete[] originCounter;
  }
  return FlatHash::insert(key);
}

int NRKFlat::remove(const unsigned int key)
{
  // Write your code
  if(!filter(key)) return 0;

  unsigned int h1, h2, h3;
  getMMHashValue(key, h1, h2, h3);
  h1=hashFunction(h1);
  h2=hashFunction(h2);
  h3=hashFunction(h3);

  counters[h1]--;
  counters[h2]--;
  counters[h3]--;
  return FlatHash::remove(key);
}

int NRKFlat::search(const unsigned int key)
{
  // Write your code
  if(!filter(key)) return 0;
  return FlatHash::search(key);
}

#endif
