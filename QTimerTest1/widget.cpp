#include "widget.h"
#include "ui_widget.h"

#define TIMEINTERVAL 2000

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    timerId = 0;
    pictureId = 1;
    timer = new QTimer(this); // 要不要this 要

    ui->label->setScaledContents(true);
    ui->label->setPixmap(QPixmap(":/1.jpg"));
//    ui->label->setPixmap(QPixmap("file:///C:/Users/86177/Desktop/照片墙/IMG_20240324_194834.jpg"));
//    ui->label->setPixmap(QPixmap("C:/Users/86177/Desktop/test.jpg"));


    connect(ui->startTimerButton, &QPushButton::clicked, this, &Widget::startButtonHandle);
    connect(ui->stopTimerButton, &QPushButton::clicked, this, &Widget::stopButtonHandle);

    connect(timer, &QTimer::timeout, this, &Widget::timerHandle);
}

void Widget::startButtonHandle()
{
    timer->start(TIMEINTERVAL);
    qDebug() << "QTimer定时器启动成功";
}


void Widget::stopButtonHandle()
{
    timer->stop();
    qDebug() << "QTimer定时器停止成功";
}

void Widget::timerHandle()
{
    qDebug() << "QTimer定时器运行中...";

    if(pictureId > 4)
        pictureId = 1;
    QString s = ":/";
    s += QString::number(pictureId);
    s += ".jpg";
    ui->label->setPixmap(QPixmap(s));

    pictureId++;


//    QStringList list;
//    for(int i = 1; i <= 4; ++i)
//    {
//        QString s = ":/";
//        s += QString::number(i);
//        s += ".jpg";
//        list.append(s);
//        ui->label->setPixmap(list.at(i));
//    }

//    ui->label->setPixmap(list.at(i));
}







Widget::~Widget()
{
    delete ui;
}

void Widget::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == timerId)
    {
        qDebug() << "定时器运行中...";

        if(pictureId > 4)
            pictureId = 1;
        QString s = ":/";
        s += QString::number(pictureId);
        s += ".jpg";
        ui->label->setPixmap(QPixmap(s));

        pictureId++;
    }
}


void Widget::on_startPushButton_clicked()
{
    timerId = startTimer(TIMEINTERVAL); // 启动定时器
    if (timerId != 0)
    {
        qDebug() << "定时器启动成功";
    }
}


void Widget::on_stopPushButton_clicked()
{
    if (timerId)
    {
        killTimer(timerId); // 停止定时器
        qDebug() << "定时器停止成功";
    }
}

