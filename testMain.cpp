#include "memoryTest.h"
#include "CPUTest.h"
#include <iostream>

int main (int argc, char **argv){
  char failed[] = "!!!!!!!!!-------Failed--------!!!!!!!!!\n";

  if(!memorySetBlockTest()){
    std::cout << failed;
    return 1;
  }

  if(!memorySetTest()){
    std::cout << failed;
    return 1;
  }

  if(!memoryGetBlockTest()){
    std::cout << failed;
    return 1;
  }

  if(!memoryClearTest()){
    std::cout << failed;
    return 1;
  }

  if(!loadSpriteTest()){
    std::cout << failed;
    return 1;
  }

  if(!spriteSetFlagTest()){
    std::cout << failed;
    return 1;
  }

  if(!spriteDontSetFlagTest()){
    std::cout << failed;
    return 1;
  }

  if(!spriteWrapTest()){
    std::cout << failed;
    return 1;
  }

  std::cout << "Success yeaa\n";
  return 0;
}
