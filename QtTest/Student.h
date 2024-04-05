#ifndef STUDENT_H
#define STUDENT_H

#include <QObject>
#include <QDebug>

class Student : public QObject
{
    Q_OBJECT

public:
    Student(QObject* parent = nullptr);

public slots:
    void treat();
};

#endif // STUDENT_H
