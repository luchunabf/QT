#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initMainWindow();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initMainWindow()
{
    myUdpSocket = new QUdpSocket(this);
    myUdpPort = 23232;
    myUdpSocket->bind(myUdpPort, QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);
    connect(myUdpSocket, &QUdpSocket::readyRead, this, &MainWindow::recvAndProcessChatMsg);

    // myfsrv = new FileSrvDlg(this);
    // connect(myfsrv, &FileSrvDlg::sendFileName, this, &MainWindow::getSfileName);
}

void MainWindow::onLine(QString name, QString time)
{
    bool notExist = ui->userListTableWidget->findItems(name, Qt::MatchExactly).isEmpty();
    if(notExist)
    {
        QTableWidgetItem *newuser = new QTableWidgetItem(name);
        ui->userListTableWidget->insertRow(0); // 先插入一行空数据
        ui->userListTableWidget->setItem(0,0,newuser); // 然后再setItem添加数据
        ui->chatTextBrowser->setTextColor(Qt::gray);
        ui->chatTextBrowser->setCurrentFont(QFont("Times New Roman", 12));
        ui->chatTextBrowser->append(tr("%1 %2 上线").arg(time).arg(name));
        sendChatMsg(OnLine);
    }
}

void MainWindow::offLine(QString name, QString time)
{
    int row = ui->userListTableWidget->findItems(name, Qt::MatchExactly).first()->row();
    ui->userListTableWidget->removeRow(row);
    ui->chatTextBrowser->setTextColor(Qt::gray);
    ui->chatTextBrowser->setCurrentFont(QFont("Times New Roman", 12));
    ui->chatTextBrowser->append(tr("%1 %2 离线").arg(time).arg(name));
}

void MainWindow::sendChatMsg(ChatMsgType msgType, QString rmtName)
{
    QByteArray qba;
    QDataStream write(&qba, QIODevice::WriteOnly);
    QString locHostIP = getLocHostIp();
    QString locChatMsg = getLocChatMsg();
    write << msgType << myname; // 消息类型是必发项
    switch (msgType)
    {
        case ChatMsg:
            write << locHostIP << locChatMsg;
            break;
        case OnLine:
            write << locHostIP;
            break;
        case OffLine:
            break;
        case SfileName:
            write << locHostIP << rmtName << myFileName;
            break;
        case RefFile:
            write << locHostIP << rmtName;
            break;
        default:
            break;
    }
    // UDP使用writeDatagram将消息广播出去
    myUdpSocket->writeDatagram(qba, qba.length(), QHostAddress::Broadcast, myUdpPort);
}

QString MainWindow::getLocHostIp()
{
    QList<QHostAddress> addrlist = QNetworkInterface::allAddresses();
    foreach (QHostAddress addr, addrlist) {
        if(addr.protocol() == QAbstractSocket::IPv4Protocol)
            return addr.toString();
    }
    return 0; // QString类型的返回值还能return 0;
}

QString MainWindow::getLocChatMsg()
{
    QString chatmsg = ui->chatTextEdit->toHtml(); // 获取消息输入框的内容（有可能是图片表情包等，因此toHtml）
    ui->chatTextEdit->clear();
    ui->chatTextEdit->setFocus();
    return chatmsg;
}

void MainWindow::recvFileName(QString name, QString hostip, QString rmtname, QString filename)
{

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    sendChatMsg(OffLine);
}

void MainWindow::on_sendPushButton_clicked()
{
    sendChatMsg(ChatMsg);
}

void MainWindow::recvAndProcessChatMsg()
{
    while(myUdpSocket->hasPendingDatagrams()) // udpsocket接收到消息
    {
        QByteArray qba;
        qba.resize(myUdpSocket->pendingDatagramSize());
        myUdpSocket->readDatagram(qba.data(), qba.size()); // 用udpsocket读数据（readDatagram）
        QDataStream read(&qba, QIODevice::ReadOnly);
        int msgType;
        read >> msgType; // 第一个读消息类型
        QString name, hostip, chatmsg, rname, fname;
        QString curtime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

        switch (msgType)
        {
        case ChatMsg:
        {
            read >> name >> hostip >> chatmsg;
            ui->chatTextBrowser->setTextColor(Qt::darkGreen);
            ui->chatTextBrowser->setCurrentFont(QFont("Times New Roman", 14));
            ui->chatTextBrowser->append("【" + name + "】" + curtime);
            ui->chatTextBrowser->append(chatmsg);
            break;
        }
        case OnLine:
            read >> name >> hostip;
            onLine(name, curtime);
            break;
        case OffLine:
            read >> name;
            offLine(name, curtime);
            break;
        case SfileName:
            read >> name >> hostip >> rname >> fname;
            recvFileName(name, hostip, rname, fname);
            break;
        case RefFile:
            read >> name >> hostip >> rname;
            // if(myname == rname) myfsrv->cntRefused();
            break;
        default:
            break;
        }

    }
}


void MainWindow::on_searchPushButton_clicked()
{
    myname = this->windowTitle();
    ui->userLabel->setText(myname);
    sendChatMsg(OnLine);
}

void MainWindow::getSfileName(QString)
{

}


void MainWindow::on_transPushButton_clicked()
{

}





















