#include <gtkmm/drawingarea.h>
#include "screenSource.h"
#include <gtkmm.h>

class screen : public Gtk::DrawingArea {
 public:
  screen(screenSource*);
  screen();
  ~screen();
  void force_redraw();
  void run();
  void setVideoSource(screenSource*);
  void stop();
  void start();
 protected:
  virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>&);
 private:
  screenSource *screenSrc;
  Glib::Thread *refreshThread;
  bool stoped;
  struct timespec *t;
  void init();
};
