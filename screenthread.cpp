#include "screenthread.h"

extern AVFormatContext *pFormatCtx;

ScreenThread::ScreenThread()
{
    bi.biSize = 40;
    bi.biWidth = OUT_WIDTH;
    bi.biHeight = OUT_HEIGHT;
    bi.biPlanes = 1;
    bi.biBitCount = 24;
    bi.biCompression = 0;
    bi.biSizeImage = OUT_WIDTH*OUT_HEIGHT*3;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    bf.bfType = 0x4d42;
    bf.bfSize = 54 + bi.biSizeImage;
    bf.bfReserved1 = 0;
    bf.bfReserved2 = 0;
    bf.bfOffBits = 54;
}

ScreenThread::~ScreenThread()
{

}

void ScreenThread::stop()
{

}

void ScreenThread::run()
{
    int pictures = setParameter.queryPictures();
    while(pictures--)
    {
        takePhotos();
        sleep(1);
    }
}

void ScreenThread::takePhotos(void)
{
    AVPacket packet;
    publicFile pf;


    QString localTime = pf.getLocalTime();

    char* LocalTime = pf.qstringToChar(localTime);

    char* filename = new char[50];
    strcpy(filename, EVMSSAVEPICTURE);
    strcat(filename, LocalTime);
    strcat(filename, PICTURETAIL);

    //从摄像头获取图像数据
    if( 0 == av_read_frame(pFormatCtx,&packet))
    {
        memset(frame_buffer, 0, FRAMEBUFFERSIZE);
        YUYV_to_RGB888(packet);
        FILE *fp1;
        fp1=fopen(filename,"wb");
        if(fp1==NULL)
        {
            qDebug()<<"takePhotos >> open file fail";
            return;
        }
        fwrite(&bf.bfType, 14, 1, fp1);
        fwrite(&bi.biSize, 40, 1, fp1);
        fwrite(frame_buffer, bi.biSizeImage, 1, fp1);
        fclose(fp1);
        delete []filename;

    }
}

bool ScreenThread::YUYV_to_RGB888(AVPacket packet)
{
    int i,j;
    unsigned char y1,y2,u,v,r1,b1,r2,b2;
    int     g1,g2;
    char    *pointer;
    int high,width;

    pointer = (char *)packet.data;
    high=OUT_HEIGHT;
    width=OUT_WIDTH/2;
    for(i=0;i<high;i++)
    {
        for(j=0;j<(OUT_WIDTH/2);j++)
        {
            y1 = *( pointer + (i*width+j)*4);
            u  = *( pointer + (i*width+j)*4 + 1);
            y2 = *( pointer + (i*width+j)*4 + 2);
            v  = *( pointer + (i*width+j)*4 + 3);


            r1=(int)R_table[y1][v];                     //r1 = y1 + 1.042*(v-128);
            g1=y1-UV_table[u][v];                       //g1 = y1 - 0.34414*(u-128)-0.71414*(v-128);
            b1=(int)B_table[y1][u];                     //b1 = y1 + 1.772*(u-128);

            r2=(int)R_table[y2][v];                     //r2 = y2 + 1.042*(v-128);
            g2=y2-UV_table[u][v];                       //g2 = y2-0.34414*(u-128)-0.71414*(v-128);
            b2=(int)B_table[y2][u];                     //b2 = y2 + 1.772*(u-128);

            if(g1>255)      g1 = 255;
            else if(g1<0)   g1 = 0;

            if(g2>255)      g2 = 255;
            else if(g2<0)   g2 = 0;

            *(frame_buffer + ((high-1-i)*width+j)*6    ) = b1;
            *(frame_buffer + ((high-1-i)*width+j)*6 + 1) = (unsigned char)g1;
            *(frame_buffer + ((high-1-i)*width+j)*6 + 2) = r1;
            *(frame_buffer + ((high-1-i)*width+j)*6 + 3) = b2;
            *(frame_buffer + ((high-1-i)*width+j)*6 + 4) = (unsigned char)g2;
            *(frame_buffer + ((high-1-i)*width+j)*6 + 5) = r2;
        }
    }
    return true;
}
