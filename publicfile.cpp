#include "publicfile.h"


publicFile::publicFile()
{

}


publicFile::~publicFile()
{
}

/* 获取当前时间戳 */
QString publicFile::getLocalTime()
{
    QDateTime localTime = QDateTime::currentDateTime();
    QString currentTime = localTime.toString("yyyyMMddhhmmss");
    return currentTime;
}


/* QString 转换成  Char* */
char* publicFile::qstringToChar(QString sourceTmp)
{
    if(sourceTmp.isEmpty())
    {
        return NULL;
    }

    QByteArray ba = sourceTmp.toLatin1();
    char* targetTmp;
    targetTmp = ba.data();

    return targetTmp;
}


/* QString 拆分 */
QString publicFile::separateQString(QString sourceTmp)
{
    if(sourceTmp.isEmpty())
    {
        return NULL;
    }

    QString::SectionFlag flag = QString::SectionSkipEmpty;

    QString targetTmp;
    targetTmp = sourceTmp.section('/', 3, 3, flag);

    return targetTmp;
}

/* QString to int */
int publicFile::qstringToInt(QString sourceTmp)
{
    if(sourceTmp.isEmpty())
    {
        return -1;
    }

    bool ok;

    int ret = sourceTmp.toInt(&ok, 10);

    return ret;

}

void publicFile::CreateFileDirectory(void)
{
    static const char* dirs[] = {
        EVMSFILE,
        EVMSSAVEPICTURE,
        EVMSSAVEVIDEO,
        NULL
    };

    for(int i = 0; dirs[i] != NULL; ++i){
        if (isDirExist(dirs[i]) == -1){
            createDir(dirs[i]);
        }
    }
    return ;
}

/* 判断文件夹是否存在 */
int publicFile::isDirExist(const char* dirPath)
{
    if (dirPath == NULL)
    {
        return -1;
    }
    if (opendir(dirPath) == NULL)
    {
        return -1;
    }

    return 0;
}


/* 创建文件夹 */
int publicFile::createDir(const char* dirPath)
{
    int ret = mkdir(dirPath, 0777);

    if (ret == -1)
    {
        return -1;
    }

    return 0;
}

