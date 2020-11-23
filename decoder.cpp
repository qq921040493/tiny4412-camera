#include "decoder.h"

Decoder::Decoder()
{
    createYUVTab_16();
}

Decoder::~Decoder()
{
    deleteYUVTab();
}

void Decoder::deleteYUVTab()
{
    av_free(colortab);
    av_free(rgb_2_pix);
}

void Decoder::createYUVTab_16()
{
    int i;
    int u, v;

    colortab = (int *)av_malloc(4*256*sizeof(int));
    u_b_tab = &colortab[0*256];
    u_g_tab = &colortab[1*256];
    v_g_tab = &colortab[2*256];
    v_r_tab = &colortab[3*256];

    for (i=0; i<256; i++)
    {
        u = v = (i-128);

        u_b_tab[i] = (int) ( 1.772 * u);
        u_g_tab[i] = (int) ( 0.34414 * u);
        v_g_tab[i] = (int) ( 0.71414 * v);
        v_r_tab[i] = (int) ( 1.402 * v);
    }

    rgb_2_pix = (unsigned int *)av_malloc(3*768*sizeof(unsigned int));

    r_2_pix = &rgb_2_pix[0*768];
    g_2_pix = &rgb_2_pix[1*768];
    b_2_pix = &rgb_2_pix[2*768];

    for(i=0; i<256; i++)
    {
        r_2_pix[i] = 0;
        g_2_pix[i] = 0;
        b_2_pix[i] = 0;
    }

    for(i=0; i<256; i++)
    {
        r_2_pix[i+256] = (i & 0xF8) << 8;
        g_2_pix[i+256] = (i & 0xFC) << 3;
        b_2_pix[i+256] = (i ) >> 3;
    }

    for(i=0; i<256; i++)
    {
        r_2_pix[i+512] = 0xF8 << 8;
        g_2_pix[i+512] = 0xFC << 3;
        b_2_pix[i+512] = 0x1F;
    }

    r_2_pix += 256;
    g_2_pix += 256;
    b_2_pix += 256;
}

void Decoder::displayYUV_16(unsigned int *pdst, unsigned char *y, unsigned char *u, unsigned char *v, int width, int height, int src_ystride, int src_uvstride, int dst_ystride)
{
    int i, j;
    int r, g, b, rgb;

    int yy, ub, ug, vg, vr;

    unsigned char* yoff;
    unsigned char* uoff;
    unsigned char* voff;

    int width2 = width/2;
    int height2 = height/2;

    for(j=0; j<height2; j++)
    {
        yoff = y + j * 2 * src_ystride;
        uoff = u + j * src_uvstride;
        voff = v + j * src_uvstride;

        for(i=0; i<width2; i++)
        {
            yy  = *(yoff+(i<<1));
            ub = u_b_tab[*(uoff+i)];
            ug = u_g_tab[*(uoff+i)];
            vg = v_g_tab[*(voff+i)];
            vr = v_r_tab[*(voff+i)];

            b = yy + ub;
            g = yy - ug - vg;
            r = yy + vr;

            rgb = r_2_pix[r] + g_2_pix[g] + b_2_pix[b];

            yy = *(yoff+(i<<1)+1);
            b = yy + ub;
            g = yy - ug - vg;
            r = yy + vr;

            pdst[(j*dst_ystride+i)] = (rgb)+((r_2_pix[r] + g_2_pix[g] + b_2_pix[b])<<16);

            yy = *(yoff+(i<<1)+src_ystride);
            b = yy + ub;
            g = yy - ug - vg;
            r = yy + vr;

            rgb = r_2_pix[r] + g_2_pix[g] + b_2_pix[b];

            yy = *(yoff+(i<<1)+src_ystride+1);
            b = yy + ub;
            g = yy - ug - vg;
            r = yy + vr;

            pdst [((2*j+1)*dst_ystride+i*2)>>1] = (rgb)+((r_2_pix[r] + g_2_pix[g] + b_2_pix[b])<<16);
        }
    }
}
int Decoder::initial()
{
    avcodec_register_all();
    av_init_packet(&packet);

    codec = avcodec_find_decoder(AV_CODEC_ID_H264);
    if (!codec)
    {
        qDebug()<<"avcodec_find_encoder failed";
        return -1;
    }

    context = avcodec_alloc_context3(codec);
    if (!context)
    {
        qDebug()<<"avcodec_alloc_context3 failed";
        return -2;
    }

    context->codec_type = AVMEDIA_TYPE_VIDEO;
    context->pix_fmt = AV_PIX_FMT_YUV420P;

    if (avcodec_open2(context, codec, NULL) < 0)
    {
        qDebug()<<"avcodec_open2 failed";
        return -3;
    }

    frame = av_frame_alloc();
    if (!frame)
    {
        qDebug()<<"av frame alloc failed";
        return -4;
    }

    return 0;
}

void Decoder::unInitial()
{
    avcodec_close(context);
    av_free(context);
    av_frame_free(&frame);
}

int Decoder::decode(uint8_t *pDataIn, int nInSize, uint8_t *pDataOut,int *nWidth, int *nHeight)
{
    av_init_packet(&packet);
    packet.size = nInSize;
    packet.data = pDataIn;

    if (packet.size > 0)
    {
        int got_picture=0;
        int ret= avcodec_decode_video2(context, frame, &got_picture, &packet);
        if (ret < 0)
        {
            qDebug()<<"avcodec_encode_video2 failed";
            return -2;
        }

        if (got_picture)
        {
            *nWidth = context->width;
            *nHeight = context->height;

             displayYUV_16((unsigned int*)pDataOut, frame->data[0], frame->data[1],frame->data[2],
                     *nWidth,*nHeight,frame->linesize[0],frame->linesize[2],*nWidth);
        }
    }
    else
    {
        qDebug()<<"no data to decode";
        return -1;
    }

    return 0;
}

