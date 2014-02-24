#include "CPU.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>

#define stdClock 2;
#define noClock 0;
#define skipInstr 4;

CPU::CPU(memory *mm){
  this->V = (byte*)calloc(16, sizeof(byte));
  this->stack = (address*)calloc(16, sizeof(address));
  this->stackPointer = 0;
  this->mem = mm;
  this->PC = 0x200;
  this->delay = 0;
  this->sound = 0;
  this->keyInput = (bool*)calloc(0x10, sizeof(bool));
  this->clockTimer = (struct timespec*)malloc(sizeof(struct timespec));
  this->clockTimer->tv_sec = 0;
  this->clockTimer->tv_nsec = 1190476; //approx 840Hz

  //create a 8x32 byte array with a total of 64*32 bits
  this->videoBuffer = (byte**)malloc(sizeof(byte*)*32);
  for(int i = 0; i < 32; i++){
    this->videoBuffer[i] = (byte*)calloc(sizeof(byte), 8);
  }
  this->loadPreSprites();
  this->stop = false;
  this->runThread = Glib::Thread::create(sigc::mem_fun(*this, &CPU::init));
}

CPU::~CPU(){
  this->stop = true;
  this->runThread->join();

  free(this->V);
  free(this->stack);
  for(int i = 0; i < 32; i++)
    free(this->videoBuffer[i]);
  free(this->videoBuffer);
  delete this->mem;
  free(this->keyInput);
  free(this->clockTimer);
}


void CPU::timer60Hz(){
  while(!this->stop){
    emiter60Hz();
    struct timespec t;
    t.tv_sec = 0;
    t.tv_nsec = 16666667;
    nanosleep(&t, NULL);
  }
}

void CPU::timerDecreementer(){
  if(this->delay > 0)
    this->delay--;
  if(this->sound > 0)
    this->sound--;
}

void CPU::init(){
  this->emiter60Hz.connect(sigc::mem_fun(*this, &CPU::timerDecreementer));
  this->TimerClockThread = Glib::Thread::create(sigc::mem_fun(*this, &CPU::timer60Hz));

  this->run();
}

void CPU::loadPreSprites(){
  //load default sprite digits to memory
  byte sprites[80] = {
    0xF0,0x90,0x90,0x90,0xF0, // --> 0x0
    0x20,0x60,0x20,0x20,0x70, // --> 0x1
    0xF0,0x10,0xF0,0x80,0xF0, // --> 0x2
    0xF0,0x10,0xF0,0x10,0xF0, // --> 0x3
    0x90,0x90,0xF0,0x10,0x10, // --> 0x4
    0xF0,0x80,0xF0,0x10,0xF0, // --> 0x5
    0xF0,0x80,0xF0,0x90,0xF0, // --> 0x6
    0xF0,0x10,0x20,0x40,0x40, // --> 0x7
    0xF0,0x90,0xF0,0x90,0xF0, // --> 0x8
    0xF0,0x90,0xF0,0x10,0xF0, // --> 0x9
    0xF0,0x90,0xF0,0x90,0x90, // --> 0xA
    0xE0,0x90,0xE0,0x90,0xE0, // --> 0xB
    0xF0,0x80,0x80,0x80,0xF0, // --> 0xC
    0xE0,0x90,0x90,0x90,0xE0, // --> 0xD
    0xF0,0x80,0xF0,0x80,0xF0, // --> 0xE
    0xF0,0x80,0xF0,0x80,0x80  // --> 0xF
  };
  this->mem->set(0, sprites, 80);
}

void CPU::setPC(address newPC){
  this->PC = newPC;
}

address CPU::getPC(){
  return this->PC;
}

void CPU::addPC(address addPC){
  this->PC += addPC;
}

byte** CPU::getVideoBuffer(){
  byte **cpBuffer = (byte**)malloc(32*sizeof(byte*));
  for(int i = 0; i < 32; i++){
    cpBuffer[i] = (byte*)malloc(sizeof(byte)*8);
    memcpy(cpBuffer[i], this->videoBuffer[i], sizeof(byte)*8);
  }
  return cpBuffer;
}

bool CPU::getFlag(){
  return (this->getV(0xF)) ? true : false;
}
void CPU::loadSprite(byte *sprite, std::size_t size, byte x, byte y){
  this->clearFlag();

  for(std::size_t spriteByte = 0; spriteByte < size; spriteByte++){
    byte row[8] = {0};

    for(byte bit = 0; bit < 8; bit++){
      byte bitPlace = (x+bit)%8;
      byte bytePlace = (((x+bit)/8)%8);
      byte bitOn = (sprite[spriteByte]<<bit) & 0x80;

      if(bitOn)
        row[bytePlace] |= (0x80>>bitPlace);
    }

    byte oldRow[8] = {0}; // (byte*)malloc(sizeof(byte)*8);
    for(int i = 0; i < 8; i++){
      memcpy(oldRow, this->videoBuffer[(spriteByte+y)%32], 8);
      this->videoBuffer[(spriteByte+y)%32][i] ^= row[i];

      for(byte bufferByte = 0; bufferByte < 8; bufferByte++){
        for(byte bit = 0; bit < 8; bit++){
          byte oldBit = ((oldRow[bufferByte]>>bit) & 0x1);
          byte newBit = ((this->videoBuffer[(spriteByte+y)%32][bufferByte]>>bit) & 0x1);
          if(oldBit && !newBit)
            this->setFlag();
        }
      }
    }
  }
}

void CPU::run(){
  while(!this->stop){
    this->execute();
    nanosleep(this->clockTimer, NULL);
    if(this->stop)
      return;
  }
}

void CPU::setKeyInput(byte key, bool pressed){
  this->keyInput[key] = pressed;
}

instr CPU::readInstruction(){
  byte first = this->mem->get(this->PC);
  byte second = this->mem->get(this->PC+1);
  return (instr)((first<<8)+second);
}

void CPU::pushStack(address addr){
  this->stack[this->stackPointer] = addr;
  if(this->stackPointer < 15)
    this->stackPointer++;
}

address CPU::popStack(){
  if(this->stackPointer > 0)
    this->stackPointer--;
  return this->stack[this->stackPointer];
}

byte CPU::getV(int x){
  if(x < 16 && -1 < x)
    return this->V[x];
  else
    return 0;
}

void CPU::setV(int x, byte kk){
  if(x < 16 && -1 < x)
    this->V[x] = kk;
}

void CPU::setFlag(){
  this->setV(0xF, 1);
}

void CPU::clearFlag(){
  this->setV(0xF, 0);
}

void CPU::execute(){
  int clock = stdClock;
  instr instruction = this->readInstruction();
  byte opCode1 = (byte)((instruction & 0xF000) >> 12);
  byte opCode2 = (byte)((instruction & 0x0F00) >> 8);
  byte opCode3 = (byte)((instruction & 0x00F0) >> 4);
  byte opCode4 = (byte)((instruction & 0x000F) >> 0);

  instr nnn = (instruction & 0x0FFF);
  byte n = (instruction & 0x000F) >> 0;
  byte x = (instruction & 0x0F00) >> 8;
  byte y = (instruction & 0x00F0) >> 4;
  byte kk = (instruction & 0x00FF) >> 0;

  switch(opCode1){
  case 0x0:
    //opCode 0???
    if(opCode2 == 0x0 && opCode3 == 0xE){
      //opCode 00E?
      if(opCode4 == 0x0){
        //opCode 00E0
        //clear screen
        for(int i = 0; i < 32; i++)
          memset(this->videoBuffer[i], 0, 8);
      }else if(opCode4 == 0xE){
        //opCode 00EE
        //return from subroutine
        this->setPC(this->popStack());
      }
    }
    break;
  case 0x1:
    //opCode 1nnn
    //jump to nnn
    this->setPC(nnn);
    clock = noClock;
    break;
  case 0x2:
    //opCode 2nnn
    //call subroutine at nnn
    this->pushStack(this->getPC());
    this->setPC(nnn);
    clock = noClock;
    break;
  case 0x3:
    //opCode 3xkk
    //skip next instruction if Vx = kk
    if(this->getV(x) == kk)
      clock = skipInstr;
    break;
  case 0x4:
    //opCode 4xkk
    //skip next instruction if Vx != kk
    if(this->getV(x) != kk)
      clock = skipInstr;
    break;
  case 0x5:
    //opCode 5xy0
    //skip next instruction if Vx = Vy
    if(this->getV(x) == this->getV(y))
      clock = skipInstr;
    break;
  case 0x6:
    //opCode 6xkk
    //set Vx = kk
    this->setV(x, kk);
    break;
  case 0x7:
    //opCode 7xkk
    //set Vx = Vx + kk
    this->setV(x, this->getV(x)+kk);
    break;
  case 0x8:
    //opCode 8xy?
    switch(opCode4){
    case 0x0:
      //opCode 8xy0
      //set Vx = Vy
      this->setV(x, this->getV(y));
      break;
    case 0x1:
      //opCode 8xy1
      //set Vx = Vx OR Vy
      this->setV(x, this->getV(x) | this->getV(y));
      break;
    case 0x2:
      //opCode 8xy2
      //set Vx = Vx AND Vy
      this->setV(x, this->getV(x) & this->getV(y));
      break;
    case 0x3:
      //opCode 8xy3
      //set Vx = Vx XOR Vy
      this->setV(x, this->getV(x) ^ this->getV(y));
      break;
    case 0x4: {
      //opCode 8xy4
      //Vx = Vx + Vy, set VF carry
      byte Vx = this->getV(x);
      byte Vy = this->getV(y);
      if(Vx+Vy > 256)
        this->setFlag();
      else
        this->clearFlag();
      this->setV(x, Vx+Vy);
    }
      break;
    case 0x5: {
      //opCode 8xy5
      //Vx = Vx - Vy, set VF NOT borrow
      byte Vx = this->getV(x);
      byte Vy = this->getV(y);
      if(Vx >= Vy)
        this->setFlag();
      else
        this->clearFlag();
      this->setV(x, Vx - Vy);
    }
      break;
    case 0x6: {
      //opCode 8x?6
      //set Vx = Vx shift right 1, set carry
      byte Vx = this->getV(x);
      if(Vx & 1)
        this->setFlag();
      else
        this->clearFlag();
      this->setV(x, (Vx >> 1));
    }
      break;
    case 0x7: {
      //opCode 8xy7
      //set Vx = Vy - Vx, VF NOT borrow
      byte Vx = this->getV(x);
      byte Vy = this->getV(y);
      if(Vy >= Vx)
        this->setFlag();
      else
        this->clearFlag();
      this->setV(x, Vy-Vx);
    }
      break;
    case 0xE: {
      //opCode 8x?E
      //set Vx = Vx Shift left 1, set carry
      byte Vx = this->getV(x);
      if(Vx & 8)
        this->setFlag();
      else
        this->clearFlag();
      this->setV(x, (Vx << 1));
    }
      break;
    }
    break;
  case 0x9:
    //opCode 9xy?
    //skip next instruction if Vx != Vy
    if(this->getV(x) != this->getV(y))
      clock = skipInstr;
    break;
  case 0xA:
    //opCode Annn
    //set I = nnn
    this->I = nnn;
    break;
  case 0xB:
    //opCode Bnnn
    //jump to nnn + V0
    this->setPC(nnn+this->getV(0));
    clock = noClock;
    break;
  case 0xC:
    //opCode Cxkk
    //set Vx = random byte AND kk
    srand(time(NULL));
    this->setV(x, ((rand()%256)&kk));
    break;
  case 0xD:
    //opCode Dxyn
    //display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision
    this->loadSprite(mem->get(this->I, this->I+n), n, this->getV(x), this->getV(y));
    break;
  case 0xE:
    //opCode Ex??
    if(opCode3 == 0x9 && opCode4 == 0xE){
      //opCode Dx9E
      //skip next instruction if key with the value of Vx is pressed
      if(this->keyInput[this->getV(x)])
        clock = skipInstr;
    }else if(opCode3 == 0xA && opCode4 == 1){
      //opCode DxA1
      //skip next instruction if key with the value of Vx is not pressed
      if(!this->keyInput[this->getV(x)])
        clock = skipInstr;
    }
    break;
  case 0xF:
    //opCode Fx??
    switch(opCode3){
    case 0x0:
      //opCode Fx0?
      if(opCode4 == 0x7){
        //opCode Fx07
        //set Vx = delay timer value
        this->setV(x, this->delay);
      }else if(opCode4 == 0xA){
        //opCode Fx0A
        //wait for a key press, store the value of the key in Vx
        struct timespec t;
        t.tv_sec = 0;
        t.tv_nsec = 1000;

        while(true){
          bool pressed;
          byte i;
          for(i = 0; i < 0x10; i++){
            if(pressed |= this->keyInput[i])
              break;
          }
          if(pressed){
            this->setV(x, i);
            break;
          }
          nanosleep(&t, NULL);
        }
      }
      break;
    case 0x1:
      //opCode Fx1?
      switch(opCode4){
      case 0x5:
        //opCode Fx15
        //set delay timer = Vx
        this->delay = this->getV(x);
        break;
      case 0x8:
        //opCode Fx18
        //set sound timer = Vx
        this->sound = this->getV(x);
        break;
      case 0xE:
        //opCode Fx1E
        //set I = I + Vx
        this->I += this->getV(x);
      }
      break;
    case 0x2:
      //opCode Fx29
      //set I = location of sprite for digit Vx
      this->I = this->getV(x)*5;
      break;
    case 0x3:
      //opCode Fx33
      //store BCD representation of Vx in memory locations I, I+1, and I+2
      byte bcd[3];
      bcd[0] = (this->getV(x))%10;
      bcd[1] = (this->getV(x)/10)%10;
      bcd[2] = (this->getV(x)/100)%10;
      this->mem->set(this->I, bcd, 3);
      break;
    case 0x5:
      //opCode Fx55
      //store registers V0 through Vx in memory starting at location I
      this->mem->set(this->I, this->V, x+1);
      break;
    case 0x6:
      //opCode Fx66
      //read registers V0 through Vx from memory starting at location I
      for(int i = 0; i < x+1; i++)
        this->setV(i, this->mem->get(this->I+i));
      break;
    }
  }
  this->addPC(clock);
}
