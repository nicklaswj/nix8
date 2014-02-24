#include <gtkmm/drawingarea.h>
#include "screenSource.h"
#include <gtkmm.h>

class screen : public Gtk::DrawingArea {
 public:
  screen(screenSource*);
  ~screen();
  void force_redraw();
  void run();
 protected:
  virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>&);
 private:
  screenSource *screenSrc;
  Glib::Thread *refreshThread;
  bool stop;
  struct timespec *t;
};
