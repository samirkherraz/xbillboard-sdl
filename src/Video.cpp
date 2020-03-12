#include "Video.h"
#include <sys/time.h>
#include "Window.h"
Video::~Video()
{
    if (!pFormatContext)
    {
        avformat_close_input(&pFormatContext);
    }
    if (!pFormatContext)
    {
        avformat_free_context(pFormatContext);
    }
    if (!pPacket)
    {
        av_packet_free(&pPacket);
    }
    if (!pFrame)
    {
        av_frame_free(&pFrame);
    }
    if (!pCodecContext)
    {
        avcodec_free_context(&pCodecContext);
    }
}

bool Video::init()
{
    pFormatContext = avformat_alloc_context();
    if (pFormatContext == 0x0)
    {
        return false;
    }
    if (avformat_open_input(&pFormatContext, path.c_str(), NULL, NULL) != 0)
    {
        return false;
    }
    if (avformat_find_stream_info(pFormatContext, NULL) < 0)
    {
        return false;
    }

    pCodec = NULL;
    pCodecParameters = NULL;
    video_stream_index = -1;

    for (unsigned int i = 0; i < pFormatContext->nb_streams; i++)
    {
        AVCodecParameters *pLocalCodecParameters = NULL;
        pLocalCodecParameters = pFormatContext->streams[i]->codecpar;
        AVCodec *pLocalCodec = NULL;
        pLocalCodec = avcodec_find_decoder(pLocalCodecParameters->codec_id);

        if (pLocalCodec == NULL)
        {
            return false;
        }

        if (pLocalCodecParameters->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            if (video_stream_index == -1)
            {
                video_stream_index = i;
                pCodec = pLocalCodec;
                pCodecParameters = pLocalCodecParameters;
            }
        }
    }

    pCodecContext = avcodec_alloc_context3(pCodec);
    if (pCodecContext == 0x0)
    {
        return false;
    }

    pCodecContext->thread_count = 4;
    pCodecContext->thread_type = FF_THREAD_FRAME;

    if (avcodec_parameters_to_context(pCodecContext, pCodecParameters) < 0)
    {
        return false;
    }

    if (avcodec_open2(pCodecContext, pCodec, NULL) < 0)
    {
        return false;
    }
    pFrame = av_frame_alloc();
    if (pFrame == 0x0)
    {
        return false;
    }
    pPacket = av_packet_alloc();
    if (pPacket == 0x0)
    {
        return false;
    }
    startDecoding();
    return true;
}

int decoder(void *dd)
{

    Video *c = (Video *)dd;

    while (true)
    {
        int rest = av_read_frame(c->pFormatContext, c->pPacket);
        if (rest < 0)
        {
            c->endDecoding();
            return 0;
        }
        if (c->pPacket->stream_index == c->video_stream_index)
        {
            int response = avcodec_send_packet(c->pCodecContext, c->pPacket);

            if (response < 0)
            {
                c->endDecoding();
                return 0;
            }

            while (response >= 0)
            {
                response = avcodec_receive_frame(c->pCodecContext, c->pFrame);

                if (response == AVERROR(EAGAIN) || response == AVERROR_EOF)
                {
                    break;
                }
                else if (response >= 0)
                {
                    
                    Window::lock();
                    SDL_Texture *t = SDL_CreateTexture(c->renderer, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING, c->pCodecParameters->width, c->pCodecParameters->height);
                    SDL_UpdateYUVTexture(t, NULL, c->pFrame->data[0], c->pFrame->linesize[0],
                                         c->pFrame->data[1], c->pFrame->linesize[1],
                                         c->pFrame->data[2], c->pFrame->linesize[2]);
                    Window::unlock();
                    c->push(t);
                    
                }
                else
                {
                    c->endDecoding();
                    return 0;
                }
            }
            av_packet_unref(c->pPacket);
        }
    }
    c->endDecoding();
    return 0;
}

void Video::start()
{
    SDL_Rect e = calculateRenderRect(pCodecParameters->width, pCodecParameters->height);
    queue->lock();
    queue->setTarget(e);
    queue->unlock();
    while (!end())
    {

        SDL_Texture *t = pop();
        queue->lock();
        queue->setTexture(t);
        queue->unlock();
        show();
        SDL_Delay(1000 / 30);
    }
}

void Video::startDecoding()
{
    mtx.lock();
    SDL_CreateThread(decoder, "VideoDecoder", this);
    is_decoding = true;
    mtx.unlock();
}

void Video::endDecoding()
{
    mtx.lock();
    is_decoding = false;
    mtx.unlock();
}

bool Video::end()
{
    mtx.lock();
    bool e = is_decoding;
    int size  = mqueue.size();
    mtx.unlock();
    return (!e && size == 0);
}

SDL_Texture *Video::pop()
{
    std::unique_lock<std::mutex> lk(cmtx);
    cond.wait(lk, [this]{return mqueue.size() > 0;});
    SDL_Texture *t = mqueue.front();
    mqueue.pop();
    cond.notify_all();
    return t;
}

void Video::push(SDL_Texture *t)
{
    
    std::unique_lock<std::mutex> lk(cmtx);
    cond.wait(lk, [this]{return mqueue.size() < 120;});
    mqueue.push(t);
    cond.notify_all();
}