#include <cstddef>
typedef unsigned char byte;
typedef unsigned short address;

class memory {
 public:
  memory();
  memory(std::size_t);
  ~memory();
  void set(address, byte);
  void set(address, byte*, std::size_t);
  byte get(address);
  byte* get(address, std::size_t);
  void clear();
  void load(byte[], int);
 private:
  byte *mem;
};
