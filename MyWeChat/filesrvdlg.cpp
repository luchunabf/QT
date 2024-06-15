#include "filesrvdlg.h"
#include "ui_filesrvdlg.h"

FileSrvDlg::FileSrvDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FileSrvDlg)
{
    ui->setupUi(this);

    myTcpSrv = new QTcpServer(this);
    mySrvPort = 5555;
    connect(myTcpSrv, &QTcpServer::newConnection, this, &FileSrvDlg::sndChatMsg);
    myTcpSrv->close();
    myTotalBytes = 0;
    mySendBytes = 0;
    myBytesTobeSend = 0;
    myPayLoadSize = 64*1024;
    ui->sendProgressBar->reset();
    ui->openFilePushButton->setEnabled(true);
    ui->sendFilePushButton->setEnabled(false);
}

FileSrvDlg::~FileSrvDlg()
{
    delete ui;
}

// 该函数在主窗口中调用
void FileSrvDlg::cntRefused()
{
    // 如果接收端拒收该文件，则直接关闭服务器
    myTcpSrv->close();
    QMessageBox::warning(0, QObject::tr("提示"), "对方拒绝接收！");
}

void FileSrvDlg::closeEvent(QCloseEvent *)
{
    on_srvClosePushButton_clicked();
}

void FileSrvDlg::sndChatMsg()
{
    ui->sendFilePushButton->setEnabled(false);
    // 获取TCPSocket
    mySrvSocket = myTcpSrv->nextPendingConnection();
    connect(mySrvSocket, &QTcpSocket::bytesWritten, this, &FileSrvDlg::refreshProgress);

    myLocPathFile = new QFile(myPathFile);
    myLocPathFile->open(QFile::ReadOnly);
    myTotalBytes = myLocPathFile->size();

    QDataStream sendOut(&myOutputBlock, QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_5_11);
    // mytime.start();

    // 去除文件路径，保留文件名
    QString curFile = myPathFile.right(myPathFile.size()-myPathFile.lastIndexOf('/')-1);
    // 构造文件头
    sendOut << qint64(0) << qint64(0) << curFile;
    myTotalBytes += myOutputBlock.size();
    sendOut.device()->seek(0); // 读写操作定位从文件头开始
    // 添加实际长度和文件名长度
    sendOut << myTotalBytes << qint64(myOutputBlock.size()-sizeof(qint64)*2);
    // 将文件头通过socket发出，并更新待发送字节数myBytesTobeSend
    myBytesTobeSend = myTotalBytes - mySrvSocket->write(myOutputBlock);
    myOutputBlock.resize(0); // 清空缓存
}

void FileSrvDlg::refreshProgress(qint64 bynum)
{
    qApp->processEvents(); // 用于传输大文件时，界面不会冻结
    mySendBytes += (int)bynum;
    if(myBytesTobeSend > 0)
    {
        myOutputBlock = myLocPathFile->read(qMin(myBytesTobeSend, myPayLoadSize));
        myBytesTobeSend -= (int)mySrvSocket->write(myOutputBlock); // socket继续发送
        myOutputBlock.resize(0); // 再次清空缓存
    }
    else
    {
        myLocPathFile->close();
    }
    ui->sendProgressBar->setMaximum(myTotalBytes);
    ui->sendProgressBar->setValue(mySendBytes);
    ui->sfileSizeLineEdit->setText(tr("%1").arg(myTotalBytes/(1024*1024)) + " MB");
    ui->sendSizeLineEdit->setText(tr("%1").arg(mySendBytes/(1024*1024)) + " MB");

    if(mySendBytes == myTotalBytes)
    {
        myLocPathFile->close();
        myTcpSrv->close();
        QMessageBox::information(0, QObject::tr("完毕"), "文件传输完成！");
    }
}

void FileSrvDlg::on_openFilePushButton_clicked()
{
    myPathFile = QFileDialog::getOpenFileName(this); // 可以打开所有文件类型
    if(!myPathFile.isEmpty())
    {
        // 获取文件名，去除文件路径
        myFileName = myPathFile.right(myPathFile.size() - myPathFile.lastIndexOf('/') - 1);

        ui->sfileNameLineEdit->setText(tr("%1").arg(myFileName));
        ui->sendFilePushButton->setEnabled(true);
        ui->openFilePushButton->setEnabled(false); // 不合理，必须发送之后才能继续打开
    }
}


void FileSrvDlg::on_sendFilePushButton_clicked()
{
    // 开始监听
    if(!myTcpSrv->listen(QHostAddress::Any, mySrvPort))
    {
        QMessageBox::warning(0, QObject::tr("异常"), "打开TCP端口错误，请检查网络连接！");
        close(); // 这里是关闭窗口？是的 基类是QDialog
        return;
    }

    emit sendFileName(myFileName);
}


void FileSrvDlg::on_srvClosePushButton_clicked()
{
    if(myTcpSrv->isListening())
    {
        myTcpSrv->close();
        myLocPathFile->close();
        mySrvSocket->abort(); // 关闭socket
    }
    close();
}
































