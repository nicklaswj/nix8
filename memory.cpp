#include "memory.h"
#include <cstdlib>
#include <cstring>

memory::memory(){
  this->mem = (byte*)calloc(4095, sizeof(byte));
}

memory::memory(std::size_t memSize){
  this->mem = (byte*)calloc(memSize, sizeof(byte));
}

memory::~memory(){
  free(this->mem);
}

void memory::set(address add, byte data){
  this->mem[add] = data;
}

void memory::set(address add, byte *data, std::size_t size){
  //brug memcpy i stedet
  memcpy(&this->mem[add], data, size);
}

byte memory::get(address add){
  return this->mem[add];
}

byte* memory::get(address add, std::size_t size){
  byte *chunk = (byte*)malloc(sizeof(byte)*size);
  memcpy(chunk, &this->mem[add], sizeof(byte)*size);
  return chunk;
}

void memory::clear(){
  for(int i = 0; i < 4095; i++)
    this->mem[i] = 0;
}

void memory::load(byte data[], int size){
  for(int i = 0; i < size; i++)
    this->mem[i] = data[i];
}
