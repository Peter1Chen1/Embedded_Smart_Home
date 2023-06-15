#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include "Form.h"
#include <QMessageBox>
#include <QPixmap>

namespace Ui {
class Client;
}

class Client : public QMainWindow
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = 0);
    ~Client();

signals:
    void change_label();

    void send_ip_port(QString str_ip,QString str_port);

private slots:
    void on_pushButton_login_clicked();

    void mainwindow_showed();//声明槽函数

private:
    Ui::Client*ui;
    Form *form;
};

#endif // CLIENT_H
