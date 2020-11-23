#ifndef EXTERNDATA_H
#define EXTERNDATA_H

extern "C"{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavfilter/avfilter.h"
#include "libavdevice/avdevice.h"
#include "libavutil/opt.h"
#include "libavutil/imgutils.h"
#include "libswscale/swscale.h"
#include "libavutil/frame.h"
#include "libavutil/pixdesc.h"
}

AVFormatContext *pFormatCtx;
AVCodecContext  *pCodecCtx;
AVCodec         *pCodec;
AVFrame *pFrame,*pFrameRGB;
unsigned int videoStream;
uint8_t *buf;

#endif // EXTERNDATA_H
