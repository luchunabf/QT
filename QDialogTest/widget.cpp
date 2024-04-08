#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    dialog = new QDialog(this);
    connect(ui->modelPushButton, &QPushButton::clicked, [this](){
        //dialog->exec();
//        QDir dir;
//        qDebug() << dir.current();
//        qDebug() << dir.currentPath();
//        qDebug() << dir.homePath();
//        qDebug() << dir.tempPath();
//        qDebug() << dir.rootPath();

//        QDir dir("E:/QtDocument/QT/build-QDialogTest-Desktop_Qt_5_15_2_MinGW_64_bit-Debug/debug/test");
//        if(!dir.exists())
//        {
//            dir.mkdir("E:/QtDocument/QT/build-QDialogTest-Desktop_Qt_5_15_2_MinGW_64_bit-Debug/debug/test");
//            qDebug() << "文件夹已经创建:" << dir.currentPath();
//        }
//        else
//        {
//            qDebug() << "文件夹已经存在:" << dir.currentPath();
//        }

//        dir.cd("E:/QtDocument/QT/build-QDialogTest-Desktop_Qt_5_15_2_MinGW_64_bit-Debug/debug/");
//        dir.cdUp();
//        foreach(QFileInfo item, dir.entryInfoList())
//        {
//            if(item.isDir())
//            {
//                qDebug() << "文件夹" << item.filePath();
//            }
//            else
//            {
//                qDebug() << "文件" << item.filePath();
//            }
//        }

        QFile file("test.txt");
        if(file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            qDebug() << "文件打开成功";
        }
    });

    connect(ui->showPushButton, SIGNAL(clicked()), this, SLOT(showButtonHandle()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::showButtonHandle()
{
    dialog->show();
    qDebug() << "非模态对话框已经创建";
}

