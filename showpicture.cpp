#include "showpicture.h"
#include "ui_showpicture.h"

showpicture::showpicture(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::showpicture)
{
    ui->setupUi(this);
    setWindowTitle(tr("Show picture"));
    connect(ui->quitButton, SIGNAL(clicked()),this,SLOT(quitShowPicture()));
}

showpicture::~showpicture()
{
    delete ui;
}


void showpicture::quitShowPicture(void)
{
    this->close();
}


void showpicture::showBmpPictures(QString pictureName)
{
    QString picturePath = EVMSSAVEPICTURE + pictureName;
    QImageReader reader(picturePath);
    reader.setAutoTransform(true);
    const QImage newImage = reader.read();
    ui->showPicture->setPixmap(QPixmap::fromImage(newImage));
    ui->pictureName->setText(pictureName);
}
