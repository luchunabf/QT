#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    setFixedSize(400, 300);
    ui->pwdLineEdit->setFocus(); // 口令框置焦点
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_loginPushButton_clicked()
{
    if(!ui->pwdLineEdit->text().isEmpty())
    {
        QSqlQuery query;
        query.exec("select PassWord from member where MemberID ='"
                   + ui->adminLineEdit->text() + "'");
        query.next(); // 检索第一条记录
        QString pwdMD5 = strToMd5(ui->pwdLineEdit->text());
        //qDebug() << "pwdMD5:" << pwdMD5;
        if(query.value(0).toString() == pwdMD5)
        {
            QDialog::accept(); // 验证通过，对应QDialog的返回值
        }
        else
        {
            QMessageBox::warning(this, tr("口令错误"), tr("请输入正确的口令！"), QMessageBox::Ok);
            ui->pwdLineEdit->clear();
            ui->pwdLineEdit->setFocus();
        }
    }
    else
    {
        ui->pwdLineEdit->setFocus();
    }
}


void LoginDialog::on_exitPushButton_clicked()
{
    QDialog::reject(); // 退出登陆框
}

QString LoginDialog::strToMd5(QString str)
{
    QString strMd5;
    QByteArray qba;
    qba = QCryptographicHash::hash(str.toLatin1(), QCryptographicHash::Md5);
    strMd5.append(qba.toHex());
    return strMd5;
}















