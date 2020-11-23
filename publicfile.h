#ifndef PUBLICFILE_H
#define PUBLICFILE_H

#include <QString>
#include <QDateTime>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <QDebug>

#define INPUTNAME "video4linux2"
#define V4L2PATH  "/dev/video0"

#define EVMSFILE "/home/www/shexiangtou/"
#define EVMSSAVEPICTURE "/home/www/shexiangtou/Pictures/"
#define EVMSSAVEVIDEO "/home/www/shexiangtou/Videos/"

#define PICTURETAIL ".bmp"
#define VIDEOTAIL   ".h264"

#define OUT_WIDTH			640
#define OUT_HEIGHT			480
#define BI_RGB              0

#define FRAMEBUFFERSIZE OUT_HEIGHT*OUT_WIDTH*3

typedef unsigned short WORD;
typedef unsigned int DWORD;


class publicFile
{
public:
    publicFile();
    ~publicFile();


public:
    QString getLocalTime();
    char* qstringToChar(QString sourceTmp);
    QString separateQString(QString sourceTmp);
    int qstringToInt(QString sourceTmp);

    void CreateFileDirectory(void);
    int isDirExist(const char* dirPath);
    int createDir(const char* dirPath);


};



#endif // PUBLICFILE_H
