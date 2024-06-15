#ifndef FILESRVDLG_H
#define FILESRVDLG_H

#include <QDialog>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QTime>
#include <QTcpServer>
#include <QTcpSocket>

class QFile;
class QTcpServer;
class QTcpSocket;

namespace Ui {
class FileSrvDlg;
}

class FileSrvDlg : public QDialog
{
    Q_OBJECT

public:
    explicit FileSrvDlg(QWidget *parent = nullptr);
    ~FileSrvDlg();
    void cntRefused(); // 被客户端拒绝后的处理方法

protected:
    void closeEvent(QCloseEvent *);

private slots:
    void sndChatMsg(); // 发送消息

    void refreshProgress(qint64 bynum); // 刷新服务器进度条

    void on_openFilePushButton_clicked(); // “...”按钮的槽函数，选择要传输的文件

    void on_sendFilePushButton_clicked(); // “发送”按钮槽函数

    void on_srvClosePushButton_clicked(); // “停止”按钮槽函数

private:
    Ui::FileSrvDlg *ui;
    QTcpServer *myTcpSrv;
    QTcpSocket *mySrvSocket;
    qint16 mySrvPort;

    QFile *myLocPathFile; // 文件对象指针
    QString myPathFile; // 含路径的本地待发送文件名
    QString myFileName; // 文件名（不含路径）

    qint64 myTotalBytes; // 总共要发送的字节数
    qint64 mySendBytes; // 已发送的字节数
    qint64 myBytesTobeSend; // 余下字节数
    qint64 myPayLoadSize; // 有效载荷
    QByteArray myOutputBlock; // 缓存一次发送的数据
    QTime mytime;

signals:
    void sendFileName(QString name);
};

#endif // FILESRVDLG_H






































