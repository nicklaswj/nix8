#include <gtkmm.h>
#include "CPU.h"
#include "screen.h"
#include <cstring>

class mainWindow : public Gtk::Window {
 public:
  mainWindow();
  ~mainWindow();

 protected:
  void action_reset();
  void action_quit();
  void action_open_program();
 private:
  Gtk::Box *box;
  CPU *cpu;
  memory *mem;
  screen *scrn;
  bool on_key_press_event(GdkEventKey *event);
  bool on_key_release_event(GdkEventKey *event);
  Glib::RefPtr<Gtk::UIManager> menu_UIManager;
  Glib::RefPtr<Gtk::ActionGroup> menu_ActionGroup;
  void setup_menu();
  std::string program_path;
};
