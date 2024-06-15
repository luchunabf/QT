#include "filecntdlg.h"
#include "ui_filecntdlg.h"

FileCntDlg::FileCntDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FileCntDlg)
{
    ui->setupUi(this);

    myCntSocket = new QTcpSocket(this);
    mySrvPort = 5555;
    connect(myCntSocket, &QTcpSocket::readyRead, this, &FileCntDlg::readChatMsg);
    myFileNameSize = 0;
    myTotalBytes = 0;
    myRcvedBytes = 0;
}

FileCntDlg::~FileCntDlg()
{
    delete ui;
}

void FileCntDlg::getSrvAddr(QHostAddress saddr)
{
    mySrvAddr = saddr;
    createConnToSrv();
}

void FileCntDlg::getLocPath(QString lpath)
{
    myLocPathFile = new QFile(lpath);
}

void FileCntDlg::closeEvent(QCloseEvent *)
{
    on_cntClosePushButton_clicked();
}

void FileCntDlg::createConnToSrv()
{
    myBlockSize = 0;
    myCntSocket->abort();
    myCntSocket->connectToHost(mySrvAddr, mySrvPort);
    // mytime.start();
}

void FileCntDlg::readChatMsg()
{
    QDataStream in(myCntSocket); // 将socket绑定到QDataStream
    in.setVersion(QDataStream::Qt_5_11);
    // float usedTime = mytime.elapsed();
    if(myRcvedBytes <= sizeof(qint64)*2)
    {
        if((myCntSocket->bytesAvailable() >= sizeof(qint64)*2) && (myFileNameSize == 0))
        {
            in >> myTotalBytes >> myFileNameSize; // 接收文件头
            myRcvedBytes += sizeof(qint64)*2;
        }
        if((myCntSocket->bytesAvailable() >= myFileNameSize) && (myFileNameSize != 0))
        {
            in >> myFileName;
            myRcvedBytes += myFileNameSize;
            myLocPathFile->open(QFile::WriteOnly);
            ui->rfileNameLineEdit->setText(myFileName);
        }
        else
        {
            return;
        }
    }
    if(myRcvedBytes < myTotalBytes)
    {
        myRcvedBytes += myCntSocket->bytesAvailable();
        myInputBlock = myCntSocket->readAll();
        myLocPathFile->write(myInputBlock);
        myInputBlock.resize(0);
    }
    ui->recvProgressBar->setMaximum(myTotalBytes);
    ui->recvProgressBar->setValue(myRcvedBytes);
    // double transpeed = myRcvedBytes / usedTime;
    ui->rfileSizeLineEdit->setText(tr("%1").arg(myTotalBytes/(1024*1024)) + " MB");
    ui->recvSizeLineEdit->setText(tr("%1").arg(myRcvedBytes/(1024*1024)) + " MB");
    // ui->ratelabel->setText(tr("%1").arg(transpeed*1000/(1024*1024),0,'f',2) + " MB/s");
    if(myRcvedBytes == myTotalBytes)
    {
        myLocPathFile->close();
        myCntSocket->abort();
        QMessageBox::information(0, QObject::tr("完毕"), "文件接收完成！");
    }
}

void FileCntDlg::on_cntClosePushButton_clicked()
{
    myCntSocket->abort();
    myLocPathFile->close();
    close(); // 关闭窗口
}







































