#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "mainwindow.h"
#include <QFile>
#include <QMessageBox>
#include "qdom.h" // 用于操作xml中的DOM对象的库

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

private slots:
    void on_loginPushButton_clicked(); // “登录”按钮单击事件
    void showWechatWindow(); // 根据登录验证结果决定是否显示聊天窗口

private:
    Ui::LoginDialog *ui;
    MainWindow *wechatWindow; //指向聊天窗口的指针
    QDomDocument mydoc; // 全局变量用于获取xml中的DOM对象
};

#endif // LOGINDIALOG_H
