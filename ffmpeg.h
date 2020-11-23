#ifndef FFMPEG_H
#define FFMPEG_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <QDebug>
#include "publicfile.h"
#include <QImage>
#include <QPixmap>
#include <fstream>
#include <QFile>
#include <time.h>
#include <sys/time.h>


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


class FFMPEG
{
public:
    FFMPEG();
    ~FFMPEG();

    void InitFfmpeg(void);
    void CaptureFfmpeg(void);
    void UninitFfmpeg(void);
    QPixmap realTimeDisplay(void);

};



#endif // FFMPEG_H
