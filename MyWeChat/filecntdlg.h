#ifndef FILECNTDLG_H
#define FILECNTDLG_H

#include <QDialog>
#include <QFile>
#include <QTime>
#include <QTcpSocket>
#include <QHostAddress> // 网络IP地址类库
#include <QMessageBox>

class QTcpSocket;

namespace Ui {
class FileCntDlg;
}

class FileCntDlg : public QDialog
{
    Q_OBJECT

public:
    explicit FileCntDlg(QWidget *parent = nullptr);
    ~FileCntDlg();
    void getSrvAddr(QHostAddress saddr); // 获取服务器（发送端）的IP
    void getLocPath(QString lpath); // 获取本地文件保存路径

protected:
    void closeEvent(QCloseEvent *);

private slots:
    void createConnToSrv(); // 连接到服务器

    void readChatMsg(); // 读取服务器发来的文件数据

    void on_cntClosePushButton_clicked(); // “停止”按钮的槽函数

private:
    Ui::FileCntDlg *ui;
    QTcpSocket *myCntSocket; // 客户端socket
    QHostAddress mySrvAddr; // 服务器地址
    qint16 mySrvPort; // 服务器端口

    qint64 myTotalBytes; // 总共要接收的字节数
    qint64 myRcvedBytes; // 已接受的字节数
    QByteArray myInputBlock; // 缓存一次接收的数据
    quint16 myBlockSize; // 缓存块大小

    QFile *myLocPathFile; // 待收文件对象指针
    QString myFileName; // 待收文件名
    qint64 myFileNameSize; // 文件名大小

    QTime mytime;
};

#endif // FILECNTDLG_H














































































