#include "mainWindow.h"
#include <fstream>

mainWindow::mainWindow(){
  std::ifstream is ("game", std::ifstream::binary);
  is.seekg (0, is.end);
  int length = is.tellg();
  is.seekg (0, is.beg);
  char *buffer = (char*)malloc(sizeof(byte)*length);
  is.read(buffer, length);

  this->mem = new memory();
  this->mem->set(0x200, (byte*)buffer, length);
  this->cpu = new CPU(mem);
  this->scrn = new screen(cpu);

  this->add(*scrn);

  this->add_events(Gdk::KEY_PRESS_MASK);
  this->show_all_children();
  free(buffer);
  is.close();
}

mainWindow::~mainWindow(){
  delete this->cpu;
}

bool mainWindow::on_key_press_event(GdkEventKey *event){
  switch(event->keyval){
  case GDK_KEY_1:
    this->cpu->setKeyInput(0x1, true);
    break;
  case GDK_KEY_2:
    this->cpu->setKeyInput(0x2, true);
    break;
  case GDK_KEY_3:
    this->cpu->setKeyInput(0x3, true);
    break;
  case GDK_KEY_4:
    this->cpu->setKeyInput(0xC, true);
    break;

  case GDK_KEY_q:
    this->cpu->setKeyInput(0x4, true);
    break;
  case GDK_KEY_w:
    this->cpu->setKeyInput(0x5, true);
    break;
  case GDK_KEY_e:
    this->cpu->setKeyInput(0x6, true);
    break;
  case GDK_KEY_r:
    this->cpu->setKeyInput(0xD, true);
    break;

  case GDK_KEY_a:
    this->cpu->setKeyInput(0x7, true);
    break;
  case GDK_KEY_s:
    this->cpu->setKeyInput(0x8, true);
    break;
  case GDK_KEY_d:
    this->cpu->setKeyInput(0x9, true);
    break;
  case GDK_KEY_f:
    this->cpu->setKeyInput(0xE, true);
    break;

  case GDK_KEY_z:
    this->cpu->setKeyInput(0xA, true);
    break;
  case GDK_KEY_x:
    this->cpu->setKeyInput(0x0, true);
    break;
  case GDK_KEY_c:
    this->cpu->setKeyInput(0xB, true);
    break;
  case GDK_KEY_v:
    this->cpu->setKeyInput(0xF, true);
    break;
  }
  return false;
}

bool mainWindow::on_key_release_event(GdkEventKey *event){
  switch(event->keyval){
  case GDK_KEY_1:
    this->cpu->setKeyInput(0x1, false);
    break;
  case GDK_KEY_2:
    this->cpu->setKeyInput(0x2, false);
    break;
  case GDK_KEY_3:
    this->cpu->setKeyInput(0x3, false);
    break;
  case GDK_KEY_4:
    this->cpu->setKeyInput(0xC, false);
    break;

  case GDK_KEY_q:
    this->cpu->setKeyInput(0x4, false);
    break;
  case GDK_KEY_w:
    this->cpu->setKeyInput(0x5, false);
    break;
  case GDK_KEY_e:
    this->cpu->setKeyInput(0x6, false);
    break;
  case GDK_KEY_r:
    this->cpu->setKeyInput(0xD, false);
    break;

  case GDK_KEY_a:
    this->cpu->setKeyInput(0x7, false);
    break;
  case GDK_KEY_s:
    this->cpu->setKeyInput(0x8, false);
    break;
  case GDK_KEY_d:
    this->cpu->setKeyInput(0x9, false);
    break;
  case GDK_KEY_f:
    this->cpu->setKeyInput(0xE, false);
    break;

  case GDK_KEY_z:
    this->cpu->setKeyInput(0xA, false);
    break;
  case GDK_KEY_x:
    this->cpu->setKeyInput(0x0, false);
    break;
  case GDK_KEY_c:
    this->cpu->setKeyInput(0xB, false);
    break;
  case GDK_KEY_v:
    this->cpu->setKeyInput(0xF, false);
    break;
  }
  return false;
}
