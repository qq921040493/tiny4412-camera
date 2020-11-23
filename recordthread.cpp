#include "recordthread.h"

extern AVFormatContext *pFormatCtx;
extern AVCodecContext  *pCodecCtx;
extern AVCodec         *pCodec;
extern AVFrame *pFrame,*pFrameRGB;
extern unsigned int videoStream;
extern uint8_t *buf;

RecordThread::RecordThread()
{

}

RecordThread::~RecordThread()
{

}

void RecordThread::stop()
{

}

void RecordThread::run()
{
    int videos = setParameter.queryVideos();
    recording(videos);
}

void RecordThread::recording(int recordTimes)
{
    publicFile pf;
    AVPacket  *packet;
    uint8_t *src_data[4];
    uint8_t *dst_data[4];
    int src_linesize[4];
    int dst_linesize[4];
    int src_bufsize;
    int dst_bufsize;
    int ret;
    int PictureSize;

    /* 获得帧图大小 */
    PictureSize = avpicture_get_size(AV_PIX_FMT_YUV420P,pCodecCtx->width,pCodecCtx->height);
    buf = (uint8_t*)av_malloc(PictureSize);
    if(buf ==NULL)
    {
        qDebug()<<"recording >> av malloc failed!\n";
        exit(1);
    }

    avpicture_fill((AVPicture *)pFrameRGB,buf,AV_PIX_FMT_YUV420P,pCodecCtx->width,pCodecCtx->height);
    struct SwsContext *pSwsCtx;
    pSwsCtx = sws_getContext(pCodecCtx->width,pCodecCtx->height,pCodecCtx->pix_fmt,pCodecCtx->width,pCodecCtx->height,AV_PIX_FMT_YUV420P,SWS_BILINEAR,NULL,NULL,NULL);


    src_bufsize = av_image_alloc(src_data, src_linesize, pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, 16);
    dst_bufsize = av_image_alloc(dst_data, dst_linesize, OUT_WIDTH, OUT_HEIGHT, AV_PIX_FMT_YUV420P, 1);


    packet = (AVPacket *)av_malloc(sizeof(AVPacket));

    /* set out format */
    AVFormatContext		*outFormatCtx;
    AVOutputFormat		*outfmt;
    AVCodecContext		*outCodecCtx;
    AVStream			*video_st;
    AVDictionary 		*param = 0;
    AVCodec				*outCodec;
    AVFrame				*outFrame;
    AVPacket 			outpkt;

    uint8_t *picture_buf;
    int 	picture_size;
    int 	y_size;
    int		got_picture;
    int 	loop = 0;


    outFormatCtx = avformat_alloc_context();


    QString localTime = pf.getLocalTime();

    char* LocalTime = pf.qstringToChar(localTime);

    char* videoName = new char[50];
    strcpy(videoName, EVMSSAVEVIDEO);
    strcat(videoName, LocalTime);
    strcat(videoName, VIDEOTAIL);

    outfmt = av_guess_format(NULL, videoName, NULL);
    outFormatCtx->oformat = outfmt;

    if (avio_open(&outFormatCtx->pb,videoName, AVIO_FLAG_READ_WRITE) < 0)
    {
        qDebug()<<"recording >> Failed to open output file!\n";
        return;
    }

    video_st = avformat_new_stream(outFormatCtx, 0);
    if (video_st==NULL)
    {
        qDebug()<<"recording >> creat new stream err";
        return;
    }

    outCodecCtx = video_st->codec;
    outCodecCtx->codec_id = outfmt->video_codec;
    outCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
    outCodecCtx->pix_fmt = AV_PIX_FMT_YUV420P;
    outCodecCtx->width = OUT_WIDTH;
    outCodecCtx->height = OUT_HEIGHT;
    outCodecCtx->bit_rate = 1500000;
    outCodecCtx->gop_size=250;

    outCodecCtx->time_base.num = 1;
    outCodecCtx->time_base.den = 25;
    outCodecCtx->qmin = 10;
    outCodecCtx->qmax = 51;


    if(pCodecCtx->codec_id == AV_CODEC_ID_H264) {
        av_dict_set(&param,"preset", "faster", 0);
        //av_dict_set(&param,"preset", "slow", 0);
        av_dict_set(&param,"tune", "zerolatency", 0);
    }

    av_dump_format(outFormatCtx, 0, videoName, 1);

    outCodec = avcodec_find_encoder(outCodecCtx->codec_id);
    if (!outCodec)
    {
        qDebug()<<"recording >> Can not find encoder! \n";
        return;
    }

    if (avcodec_open2(outCodecCtx, outCodec, &param) < 0)
    {
        qDebug()<<"recording >> Failed to open encoder! \n";
        return;
    }

    outFrame = av_frame_alloc();
    picture_size = avpicture_get_size(outCodecCtx->pix_fmt, outCodecCtx->width, outCodecCtx->height);
    picture_buf = (uint8_t *)av_malloc(picture_size);
    avpicture_fill((AVPicture *)outFrame, picture_buf, outCodecCtx->pix_fmt, outCodecCtx->width, outCodecCtx->height);
    outFrame->format = outCodecCtx->pix_fmt;
    outFrame->width  = outCodecCtx->width;
    outFrame->height = outCodecCtx->height;


    avformat_write_header(outFormatCtx,NULL);
    av_new_packet(&outpkt,picture_size);
    y_size = outCodecCtx->width * outCodecCtx->height;

    /* 1秒=30帧  N秒=30*N帧 */
    while(loop++ < recordTimes*30)
    {
        av_read_frame(pFormatCtx, packet);
        memcpy(src_data[0], packet->data, packet->size);
        sws_scale(pSwsCtx, src_data,  src_linesize, 0, pCodecCtx->height, dst_data, dst_linesize);

        outFrame->data[0] = dst_data[0];
        outFrame->data[1] = dst_data[0] + y_size;
        outFrame->data[2] = dst_data[0] + y_size*5/4;

        outFrame->pts=(loop -1)*(video_st->time_base.den)/((video_st->time_base.num)*25);
        ret = avcodec_encode_video2(outCodecCtx, &outpkt, outFrame, &got_picture);
        if(ret < 0)
        {
            qDebug()<<"Failed to encode! \n";
            return;
        }

        if(got_picture==1)
        {
            outpkt.stream_index = video_st->index;
            ret = av_write_frame(outFormatCtx, &outpkt);
            av_free_packet(&outpkt);
        }
    }

    ret = flushEncoder(outFormatCtx,0);
    if(ret < 0)
    {
        qDebug()<<"recording >> Flushing encoder failed\n";
        return;
    }

    av_write_trailer(outFormatCtx);
    if (video_st)
    {
        avcodec_close(video_st->codec);
        av_free(outFrame);
        av_free(picture_buf);
    }
    avio_close(outFormatCtx->pb);
    avformat_free_context(outFormatCtx);

    av_free_packet(packet);
    delete []videoName;
}

int RecordThread::flushEncoder(AVFormatContext *fmtCtx, unsigned int streamIndex)
{
    int ret;
    int getFrame;
    AVPacket encPkt;

    if (!(fmtCtx->streams[streamIndex]->codec->codec->capabilities & CODEC_CAP_DELAY))
    {
        return 0;
    }

    while (1)
    {
        encPkt.data = NULL;
        encPkt.size = 0;
        av_init_packet(&encPkt);
        ret = avcodec_encode_video2 (fmtCtx->streams[streamIndex]->codec, &encPkt, NULL, &getFrame);
        av_frame_free(NULL);
        if (ret < 0)
        {
            break;
        }
        if (!getFrame)
        {
            ret=0;
            break;
        }
       // qDebug()<<"Flush Encoder: Succeed to encode 1 frame!";
        ret = av_write_frame(fmtCtx, &encPkt);
        if (ret < 0)
        {
            break;
        }
    }
    return ret;
}
