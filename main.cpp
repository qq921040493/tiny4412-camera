#include "mainwindow.h"
#include "logindialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow *w = new MainWindow;
    LoginDialog dlg;
    if(dlg.exec()==QDialog::Accepted){
        w->show();
    }

    return a.exec();
}
