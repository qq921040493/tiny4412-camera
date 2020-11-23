#include "setting.h"
#include "ui_setting.h"

setting::setting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setting)
{
    ui->setupUi(this);
    setGeometry(0, 0, 500, 500);
    connect(ui->quitButton, SIGNAL(clicked()),this,SLOT(quitSetting()));
    connect(ui->saveButton, SIGNAL(clicked()),this,SLOT(saveParameter()));
}

setting::~setting()
{
    delete ui;
}

bool setting::qtSqlInit(void)
{
    QSqlDatabase db1 = QSqlDatabase::addDatabase("QSQLITE");
    db1.setDatabaseName("qtsql");
    if(!db1.open())
    {
        qDebug()<<"connect sql failure!";
        return false;
    }

    QSqlDatabase db = QSqlDatabase::database("QSQLITE");

    QSqlQuery query(db);
    bool createStatus = query.exec("CREATE TABLE qtsql(qt_id INTEGER PRIMARY KEY, qt_setting VARCHAR, qt_pictures INTEGER, qt_videos INTEGER)");
    if(!createStatus)
    {
        qDebug()<<"create sql data failure or the sql has been created!";
        return false;
    }

    query.prepare("INSERT INTO qtsql(qt_id, qt_setting, qt_pictures, qt_videos) VALUES(:qt_id, :qt_setting, :qt_pictures, :qt_videos)");

    query.addBindValue(1);
    query.addBindValue("QtSetting");
    query.addBindValue(1);
    query.addBindValue(3);

    bool status = query.exec();
    if(!status)
    {
        qDebug()<<"qtSqlInit >> inster sql data failure!";
        return false;
    }
    return true;
}

bool setting::queryAll(void)
{
    QSqlDatabase db = QSqlDatabase::database("QSQLITE");
    QSqlQuery query(db);

    query.exec("select * from qtsql");

    QSqlRecord rec = query.record();
    qDebug() << QObject::tr("qtsql表字段数：" ) << rec.count();

    while(query.next())
    {
        for(int index = 0; index < rec.count(); index++)
            qDebug() << query.value(index) << " ";
        qDebug() << "\n";
    }
    return true;
}

bool setting::updateById(int pictures, int videos)
{
    int id = 1;

    QSqlDatabase db = QSqlDatabase::database("QSQLITE");
    QSqlQuery query(db);

    query.prepare(QString("update qtsql set qt_pictures=?, qt_videos=? where qt_id=%1").arg(id));

    query.bindValue(0,pictures);
    query.bindValue(1, videos);


    bool status = query.exec();
    if(!status)
    {
        qDebug()<<"update sql data failure!";
        db.close();
        return false;
    }
    db.close();
    return true;
}

int setting::queryPictures(void)
{
    QSqlDatabase db = QSqlDatabase::database("QSQLITE");
    QSqlQuery query(db);
    int pictures;

    query.exec("select * from qtsql");

    while (query.next())
    {
        pictures = query.value(2).toInt();
        qDebug()<<"the picture number is : " <<QString::number(pictures, 10);
    }
    return pictures;
}

int setting::queryVideos(void)
{
    QSqlDatabase db = QSqlDatabase::database("QSQLITE");
    QSqlQuery query(db);

    int videos;
    query.exec("select * from qtsql");

    while (query.next())
    {
        videos = query.value(3).toInt();
        qDebug()<<"the picture number is : " <<QString::number(videos, 10);
    }
    return videos;
}

void setting::quitSetting(void)
{
    this->close();
}


void setting::saveParameter(void)
{
    /* get input photo numbers */
    QString QStPhoto = ui->photoLineEdit->text();
    int photoNumber = pf.qstringToInt(QStPhoto);

     /* get input recording times  */
    QString QStRecord = ui->recordLineEdit->text();
    int recordTimes = pf.qstringToInt(QStRecord);

    /* update sql, write photo numbers, times to sql */
    bool status = updateById(photoNumber, recordTimes);
    if(!status)
    {
        qDebug()<<"setting >> saveParameter >> inster sql number failure";
        return;
    }
}
