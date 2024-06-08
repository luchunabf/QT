#include "mainwindow.h"
#include "logindialog.h"
#include <QApplication>
#include <QProcess> // Qt进程模块

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if(!createMySqlConn())
    {
        QProcess process;
        process.start("D:\\MySql\\bin\\mysqld.exe");
        //第二次尝试连接
        if(!createMySqlConn()) return 1;
    }
    //qDebug() << "数据库连接成功";
    LoginDialog loginDialog;
    if(loginDialog.exec() == QDialog::Accepted) // QDialog需要exec
    {
        MainWindow w;
        w.show();
        return a.exec();
    }
    else
    {
        return 0;
    }




}
