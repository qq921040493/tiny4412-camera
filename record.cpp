#include "record.h"
#include "ui_record.h"

record::record(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::record)
{
    ui->setupUi(this);
    setGeometry(0, 0, 820, 620);
    setWindowTitle(tr("Views preview"));

    dec=new Decoder();
    dec->initial();

    m_file_list_view = new QListView();
    m_string_list_model = new QStringListModel();
    m_proxy_model = new QSortFilterProxyModel();

    connect(ui->quitButton, SIGNAL(clicked()),this,SLOT(quitRecord()));
//    connect(ui->upButton, SIGNAL(clicked()), this, SLOT(selectUpVideo()));
//    connect(ui->downButton, SIGNAL(clicked()), this, SLOT(selectDownVideo()));
    connect(ui->playButton, SIGNAL(clicked()), this, SLOT(clickPlayButton()));
    connect(ui->listView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onDoubleClick(QModelIndex)));
}

record::~record()
{
    dec->unInitial();
    delete dec;
    delete ui;
}


void record::quitRecord(void)
{
    delete m_file_list_view;
    delete m_string_list_model;
    delete m_proxy_model;
    this->close();
}

void record::onDoubleClick(const QModelIndex& index)
{
    QString pictureName = index.data().toString();
    qDebug()<<"video name is " << pictureName;
    QString picturePath = EVMSSAVEVIDEO + pictureName;
}

void record::recordFilterInit(void)
{
    QDir* dir = new QDir(EVMSSAVEVIDEO);

    QStringList filters;
    filters << "*.h264";

    dir->setNameFilters(filters);

    QStringList file_list = dir->entryList();

    m_string_list_model->setStringList(file_list);
    m_proxy_model->setSourceModel(m_string_list_model);

    ui->listView->setModel(m_proxy_model);

    //设置当前选中行 0行 0列
    QModelIndex index = m_proxy_model->index(0, 0);
    ui->listView->setCurrentIndex(index);
}


void record::selectUpVideo(void)
{
    QModelIndex curr_index = ui->listView->currentIndex();
    int curr_row = curr_index.row();

    if (curr_row == 0)
    {
         QMessageBox::information(this, tr("提示"), tr("前面没有了"), QMessageBox::Ok);
         return ;
    }

    curr_row = curr_row - 1;

    curr_index = m_proxy_model->index(curr_row, 0);
    ui->listView->setCurrentIndex(curr_index);
}

void record::selectDownVideo(void)
{
    QModelIndex curr_index = ui->listView->currentIndex();
    int curr_row = curr_index.row();

    int rows = m_proxy_model->rowCount();

    if (curr_row == (rows - 1)) {
        QMessageBox::information(this, tr("提示"), tr("后面没有了"), QMessageBox::Ok);
        return ;
    }

    curr_row = curr_row + 1;

    curr_index = m_proxy_model->index(curr_row, 0);
    ui->listView->setCurrentIndex(curr_index);

}

void record::delay(int msec)
{
    QTime dieTime= QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
}

void record::display(char* fileName)
{
    FILE * inpf=NULL;
    int inLen=0;
    unsigned char* inBuf=NULL;
    unsigned char* outBuf=NULL;
    int height=0;
    int width=0;
    inpf = fopen(fileName, "rb");
    if(!inpf)
        return;

    inBuf = (unsigned char*)calloc ( 500000, sizeof(char));
    outBuf = (unsigned char*)calloc ( 1000000, sizeof(char));
    while(!feof(inpf))
    {
        memset(outBuf,0,1000000);
        inLen = getNextNal(inpf, inBuf);

        int result= dec->decode(inBuf, inLen,outBuf, &width, &height);

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

void record::play(QImage image)
{
    QImage img=image.scaled(ui->showVideo->width(),ui->showVideo->height(),Qt::KeepAspectRatio);

    ui->showVideo->setPixmap(QPixmap::fromImage(img));
}

int record::findStartCode (unsigned char *buf, int zeros_in_startcode)
{
    int info;
    int i;

    info = 1;
    for (i = 0; i < zeros_in_startcode; i++)
        if(buf[i] != 0)
            info = 0;

    if(buf[i] != 1)
        info = 0;
    return info;
}

int record::getNextNal(FILE* inpf, unsigned char* buf)
{
    int pos = 0;
    int startCodeFound = 0;
    int info2 = 0;
    int info3 = 0;

    while(!feof(inpf) && (buf[pos++]=fgetc(inpf))==0);

    while (!startCodeFound)
    {
        if (feof (inpf))
        {
            return pos-1;
        }
        buf[pos++] = fgetc (inpf);
        info3 = findStartCode(&buf[pos-4], 3);
        if(info3 != 1)
            info2 = findStartCode(&buf[pos-3], 2);
        startCodeFound = (info2 == 1 || info3 == 1);
    }
    if(info2)
    {
        fseek (inpf, -3, SEEK_CUR);
        return pos - 3;
    }
    if(info3)
    {
        fseek (inpf, -4, SEEK_CUR);
        return pos - 4;
    }
}


void record::clickPlayButton(void)
{
    QModelIndex curr_index = ui->listView->currentIndex();

    QString VideoName = curr_index.data().toString();
    ui->videoName->setText(VideoName);

    QString VideoPath = EVMSSAVEVIDEO + VideoName;

    display(pf.qstringToChar(VideoPath));
}
