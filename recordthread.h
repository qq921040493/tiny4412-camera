#ifndef RECORDTHREAD_H
#define RECORDTHREAD_H

#include <QThread>
#include "setting.h"
#include "publicfile.h"

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

class RecordThread : public QThread
{

public:
    RecordThread();
    ~RecordThread();
    void stop();

private:
    setting setParameter;

    void recording(int recordTimes);
    int flushEncoder(AVFormatContext *fmtCtx, unsigned int streamIndex);

protected:
    void run();

};


#endif // RECORDTHREAD_H
