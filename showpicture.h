#ifndef SHOWPICTURE_H
#define SHOWPICTURE_H

#include <QDialog>
#include <QImageReader>
#include "publicfile.h"

namespace Ui {
class showpicture;
}

class showpicture : public QDialog
{
    Q_OBJECT

public:
    explicit showpicture(QWidget *parent = 0);
    ~showpicture();

    void showBmpPictures(QString pictureName);

private:
    Ui::showpicture *ui;

private slots:
    void quitShowPicture(void);

};

#endif // SHOWPICTURE_H
