#ifndef SCREENSOURCE_H
#define SCREENSOURCE_H

class screenSource {
 public:
  screenSource(){};
  virtual ~screenSource(){};
  virtual unsigned char** getVideoBuffer() = 0;
};

#endif
