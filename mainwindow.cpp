#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("shipinjiankong"));
    setGeometry(0, 0, 820, 620);

    deviceInit();

    /* 信号与槽函数 */
    connect(ui->quitButton, SIGNAL(clicked()),this,SLOT(quitMainWindow()));
    connect(ui->settingButton, SIGNAL(clicked()),this,SLOT(clickSettingButton()));
    connect(ui->screenButton, SIGNAL(clicked()), this, SLOT(clickTakePhotoButton()));
    connect(ui->recordButton, SIGNAL(clicked()), this, SLOT(clickRecordButton()));
    connect(ui->screenShowButton, SIGNAL(clicked()), this, SLOT(clickScreenShowButton()));
    connect(ui->videoShowButton, SIGNAL(clicked()), this, SLOT(clickVideoShowButton()));

    /* 使用QTimer 定时器*/
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showtime()));
    timer->start(33);

}


/* 设备初始化 */
void MainWindow::deviceInit(void)
{
    /* 创建文件系统 */
    pf.CreateFileDirectory();

    /* 初始化ffmpeg */
    ffmpeg.InitFfmpeg();

    /* ffmpeg开始采集 */
    ffmpeg.CaptureFfmpeg();

    /* 数据库初始化 */
    setParameter.qtSqlInit();
}


MainWindow::~MainWindow()
{
    delete ui;
    ffmpeg.UninitFfmpeg();
}

void MainWindow::quitMainWindow(void)
{
    this->close();
}

void MainWindow::clickSettingButton(void)
{
    setParameter.show();
    setParameter.exec();
}

void MainWindow::showtime(void)
{
    ui->carema->setPixmap(ffmpeg.realTimeDisplay());
}

void MainWindow::clickTakePhotoButton(void)
{
    screenthread.start();
    QMessageBox::information(NULL,"success!","taken",QMessageBox::Ok);
}

void MainWindow::clickRecordButton(void)
{
    recordthread.start();
}

void MainWindow::clickScreenShowButton(void)
{
    screen.filterInit();
    this->hide();
    screen.show();
    screen.exec();
    this->show();
}

void MainWindow::clickVideoShowButton(void)
{
    recording.recordFilterInit();
    this->hide();
    recording.show();
    recording.exec();
    this->show();
}



