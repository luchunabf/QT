#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    ui->pwdLineEdit->setFocus(); // 获取焦点到密码框
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_loginPushButton_clicked()
{
    showWechatWindow();
}

// 实现登录验证功能
void LoginDialog::showWechatWindow()
{
    // 解析xml文件
    QFile file("E:/QtDocument/QT/MyWeChat/userlog.xml"); // 创建xml文件对象
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(0, QObject::tr("错误"), "userlog.xml文件打开失败！");
        return;
    }

    mydoc.setContent(&file);// 将xml对象赋给QDomDocument类型的Qt文档句柄
    file.close();
    QDomElement root = mydoc.documentElement(); // 获取xml文档的dom根元素
    if(root.hasChildNodes())
    {
        QDomNodeList userList = root.childNodes(); // 获取根元素的全部子节点
        bool exist = false; // 指示用户是否存在
        for(int i = 0; i < userList.count(); i++)
        {
            QDomNode user = userList.at(i); // 根据当前索引i获取用户节点元素
            QDomNodeList record = user.childNodes(); // 该用户的全部属性元素
            // 解析出用户名和密码
            QString uname = record.at(0).toElement().text();
            QString pword = record.at(1).toElement().text();
            if(uname == ui->usrLineEdit->text()) // 可直接判断==
            {
                exist = true;
                if(!(pword == ui->pwdLineEdit->text()))
                {
                    QMessageBox::warning(0, QObject::tr("提示"), "密码错误，请重新输入！");
                    ui->pwdLineEdit->clear();
                    ui->pwdLineEdit->setFocus();
                    return;
                }
            }
        }
        if(!exist)
        {
            QMessageBox::warning(0, QObject::tr("提示"), "此用户不存在，请重新输入。");
            ui->usrLineEdit->clear();
            ui->pwdLineEdit->clear();
            ui->usrLineEdit->setFocus();
            return;
        }
        // 用户存在且密码正确
        wechatWindow = new MainWindow(this);
        wechatWindow->setWindowTitle(ui->usrLineEdit->text());
        wechatWindow->show();
    }
}


























