#include "mainWindow.h"
#include <iostream>
#include <fstream>

mainWindow::mainWindow(){
  this->set_title("nix8 The GTK chip interpreter");
  this->box = new Gtk::Box(Gtk::ORIENTATION_VERTICAL);
  this->add(*box);
  this->setup_menu();
  this->cpu = NULL;
  this->scrn = new screen();
  this->box->pack_start(*scrn, Gtk::PACK_EXPAND_WIDGET);
  this->box->show_all();
  this->add_events(Gdk::KEY_PRESS_MASK);
  this->show_all_children();
  this->about_dialog = new Gtk::AboutDialog();
  this->about_dialog->set_transient_for(*this);
  this->about_dialog->set_program_name("nix8");
  this->about_dialog->set_version("1.0");
  this->about_dialog->set_comments("nix8 the CHIP8 interpreter for linux");
  this->about_dialog->set_copyright("MIT License");
  this->about_dialog->set_website("https://github.com/nicklaswj/nix8");

  std::vector<Glib::ustring> authors;
  authors.push_back("Nicklas Warming Jacobsen");
  this->about_dialog->set_authors(authors);
  this->about_dialog->signal_response().connect(sigc::mem_fun(*this, &mainWindow::about_dialog_response));
}

void mainWindow::about_dialog_response(int response_id){
  this->about_dialog->hide();
}

void mainWindow::show_about(){
  this->about_dialog->show();
  this->about_dialog->present();
}

void mainWindow::setup_menu(){
  this->menu_ActionGroup = Gtk::ActionGroup::create();
  this->menu_ActionGroup->add(Gtk::Action::create("FileOpen",
                                                  Gtk::Stock::OPEN, "Open", "Open CHIP8 program"),
                              sigc::mem_fun(*this, &mainWindow::action_open_program));
  this->menu_ActionGroup->add(Gtk::Action::create("FileReset",
                                                  Gtk::Stock::REFRESH, "Reset", "Reset program"),
                              sigc::mem_fun(*this, &mainWindow::action_reset));
  this->menu_ActionGroup->add(Gtk::Action::create("FileQuit",
                                                  Gtk::Stock::QUIT, "Quit", "Quit"),
                              sigc::mem_fun(*this, &mainWindow::action_quit));
  this->menu_ActionGroup->add(Gtk::Action::create("HelpAbout",
                                                  Gtk::Stock::ABOUT, "About", "About this program"),
                              sigc::mem_fun(*this, &mainWindow::show_about));
  this->menu_UIManager = Gtk::UIManager::create();
  this->menu_UIManager->insert_action_group(this->menu_ActionGroup);
  this->add_accel_group(this->menu_UIManager->get_accel_group());

  this->menu_ActionGroup->add(Gtk::Action::create("FileMenu", "File"));
  this->menu_ActionGroup->add(Gtk::Action::create("HelpMenu", "Help"));


  Glib::ustring ui_info =
    "<ui>"
    "  <menubar name='MenuBar'>"
    "    <menu action='FileMenu'>"
    "      <menuitem action='FileOpen'/>"
    "      <menuitem action='FileReset'/>"
    "      <separator/>"
    "      <menuitem action='FileQuit'/>"
    "    </menu>"
    "    <menu action='HelpMenu'>"
    "      <menuitem action='HelpAbout'/>"
    "    </menu>"
    "  </menubar>"
    "</ui>";

  try
  {
    this->menu_UIManager->add_ui_from_string(ui_info);
  }
  catch(const Glib::Error& ex)
  {
    std::cerr << "building menus failed: " <<  ex.what();
  }
  Gtk::Widget *menubar = this->menu_UIManager->get_widget("/MenuBar");
  this->box->pack_start(*menubar, Gtk::PACK_SHRINK);
}

mainWindow::~mainWindow(){
  delete this->cpu;
}

void mainWindow::action_open_program(){
  Gtk::FileChooserDialog dialog("Please choose a CHIP8 program",
                                Gtk::FILE_CHOOSER_ACTION_OPEN);
  dialog.set_transient_for(*this);
  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

  int result = dialog.run();

  if(result == Gtk::RESPONSE_OK){
    this->program_path = dialog.get_filename();
    this->action_reset();
  }
}

void mainWindow::action_reset(){
  if(this->cpu)
    delete this->cpu;
  if(this->program_path.empty())
    return;

  this->scrn->stop();

  std::ifstream is(this->program_path.c_str(), std::ifstream::binary);
  is.seekg (0, is.end);
  int length = is.tellg();
  is.seekg (0, is.beg);
  if(length > 4095-0x200)
    return;

  char *buffer = (char*)malloc(sizeof(byte)*length);
  is.read(buffer, length);

  this->mem = new memory();
  this->mem->set(0x200, (byte*)buffer, length);
  free(buffer);
  this->cpu = new CPU(mem);
  this->scrn->setVideoSource(this->cpu);
  this->scrn->start();
}

void mainWindow::action_quit(){
  this->hide();
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
