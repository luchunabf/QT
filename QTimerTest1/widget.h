#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QDebug>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_startPushButton_clicked();

    void on_stopPushButton_clicked();

    void startButtonHandle();
    void stopButtonHandle();

    void timerHandle();

protected:
    virtual void timerEvent(QTimerEvent *event);

private:
    Ui::Widget *ui;
    int timerId;
    QTimer * timer;
    int pictureId;

//private:
//    QTimer* timer;
};
#endif // WIDGET_H
