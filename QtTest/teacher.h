#ifndef TEACHER_H
#define TEACHER_H

#include <QObject>

class Teacher : public QObject
{
    Q_OBJECT

public:
    Teacher(QObject* parent = nullptr);
signals:
    void hungry();
};

#endif // TEACHER_H
