#ifndef SCREENTHREAD_H
#define SCREENTHREAD_H

#include <QThread>
#include "setting.h"
#include "publicfile.h"
#include "yuyvtorgb888.h"

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

typedef struct _BITMAPFILEHEADER{
    WORD     bfType;                //BMP文件头，必须为BM
    DWORD    bfSize;                //文件大小，字节为单位
    DWORD    bfReserved1;            //保留字，值为0
    DWORD    bfReserved2;
    DWORD    bfOffBits;             //位图数据起始位置，此处取54
}BITMAPFILEHEADER;
typedef struct _BITMAPINFOHEADER{
    DWORD    biSize;               //本结构所占字节数，此处取40
    DWORD    biWidth;              //图片宽度，像素为单位
    DWORD    biHeight;             //高度，同上
    WORD     biPlanes;             //目标设备级别，此处必须为1
    WORD     biBitCount;           //像素位数，此处取24（真彩色）
    DWORD    biCompression;        //位图类型，必须为0
    DWORD    biSizeImage;          //位图大小
    DWORD    biXPelsPerMeter;      //水平像素数，此处取0
    DWORD    biYPelsPerMeter;      //竖直像素数，此处取0
    DWORD    biClrUsed;            //位图实际使用的颜色表中的颜色数，此处取0
    DWORD    biClrImportant;       //位图显示过程中重要的颜色数，此处取0
}BITMAPINFOHEADER;


class ScreenThread : public QThread
{

public:
    ScreenThread();
    ~ScreenThread();
    void stop();

private:
    setting setParameter;

    BITMAPFILEHEADER   bf;
    BITMAPINFOHEADER   bi;

    unsigned char frame_buffer[FRAMEBUFFERSIZE];
    bool YUYV_to_RGB888(AVPacket packet);
    void takePhotos(void);

protected:
    void run();


};



#endif // SCREENTHREAD_H
