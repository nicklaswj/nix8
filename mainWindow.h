#include <gtkmm.h>
#include "CPU.h"
#include "screen.h"

class mainWindow : public Gtk::Window {
 public:
  mainWindow();
  ~mainWindow();

 protected:

 private:
  CPU *cpu;
  memory mem;
  screen scrn;
  bool on_key_press_event(GdkEventKey *event)
};
