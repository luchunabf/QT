#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QUdpSocket> // UDP套接字库
#include <QNetworkInterface> // 网络（IP地址）接口类库
#include <QDateTime>
#include <QFile>
#include <QFileDialog>
#include "qdom.h"
#include "filesrvdlg.h" // 服务器类
#include "filecntdlg.h" // 客户端类

class FileSrvDlg;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

enum ChatMsgType{ChatMsg, OnLine, OffLine, SfileName, RefFile}; // 定义五种UDP消息类型

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initMainWindow(); // 窗口初始化
    void onLine(QString name, QString time); // 新用户上线方法
    void offLine(QString name, QString time); // 用户离线方法
    void sendChatMsg(ChatMsgType msgType, QString rmtName = ""); // 发送UDP消息
    QString getLocHostIp(); // 获取本端的IP地址
    QString getLocChatMsg(); // 获取本端的聊天信息内容
    void recvFileName(QString name, QString hostip, QString rmtname, QString filename);

protected:
    void closeEvent(QCloseEvent *); // 重写关闭窗口事件以便发送用户离线通知消息

private slots:
    void on_sendPushButton_clicked();

    void recvAndProcessChatMsg(); // 接收并处理UDP数据报

    void on_searchPushButton_clicked(); // 搜索线上所有用户

    void getSfileName(QString); // 获取服务器类sendFileName()信号发送过来的文件名

    void on_transPushButton_clicked(); // 传输文件按钮单击事件

private:
    Ui::MainWindow *ui;
    QString myname = ""; // 本端当前的用户名
    QUdpSocket *myUdpSocket; // UDP套接字指针
    qint16 myUdpPort; // UDP端口号
    QDomDocument myDoc;
    QString myFileName;
    FileSrvDlg *myfsrv;
};
#endif // MAINWINDOW_H


























