#include "logindialog.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent):QDialog(parent)
{

    usrLabel = new QLabel(this);
    usrLabel->move(70,80);
    usrLabel->setText("用户名");
    usrLineEdit = new QLineEdit(this);
    usrLineEdit->move(140,80);
    usrLineEdit->setPlaceholderText("请输入用户名");
    userNameHLayout=new QHBoxLayout();
    userNameHLayout->addStretch();
    userNameHLayout->addWidget(usrLabel);
    userNameHLayout->addWidget(usrLineEdit);
    userNameHLayout->addStretch();

    pwdLabel = new QLabel(this);
    pwdLabel->move(70,130);
    pwdLabel->setText("密码");
    pwdLineEdit = new QLineEdit(this);
    pwdLineEdit->move(140,130);
    pwdLineEdit->setPlaceholderText("请输入密码");
    passHLayout=new QHBoxLayout();
    passHLayout->addStretch();
    passHLayout->addWidget(pwdLabel);
    passHLayout->addWidget(pwdLineEdit);
    passHLayout->addStretch();

    LoginBtn = new QPushButton(this);
    LoginBtn->move(70,200);
    LoginBtn->setText("登录");
    exitBtn = new QPushButton(this);
    exitBtn->move(220,200);
    exitBtn->setText("退出");
    connect(LoginBtn,SIGNAL(clicked(bool)),this,SLOT(login()));
    connect(exitBtn,SIGNAL(clicked(bool)),this,SLOT(close()));
    btHLayout = new QHBoxLayout();
    btHLayout->addStretch();
    btHLayout->addWidget(LoginBtn);
    btHLayout->addWidget(exitBtn);
    btHLayout->addStretch();
    pVbox = new QVBoxLayout();
    pVbox->addStretch();
    pVbox->addLayout(userNameHLayout);
    pVbox->addLayout(passHLayout);
    pVbox->addLayout(btHLayout);
    pVbox->addStretch();
    this->setLayout(pVbox);


}

LoginDialog::~LoginDialog(){

}

void LoginDialog::login(){
    if(usrLineEdit->text().trimmed()=="123"&&pwdLineEdit->text().trimmed()=="123"){
        accept();
    }else{
        QMessageBox::warning(this,"警告","密码或者用户名错误！",QMessageBox::Yes);
        usrLineEdit->clear();
        pwdLineEdit->clear();
        usrLineEdit->setFocus();
    }
}

void LoginDialog::close(){
    this->close();
}
