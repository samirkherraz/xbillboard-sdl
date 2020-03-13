#ifndef Pdf_H
#define Pdf_H

#include "Media.h"
#include <poppler/cpp/poppler-document.h>
#include <poppler/cpp/poppler-page.h>
#include <poppler/cpp/poppler-page-renderer.h>
#include <poppler/cpp/poppler-image.h>

class Pdf : protected Media
{
public:
  Pdf(std::string _path, SDL_Rect _rect, SDL_Renderer *_renderer, Drawable *_queue) : Media(_path, _rect, _renderer, _queue){};
  ~Pdf();
  bool init();
  void start(int delay);

private:
  static void do_nothing(const std::string &, void *);
  poppler::document *document;
  int nbPages = 0;
  poppler::page_renderer page_renderer;
};

#endif
