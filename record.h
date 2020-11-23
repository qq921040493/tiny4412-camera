#ifndef RECORD_H
#define RECORD_H

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
#include "decoder.h"
#include "playvideo.h"


namespace Ui {
class record;
}

class record : public QDialog
{
    Q_OBJECT

public:
    explicit record(QWidget *parent = 0);
    ~record();

    void recordFilterInit(void);
    void delay(int msec);
    void display(char* fileName);

private:
    Ui::record *ui;
    Decoder * dec;
    QListView* m_file_list_view;
    QStringListModel* m_string_list_model;
    QSortFilterProxyModel* m_proxy_model;
    publicFile pf;
    playvideo playvideos;

    void play(QImage image);
    int findStartCode (unsigned char *buf, int zeros_in_startcode);
    int getNextNal(FILE* inpf, unsigned char* buf);

private slots:
    void quitRecord(void);
    void selectUpVideo(void);
    void selectDownVideo(void);
    void clickPlayButton(void);
    void onDoubleClick(const QModelIndex& index);
};

#endif // RECORD_H
