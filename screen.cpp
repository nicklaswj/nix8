#include "screen.h"
#include <cairomm/context.h>

screen::screen(screenSource *sSrc){
  this->screenSrc = sSrc;
}

screen::~screen(){

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

  return true;
}
