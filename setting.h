#ifndef SETTING_H
#define SETTING_H

#include <QDialog>
#include <QDebug>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlRecord>
#include "publicfile.h"

namespace Ui {
class setting;
}

class setting : public QDialog
{
    Q_OBJECT

public:
    explicit setting(QWidget *parent = 0);
    ~setting();

    bool qtSqlInit(void);
    bool queryAll(void);
    bool updateById(int pictures, int videos);
    int queryPictures(void);
    int queryVideos(void);


private:
    Ui::setting *ui;
    publicFile pf;

private slots:
    void quitSetting(void);
    void saveParameter(void);
};

#endif // SETTING_H
