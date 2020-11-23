#include "playvideo.h"
#include "ui_playvideo.h"

playvideo::playvideo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::playvideo)
{
    ui->setupUi(this);
    setWindowTitle(tr("Videos"));
    connect(ui->quitButton, SIGNAL(clicked()),this,SLOT(quitPlayVideo()));
}

playvideo::~playvideo()
{
    delete ui;
}

void playvideo::quitPlayVideo(void)
{
    this->close();
}

void playvideo::display(char* fileName)
{
    FILE * fp = fopen(fileName, "rb");
    if(!fp)
    {
        return;
    }


    int inLen=0;
    unsigned char* inBuf=NULL;
    unsigned char* outBuf=NULL;
    int height=0;
    int width=0;

    inBuf = (unsigned char*)calloc ( 500000, sizeof(char));
    outBuf = (unsigned char*)calloc ( 1000000, sizeof(char));
    while(!feof(fp))
    {
        memset(outBuf,0,1000000);
        inLen = getNextNal(fp, inBuf);

        int result= dec.decode(inBuf, inLen,outBuf, &width, &height);

        if(result>=0)
        {
            QImage image= QImage(outBuf, width, height, QImage::Format_RGB16);
            if(!image.isNull())
            {
                play(image);
            }
        }
       delay(40);
    }
}

void playvideo::delay(int msec)
{
    QTime dieTime= QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
    }
}

void playvideo::play(QImage image)
{
    QImage img=image.scaled(ui->showVideo->width(),ui->showVideo->height(),Qt::KeepAspectRatio);

    ui->showVideo->setPixmap(QPixmap::fromImage(img));
}

int playvideo::findStartCode (unsigned char *buf, int zeros_in_startcode)
{
    int info;
    int i;

    info = 1;
    for (i = 0; i < zeros_in_startcode; i++)
    {
        if(buf[i] != 0)
        {
            info = 0;
        }
    }
    if(buf[i] != 1)
    {
        info = 0;
    }
    return info;
}

int playvideo::getNextNal(FILE* fp, unsigned char* buf)
{
    int pos = 0;
    int startCodeFound = 0;
    int info2 = 0;
    int info3 = 0;

    while(!feof(fp) && (buf[pos++]=fgetc(fp))==0);

    while (!startCodeFound)
    {
        if (feof (fp))
        {
            return pos-1;
        }
        buf[pos++] = fgetc (fp);
        info3 = findStartCode(&buf[pos-4], 3);
        if(info3 != 1)
            info2 = findStartCode(&buf[pos-3], 2);
        startCodeFound = (info2 == 1 || info3 == 1);
    }
    if(info2)
    {
        fseek (fp, -3, SEEK_CUR);
        return pos - 3;
    }
    if(info3)
    {
        fseek (fp, -4, SEEK_CUR);
        return pos - 4;
    }
}
