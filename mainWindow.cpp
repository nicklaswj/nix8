#include "mainWindow.h"

mainWindow::mainWindow(){
  this->mem = new memory();
  this->cpu = new CPU(mem);
  this->scrn = new screen(cpu);

  this->add(*scrn);

  this->add_events(Gdk::KEY_PRESS_MASK);
  this->show_all_children();
}

mainWindow::~mainWindow(){
  delete this->cpu;
}

mainWindow::on_key_press_event(gdkeventkey *event){
 switch(event->keyval){
 case GDK_KEY_1:
   break;
 case GDK_KEY_2:
   break;
 case GDK_KEY_3:
   break;
 case GDK_KEY_q:
   break;
 case GDK_KEY_w:
   break;
 case GDK_KEY_e:
   break;
 case GDK_KEY_a:
   break;
 case GDK_KEY_s:
   break;
 case GDK_KEY_d:
   break;
 case GDK_KEY_z:
   break;
 case GDK_KEY_x:
   break;
 case GDK_KEY_c:
   break;
 }
}
