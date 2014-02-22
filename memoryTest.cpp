#include "memory.h"
#include <cstdlib>
#include <iostream>

bool memorySetBlockTest(){
  std::cout << "Running memorySetBlockTest" << std::endl;
  memory *mem = new memory();

  //make test block
  byte *block = (byte*)malloc(256*sizeof(byte));
  for(int i = 0; i < 256; i++){
    block[i] = i;
  }

  mem->set(200, block, 256);

  for(int i = 0; i < 256; i++){
    if(mem->get(i+200) != i)
      return false;
  }

  free(block);
  delete(mem);
  return true;
}

bool memorySetTest(){
  std::cout << "Running memorySetTest" << std::endl;
  memory *mem = new memory();

  for(int i = 0; i < 256; i++){
    mem->set(i+200, (byte)i);
  }

  for(int i = 0; i < 256; i++){
    if(mem->get(i+200) != i)
      return false;
  }

  delete(mem);
  return true;
}

bool memoryGetBlockTest(){
  std::cout << "Running memoryGetBlockTest" << std::endl;
  memory *mem = new memory();

  //make test block
  byte *block = (byte*)malloc(256*sizeof(byte));
  for(int i = 0; i < 256; i++){
    block[i] = i;
  }
  //set block
  mem->set(200, block, 256);
  //get block
  byte *returnBlock = mem->get(200, 256);

  for(int i = 0; i < 256; i++){
    if(block[i] != returnBlock[i])
      return false;
  }
  free(block);
  free(returnBlock);
  delete(mem);
  return true;
}

bool memoryClearTest(){
  std::cout << "Running memoryClearTest" << std::endl;
  memory *mem = new memory();

  //make test block
  byte *block = (byte*)malloc(256*sizeof(byte));
  for(int i = 0; i < 256; i++){
    block[i] = i;
  }

  mem->set(200, block, 256);
  mem->clear();
  for(int i = 0; i < 256; i++){
    if(mem->get(i+200) != 0)
      return false;
  }

  free(block);
  delete(mem);
  return true;
}
