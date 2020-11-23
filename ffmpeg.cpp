#include "ffmpeg.h"
#include "externdata.h"

extern AVFormatContext *pFormatCtx;
extern AVCodecContext  *pCodecCtx;
extern AVCodec         *pCodec;
extern AVFrame *pFrame,*pFrameRGB;
extern unsigned int videoStream;
extern uint8_t *buf;

FFMPEG::FFMPEG()
{

}

FFMPEG::~FFMPEG()
{

}


void FFMPEG::InitFfmpeg(void)
{
    av_register_all();
    pFormatCtx = avformat_alloc_context();

    /* 注册设备 */
    avdevice_register_all();


    AVInputFormat *ifmt=av_find_input_format(INPUTNAME);

    /* 设置设备名称 */
    if(avformat_open_input(&pFormatCtx,V4L2PATH,ifmt,NULL)!=0)
    {
        qDebug()<<"Couldn't open input stream.\n";
    }

    if(avformat_find_stream_info(pFormatCtx,NULL)<0)
    {
        qDebug()<<"Couldn't find stream information.\n";
    }
}


void FFMPEG::CaptureFfmpeg(void)
{
    /* 获取视频数据 */
    for(int i=0;i<pFormatCtx->nb_streams;i++)
    {
        if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO)
        {
            videoStream = i ;
        }
    }

    if(videoStream == -1)
    {
        qDebug()<<"CaptureFfmpeg >> find video stream failed";
        exit(1);
    }
    pCodecCtx = pFormatCtx->streams[videoStream]->codec;

    pCodec = avcodec_find_decoder(pCodecCtx->codec_id);

    if(pCodec ==NULL)
    {
        qDebug()<< "CaptureFfmpeg >> avcode find decoder failed!";
        exit(1);
    }
    /* 打开解码器 */
    if(avcodec_open2(pCodecCtx,pCodec,NULL)<0)
    {
        qDebug()<<"CaptureFfmpeg >> avcode open failed!\n";
        exit(1);
    }

    /* 为每帧图像分配内存 */
    pFrame = av_frame_alloc();
    pFrameRGB = av_frame_alloc();

    if(pFrame==NULL||pFrameRGB==NULL)
    {
        qDebug()<<"CaptureFfmpeg >> av frame alloc failed!\n";
        exit(1);
    }

}

QPixmap FFMPEG::realTimeDisplay(void)
{
    QPixmap pImg;
    AVPacket packet;
    int frameFinished;
    struct SwsContext *pSwsCtx;
    int PictureSize;

    /* 获得帧图大小 */
    PictureSize = avpicture_get_size(AV_PIX_FMT_RGB32,pCodecCtx->width,pCodecCtx->height);

    buf = (uint8_t*)av_malloc(PictureSize);
    if(buf ==NULL)
    {
        qDebug()<<"CaptureFfmpeg >> av malloc failed!\n";
        exit(1);
    }

    avpicture_fill((AVPicture *)pFrameRGB,buf,AV_PIX_FMT_RGB32,pCodecCtx->width,pCodecCtx->height);
    pSwsCtx = sws_getContext(pCodecCtx->width,pCodecCtx->height,pCodecCtx->pix_fmt,pCodecCtx->width,pCodecCtx->height,AV_PIX_FMT_RGB32,SWS_BICUBIC,NULL,NULL,NULL);

    av_read_frame(pFormatCtx,&packet);
    if(packet.stream_index==videoStream)
    {
        /* 真正的解码 */
        avcodec_decode_video2(pCodecCtx,pFrame,&frameFinished,&packet);
        if(frameFinished)
        {
            /* 转换图像格式，将解压出来的YUV420P的图像转换为BRG24的图像 */
            sws_scale(pSwsCtx,pFrame->data,pFrame->linesize,0,pCodecCtx->height,pFrameRGB->data,pFrameRGB->linesize);
            QImage tmpImg((uchar *)buf,pCodecCtx->width,pCodecCtx->height,QImage::Format_RGB32);

            pImg = pImg.fromImage(tmpImg);

        }
        av_free_packet(&packet);
        sws_freeContext(pSwsCtx);
    }
    return pImg;
}

void FFMPEG::UninitFfmpeg(void)
{

    av_free(pFrame);
    av_free(pFrameRGB);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);
}

