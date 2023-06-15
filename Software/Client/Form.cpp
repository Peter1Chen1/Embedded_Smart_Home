#include "Form.h"
#include "ui_Form.h"

#include <QDebug>

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    //mv = new MainWindows;
    tcp = new QTcpSocket;
    manager = new QNetworkAccessManager();

    QTimer *timer;
    timer = new QTimer;
    timer->start(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(time_showed()));
    connect(manager,&QNetworkAccessManager::finished, this,&Form::get_weather);

    ui->image_led->setStyleSheet("border-image: url(:/new/prefix1/image/room_led_off.png);");
    ui->image_fan->setStyleSheet("border-image: url(:/new/prefix1/image/fengshan2.png);");
    ui->image_spk->setStyleSheet("border-image: url(:/new/prefix1/image/voice_close.png);");
}

Form::~Form()
{
    delete ui;
}

void Form::on_pushButton_back_clicked()
{
    emit show_mainwindow();//发送信号
    tcp->disconnectFromHost();
    //this->close();
    //mw->show();
}

void Form::label_changed()
{
    //ui->label->setText("123");
}

void Form::recv_ip_port(QString str_ip, QString str_port)
{
    qDebug() << str_ip << str_port;
    int port = str_port.toInt();
    tcp->connectToHost(str_ip,port);
}

void Form::on_pushButton_led_clicked()
{
    if(ui->pushButton_led->text() == "turn on led")
    {
        ui->label_led->setText("led is on");
        ui->pushButton_led->setText("turn off led");
        ui->image_led->setStyleSheet("border-image: url(:/image/room_led_on.png);");
        tcp->write("1");
    }
    else if(ui->pushButton_led->text() == "turn off led")
    {
        ui->label_led->setText("led is off");
        ui->pushButton_led->setText("turn on led");
        ui->image_led->setStyleSheet("border-image: url(:/image/room_led_off.png);");
        tcp->write("2");
    }
}

void Form::on_pushButton_spk_clicked()
{
    if(ui->pushButton_spk->text() == "turn on spk")
    {
        ui->label_spk->setText("spk is on");
        ui->pushButton_spk->setText("turn off spk");
        ui->image_spk->setStyleSheet("border-image: url(:/image/voice_open.png);");
        tcp->write("3");
    }
    else if(ui->pushButton_spk->text() == "turn off spk")
    {
        ui->label_spk->setText("spk is off");
        ui->pushButton_spk->setText("turn on spk");
        ui->image_spk->setStyleSheet("border-image: url(:/image/voice_close.png);");
        tcp->write("4");
    }
}

void Form::on_pushButton_fan_clicked()
{
    if(ui->pushButton_fan->text() == "turn on fan")
    {
        ui->label_fan->setText("fan is on");
        ui->pushButton_fan->setText("turn off fan");
        ui->image_fan->setStyleSheet("border-image: url(:/image/fengshan.png);");
        tcp->write("5");
    }
    else if(ui->pushButton_fan->text() == "turn off fan")
    {
        ui->label_fan->setText("fan is off");
        ui->pushButton_fan->setText("turn on fan");
        ui->image_fan->setStyleSheet("border-image: url(:/image/fengshan2.png);");
        tcp->write("6");
    }
}

void Form::time_showed()
{
    QDateTime time = QDateTime::currentDateTime();
    QString str_time = time.toString("yyyy-MM-dd\n hh:mm:ss dddd");
    ui->label_time->setText(str_time);
}

void Form::on_pushButton_search_clicked()
{
    QNetworkRequest request;
    QString str1 = "http://api.seniverse.com/v3/weather/now.json?key=SwJjKNAIuxjFWii6e&location=";
    QString str2 = ui->comboBox->currentText();
    QString str3 = "&language=zh-Hans&unit=c";
    QString url = str1 + str2 + str3;
    qDebug() << url;

    request.setUrl(QUrl(url));
    manager->get(request);
}

void Form::get_weather(QNetworkReply *reply)
{
    QString str_all = reply->readAll();
    qDebug() << str_all;

    QJsonParseError err;
    QJsonDocument json_recv = QJsonDocument::fromJson(str_all.toUtf8(),&err);
    qDebug() << err.error;

    if(err.error != QJsonParseError::NoError)
    {
        qDebug() << "json format mistake";
    }
    else
    {
        QJsonObject root_obj = json_recv.object();
        QJsonValue results_val = root_obj.value("results");
        if(results_val.isArray())
        {
            QJsonObject results_obj = results_val.toArray().at(0).toObject();
            QJsonObject now_obj = results_obj.value("now").toObject();
            QString str_text = "weather:" + now_obj.value("text").toString();
            QString str_temp = "temperature:" + now_obj.value("temperature").toString() + "℃";
            ui->label_type->setText(str_text);
            ui->label_temp->setText(str_temp);
        }
    }
}
