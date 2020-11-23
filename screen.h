#ifndef SCREEN_H
#define SCREEN_H

#include <QDialog>
#include <QVBoxLayout>
#include <QListView>
#include <QDir>
#include <QStringListModel>
#include <QSortFilterProxyModel>
#include <QMessageBox>
#include <QDebug>
#include <QImageReader>
#include "publicfile.h"
#include "showpicture.h"



namespace Ui {
class Screen;
}

class Screen : public QDialog
{
    Q_OBJECT

public:
    explicit Screen(QWidget *parent = 0);
    ~Screen();

    void filterInit(void);
    void showNewestPicture(QString pictureUrl);
    QString currentPictureUrl();

private:
    Ui::Screen *ui;
    QStringListModel* m_string_list_model;
    QSortFilterProxyModel* m_proxy_model;

    showpicture showpictures;

private slots:
    void quitScreen(void);
    void selectUpPicture(void);
    void selectDownPicture(void);
    void showPictureDialog(void);
    void onDoubleClick(const QModelIndex& index);

};

#endif // SCREEN_H
