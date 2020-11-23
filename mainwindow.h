#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "setting.h"
#include "ffmpeg.h"
#include "screen.h"
#include "record.h"
#include "screenthread.h"
#include "recordthread.h"
#include "publicfile.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    setting setParameter;
    FFMPEG ffmpeg;
    publicFile pf;
    Screen screen;
    record recording;
    ScreenThread screenthread;
    RecordThread recordthread;

    void deviceInit(void);

private slots:
    void quitMainWindow(void);
    void clickSettingButton(void);
    void clickTakePhotoButton(void);
    void clickRecordButton(void);
    void clickScreenShowButton(void);
    void clickVideoShowButton(void);
    void showtime(void);
};

#endif // MAINWINDOW_H
