#include "memory.h"
#include <cstddef>
#include "screenSource.h"
#include <ctime>

typedef unsigned short int instr;

class CPU : public screenSource {
 private:
  byte *V;
  address I;
  byte delay;
  byte sound;
  address PC;
  byte stackPointer;
  address *stack;
  memory *mem;
  void execute();
  void NOOP();
  void pushStack(address);
  address popStack();
  byte getV(int);
  void setV(int, byte);
  void setFlag();
  void clearFlag();
  byte **videoBuffer;
  bool *keyInput;
  void loadPreSprites();
  struct timespec *clockTimer;

 public:
  bool getFlag();
  void loadSprite(byte*, std::size_t, byte, byte);
  void loadSprite(byte*, std::size_t, byte, byte, bool);
  void setKeyInput(byte, bool);
  instr readInstruction();
  void run();
  void setPC(address);
  address getPC();
  void addPC(address);
  byte** getVideoBuffer();
  CPU(memory*);
  ~CPU();
};
