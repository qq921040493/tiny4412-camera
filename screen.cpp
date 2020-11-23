#include "screen.h"
#include "ui_screen.h"

Screen::Screen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Screen)
{
    ui->setupUi(this);
    setGeometry(0, 0, 820, 620);
    setWindowTitle(tr("Screenshot preview"));

    m_string_list_model = new QStringListModel();
    m_proxy_model = new QSortFilterProxyModel();

    connect(ui->quitButton, SIGNAL(clicked()),this,SLOT(quitScreen()));
//    connect(ui->upButton, SIGNAL(clicked()), this, SLOT(selectUpPicture()));
//    connect(ui->downButton, SIGNAL(clicked()), this, SLOT(selectDownPicture()));
    connect(ui->privewButton, SIGNAL(clicked()), this, SLOT(showPictureDialog()));
    connect(ui->listView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onDoubleClick(QModelIndex)));
}

Screen::~Screen()
{
    delete ui;
}

void Screen::quitScreen(void)
{
    delete m_string_list_model;
    delete m_proxy_model;
    this->close();
}


void Screen::filterInit(void)
{
    QDir* dir = new QDir(EVMSSAVEPICTURE);

    QStringList filters;
    filters << "*.bmp";

    dir->setNameFilters(filters);

    QStringList file_list = dir->entryList();

    m_string_list_model->setStringList(file_list);
    m_proxy_model->setSourceModel(m_string_list_model);

    ui->listView->setModel(m_proxy_model);

    //设置当前选中行 0行 0列
    QModelIndex index = m_proxy_model->index(0, 0);
    ui->listView->setCurrentIndex(index);
}


void Screen::onDoubleClick(const QModelIndex& index)
{
    QString pictureName = index.data().toString();
    qDebug()<<"picture name is " << pictureName;
    showpictures.show();
    showpictures.showBmpPictures(pictureName);
    showpictures.exec();
}

void Screen::selectUpPicture(void)
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

    //当前选中
    curr_index = ui->listView->currentIndex();
}

void Screen::selectDownPicture(void)
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

    //当前选中
    curr_index = ui->listView->currentIndex();
}

void Screen::showPictureDialog(void)
{
    QModelIndex curr_index = ui->listView->currentIndex();
    QString pictureName = curr_index.data().toString();
    showpictures.show();
    showpictures.showBmpPictures(pictureName);
    showpictures.exec();
}
