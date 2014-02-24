#include "screen.h"
#include <cairomm/context.h>

screen::screen(screenSource *sSrc){
  this->screenSrc = sSrc;
  this->t = (struct timespec*)malloc(sizeof(struct timespec));
  this->t->tv_sec = 0;
  //about 35 Hz
  this->t->tv_nsec = 28571428;

  this->stop = false;
  this->refreshThread = Glib::Thread::create(sigc::mem_fun(*this, &screen::run));
}

screen::~screen(){
  this->stop = true;
  this->refreshThread->join();
}

void screen::run(){
  while(!this->stop){
    this->force_redraw();

    nanosleep(this->t, NULL);
  }
}

void screen::force_redraw(){
  //force our program to redraw the entire clock.
  Glib::RefPtr<Gdk::Window> win = this->get_window();
  Gtk::Allocation all = get_allocation();
  if(win)
    {
      Gdk::Rectangle r(0, 0, all.get_width(), all.get_height());
      win->invalidate_rect(r, false);
    }
}

bool screen::on_draw(const Cairo::RefPtr<Cairo::Context>& cr){
  unsigned char **scrnBuffer = this->screenSrc->getVideoBuffer();
  Gtk::Allocation allocation = this->get_allocation();
  const double height = allocation.get_height();
  const double width = allocation.get_width();
  double scale;

  if(width < height){
    scale = width/64;
  }else{
    scale = height/64;
  }

  const double margin = 0.1;
  const double xCenter = ((width/scale)/2)-32;
  const double yCenter = ((height/scale)/2)-16;
  const double xMargin = xCenter+margin;
  const double yMargin = yCenter+margin;
  cr->scale(scale, scale);

  cr->set_line_width(margin);
  cr->rectangle(xCenter,yCenter,64+margin*2,32+margin*2);
  cr->stroke();

  cr->set_source_rgb(0,0,0);
  cr->move_to(0,0);

  for(int y = 0; y < 32; y++){
    for(int x = 0; x < 64; x++){
      if(scrnBuffer[y][x/8] & (0x80 >> (x%8))){
        cr->rectangle(x+xMargin, y+yMargin, 1, 1);
      }
    }
  }
  cr->fill();
  for(int i = 0; i < 32; i++){
    free(scrnBuffer[i]);
  }
  free(scrnBuffer);
  return true;
}
