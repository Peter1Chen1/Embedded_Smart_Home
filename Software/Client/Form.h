#ifndef FORM_H
#define FORM_H

#include <QWidget>

#include <QTcpSocket>
#include <QDateTime>
#include <QTimer>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();

signals:
    void show_mainwindow();//信号的声明


private slots:
    void on_pushButton_back_clicked();

    void label_changed();

    void recv_ip_port(QString str_ip,QString str_port);

    void on_pushButton_led_clicked();

    void on_pushButton_fan_clicked();

    void on_pushButton_spk_clicked();

    void time_showed();

    void on_pushButton_search_clicked();

    void get_weather(QNetworkReply *reply);

private:
    Ui::Form *ui;
    //MainWindow *mw;
    QTcpSocket *tcp;
    QNetworkAccessManager *manager;
};

#endif // FORM_H
