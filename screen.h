#include <gtkmm/drawingarea.h>
#include "screenSource.h"

class screen : public Gtk::DrawingArea {
 public:
  screen(screenSource*);
  ~screen();
 protected:
  virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>&);
 private:
  screenSource *screenSrc;
};
