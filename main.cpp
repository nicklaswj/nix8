#include <gtkmm.h>
#include "mainWindow.h"

using namespace std;

int main(int argc, char **argv){
  Glib::RefPtr<Gtk::Application> app =
    Gtk::Application::create(argc, argv, "com.nicklas.chip8");

  mainWindow w;

  return app->run(w);
}
