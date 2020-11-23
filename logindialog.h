#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include<QHBoxLayout>
#include<QVBoxLayout>
class LoginDialog : public QDialog
{
    Q_OBJECT
public:
    LoginDialog(QWidget *parent = 0);
    ~LoginDialog();
private:
    QHBoxLayout *userNameHLayout;
    QHBoxLayout *passHLayout;
    QHBoxLayout *btHLayout;
    QVBoxLayout *pVbox;
    QLabel *usrLabel;
    QLabel *pwdLabel;
    QLineEdit *usrLineEdit;
    QLineEdit *pwdLineEdit;
    QPushButton *LoginBtn;
    QPushButton *exitBtn;
private slots:
    void login();
    void close();
signals:
    void signal_username(QString);


};

#endif // LOGINDIALOG_H
