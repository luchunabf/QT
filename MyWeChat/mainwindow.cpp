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

    // 创建服务器类对象
    myfsrv = new FileSrvDlg(this);
    // 当服务器sendFileName发送文件信号触发后，执行getSfileName获取服务器类sendFileName()信号发送过来的文件名
    connect(myfsrv, &FileSrvDlg::sendFileName, this, &MainWindow::getSfileName);
}

void MainWindow::onLine(QString name, QString time)
{
    bool notExist = ui->userListTableWidget->findItems(name, Qt::MatchExactly).isEmpty();
    if(notExist)
    {
        // ui->userListTableWidget->setRowCount(1);
        // ui->userListTableWidget->setColumnCount(1);
        // //在末尾插入数据
        // int curRow = ui->userListTableWidget->rowCount();
        // ui->userListTableWidget->insertRow(curRow);

        // QTableWidgetItem *newuser = new QTableWidgetItem;
        // newuser->setText(name);
        // ui->userListTableWidget->setItem(curRow,0,newuser); // 然后再setItem添加数据

        QTableWidgetItem *newuser = new QTableWidgetItem(name);
        // ui->userListTableWidget->setRowCount(2);
        // ui->userListTableWidget->setRowCount(0); // 不加行数也可以insert
        ui->userListTableWidget->setColumnCount(1); // 【必须添加列数，否则数据不显示】
        ui->userListTableWidget->insertRow(0); // 先插入一行空数据
        ui->userListTableWidget->setItem(0,0,newuser); // 然后再setItem添加数据
        ui->chatTextBrowser->setTextColor(Qt::gray);
        ui->chatTextBrowser->setCurrentFont(QFont("Times New Roman", 12));
        ui->chatTextBrowser->append(tr("%1 %2 上线").arg(time).arg(name));
        // sendChatMsg(OnLine); // 【程序崩溃】这里接收到消息之后，继续发送消息，构成死循环
    }
}

void MainWindow::offLine(QString name, QString time)
{
    // 这里未添加判空，直接解引用，当找不到结果时，程序崩溃
    bool notExist = ui->userListTableWidget->findItems(name, Qt::MatchExactly).isEmpty();
    if(notExist)
    {
        return;
    }

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
    // if(addrlist.isEmpty()) return "";
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

// 判断收到UDP消息时，是否要接收该文件
void MainWindow::recvFileName(QString name, QString hostip, QString rmtname, QString filename)
{
    if(myname == rmtname)
    {
        int result = QMessageBox::information(this, tr("收到文件"),
                                              tr("好友 %1 给您发文件：\r\n%2，是否接收？").arg(
                                                                name).arg(filename),
                                              QMessageBox::Yes | QMessageBox::No);
        if(result == QMessageBox::Yes)
        {
            QString fname = QFileDialog::getSaveFileName(0, tr("保存"), filename);
            if(!fname.isEmpty())
            {
                FileCntDlg *fcnt = new FileCntDlg(this);
                fcnt->getLocPath(fname);
                fcnt->getSrvAddr(QHostAddress(hostip));
                fcnt->show();
            }
        }
        else
        {
            sendChatMsg(RefFile, name);
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *)
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
            if(myname == rname) myfsrv->cntRefused();
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

// fname是信号传过来的
void MainWindow::getSfileName(QString fname)
{
    myFileName = fname;
    int row = ui->userListTableWidget->currentRow();
    QString rmtName = "";
    if(row >= 0)
    {
        rmtName = ui->userListTableWidget->item(row, 0)->text(); // 小心程序崩溃
    }

    sendChatMsg(SfileName, rmtName);
}


void MainWindow::on_transPushButton_clicked()
{
    if(ui->userListTableWidget->selectedItems().isEmpty())
    {
        QMessageBox::warning(0, tr("选择好友"), tr("请先选择文件接收方！"), QMessageBox::Ok);
        return;
    }

    myfsrv->show(); // 展示QDialog
}





















