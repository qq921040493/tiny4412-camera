#ifndef PLAYVIDEO_H
#define PLAYVIDEO_H

#include <QDialog>
#include <QTime>
#include "decoder.h"

namespace Ui {
class playvideo;
}

class playvideo : public QDialog
{
    Q_OBJECT

public:
    explicit playvideo(QWidget *parent = 0);
    ~playvideo();

    void display(char* fileName);

private:
    Ui::playvideo *ui;
    Decoder dec;

    void delay(int msec);
    void play(QImage image);
    int findStartCode (unsigned char *buf, int zeros_in_startcode);
    int getNextNal(FILE* fp, unsigned char* buf);


private slots:
    void quitPlayVideo(void);
};

#endif // PLAYVIDEO_H
