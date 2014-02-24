#include "memory.h"
#include <cstddef>
#include "screenSource.h"
#include <ctime>
#include <gtkmm.h>

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
  void init();
  struct timespec *clockTimer;
  Glib::Thread *runThread;
  Glib::Thread *TimerClockThread;
  bool stop;
  void timerDecreementer();
  void timer60Hz();

 protected:
  Glib::Dispatcher emiter60Hz;

 public:
  bool getFlag();
  void loadSprite(byte*, std::size_t, byte, byte);
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
