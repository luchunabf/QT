#include "mywidget.h"
#include "./ui_mywidget.h"

MyWidget::MyWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MyWidget)
{
    ui->setupUi(this);

    // 按钮创建：第一种
    QPushButton* btn = new QPushButton;
    btn->setParent(this);
    btn->setText("Qt第一个按钮");
    btn->move(100, 100);


    // 按钮创建：第二种【常用】
    QPushButton* btn1 = new QPushButton("Qt第二个按钮", this);
    //btn->resize(20,10);
    btn->setParent(this);
    btn->move(200, 100);

    this->resize(600, 400);

    this->setWindowTitle("第一个项目");

    this->setFixedSize(800, 600);

    // 最常用的创建按钮的方式
    QPushButton* btn2 = new QPushButton("关闭窗口", this);
    btn2->move(500, 500);

    // 信号和槽
    connect(btn2, &QPushButton::clicked, this, &MyWidget::close);

    // 自定义信号和槽
    student = new Student(this);
    teacher = new Teacher(this);

    connect(teacher, &Teacher::hungry, student, &Student::treat);
    connect(btn, &QPushButton::clicked, this, &MyWidget::ClassIsOver);

}

void MyWidget::ClassIsOver()
{
    emit teacher->hungry();
}

MyWidget::~MyWidget()
{
    delete ui;

}















