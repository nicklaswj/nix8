#include "CPUTest.h"
#include "CPU.h"
#include <cstdlib>
#include <iostream>
#include <cstdio>

bool loadSpriteTest(){
  std::cout << "Running loadSpriteTest" << std::endl;
  CPU *cpu = new CPU();

  byte *sprite = (byte*)malloc(sizeof(byte)*5);
  sprite[0] = 0xF0;
  sprite[1] = 0x90;
  sprite[2] = 0xF0;
  sprite[3] = 0x90;
  sprite[4] = 0x90;

  cpu->loadSprite(sprite, 5, 0, 0);
  byte **buffer = cpu->getVideoBuffer();

  if(buffer[0][0] != 0xF0)
    return false;
  if(buffer[1][0] != 0x90)
    return false;
  if(buffer[2][0] != 0xF0)
    return false;
  if(buffer[3][0] != 0x90)
    return false;
  if(buffer[4][0] != 0x90)
    return false;

  if(cpu->getFlag())
    return false;

  free(sprite);
  delete(cpu);
  return true;
}

bool spriteSetFlagTest(){
  std::cout << "Running spriteSetFlagTest" << std::endl;
  CPU *cpu = new CPU();

  byte *sprite = (byte*)malloc(sizeof(byte)*5);
  sprite[0] = 0xF0;
  sprite[1] = 0x90;
  sprite[2] = 0xF0;
  sprite[3] = 0x90;
  sprite[4] = 0x90;

  cpu->loadSprite(sprite, 5, 0, 0);
  cpu->loadSprite(sprite, 5, 0, 0);

  if(!cpu->getFlag())
    return false;

  free(sprite);
  delete(cpu);
  return true;
}

bool spriteDontSetFlagTest(){
  std::cout << "Running spriteDontSetFlagTest" << std::endl;
  CPU *cpu = new CPU();

  byte *sprite = (byte*)malloc(sizeof(byte)*5);
  sprite[0] = 0xF0;
  sprite[1] = 0x90;
  sprite[2] = 0xF0;
  sprite[3] = 0x90;
  sprite[4] = 0x90;

  cpu->loadSprite(sprite, 5, 0, 0);
  cpu->loadSprite(sprite, 5, 4, 0);

  if(cpu->getFlag())
    return false;

  free(sprite);
  delete(cpu);
  return true;
}

void printBuffer(byte **buffer){
  for(int i = 0; i < 32; i++){
    for(int a = 0; a < 8; a++){
      for(int bit = 0; bit < 8; bit++){
        if((buffer[i][a] << bit) & 0x80)
          printf("1");
        else
          printf("0");
      }
    }
    printf("\n");
  }
}

bool spriteWrapTest(){
  std::cout << "Running spriteWrapTest" << std::endl;
  CPU *cpu = new CPU();

  byte sprite[5] = {0xF0,0x90,0xF0,0x90,0x90};

  cpu->loadSprite(sprite, 5,62,30);
  byte **buffer = cpu->getVideoBuffer();

  //  printBuffer(buffer);

  //left upper corner
  if(buffer[0][0] != 0b11000000) return false;
  if(buffer[1][0] != 0b01000000) return false;
  if(buffer[2][0] != 0b01000000) return false;

  //right upper corner
  if(buffer[0][7] != 0b00000011) return false;
  if(buffer[1][7] != 0b00000010) return false;
  if(buffer[2][7] != 0b00000010) return false;

  //left lower corner
  if(buffer[30][0] != 0b11000000) return false;
  if(buffer[31][0] != 0b01000000) return false;

  //right lower corner
  if(buffer[30][7] != 0b0000011) return false;
  if(buffer[31][7] != 0b0000010) return false;

  delete(cpu);
  free (buffer);
  return true;
}
