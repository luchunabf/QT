#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QSqlQuery> // 查询MySQL的库
#include <QMessageBox>
#include <QCryptographicHash> // 包含MD5算法库
#include <QDebug>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();
    QString strToMd5(QString str); // 将口令字符串转换为MD5加密


private slots:
    void on_loginPushButton_clicked();

    void on_exitPushButton_clicked();

private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
