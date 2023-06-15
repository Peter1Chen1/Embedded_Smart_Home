#include "Client.h"
#include "ui_Client.h"

Client::Client(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);
    form = new Form;

    connect(form,SIGNAL(show_mainwindow()),this,SLOT(mainwindow_showed()));
    //发送信号的对象，SIGNAL（信号名），接收信号的对象/槽函数的拥有者，SLOT（槽函数名）
    connect(this,SIGNAL(change_label()), form, SLOT(label_changed()));
    connect(this,SIGNAL(send_ip_port(QString,QString)), form, SLOT(recv_ip_port(QString,QString)));

    /*QPalette palette = this->palette();
    setMask(QPixmap("").mask());*/
}

Client::~Client()
{
    delete ui;
}

void Client::on_pushButton_login_clicked()
{
    if(ui->lineEdit_user->text() == NULL || ui->lineEdit_psw->text() == NULL)
    {
        QMessageBox::information(NULL,("warn"),("IP and Port can't be empty"),QMessageBox::Ok);
    }
    else
    {
        QString str_ip = ui->lineEdit_user->text();
        QString str_port = ui->lineEdit_psw->text();
        emit send_ip_port(str_ip,str_port);
        this->hide();   //隐藏
        //this->close();  //关闭
        form->show();
    }
}

void Client::mainwindow_showed()
{
    this->show();
    form->close();
}
