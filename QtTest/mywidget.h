#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QPushButton>

#include "Student.h"
#include "teacher.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MyWidget; }
QT_END_NAMESPACE

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    MyWidget(QWidget *parent = nullptr);
    ~MyWidget();

public:
    void ClassIsOver();

private:
    Ui::MyWidget *ui;
    Student* student;
    Teacher* teacher;
};
#endif // MYWIDGET_H
