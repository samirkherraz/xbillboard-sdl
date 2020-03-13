#ifndef Video_H
#define Video_H

#include "Media.h"
#include <queue>
#include <mutex>
#include <condition_variable>
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavformat/avio.h>
#include <libavutil/avutil.h>
#include <libswresample/swresample.h>
#include <libavutil/frame.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
};

class Video : protected Media
{
public:
    Video(std::string _path, SDL_Rect _rect, SDL_Renderer *_renderer, Drawable *_queue) : Media(_path, _rect, _renderer, _queue){};

    ~Video();

    bool init();
    void start();

private:
    friend int decoder(void *data);
    void startDecoding();
    void endDecoding();
    bool end();
    void push(SDL_Texture *);
    SDL_Texture *pop();
    AVFormatContext *pFormatContext;
    AVCodecParameters *pCodecParameters;
    AVCodec *pCodec;
    AVPacket *pPacket;
    AVFrame *pFrame;
    AVCodecContext *pCodecContext;
    int video_stream_index;
    struct timeval tp;
    bool is_decoding = false;
    std::mutex mtx;
    std::mutex cmtx;
    std::condition_variable cond;
    std::queue<SDL_Texture *> mqueue;
};

#endif
