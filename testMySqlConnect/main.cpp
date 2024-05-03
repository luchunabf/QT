#include "widget.h"

#include <QApplication>
#include <QSqlDatabase>
#include <QFile>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //数据库驱动： ("QSQLITE", "QODBC", "QODBC3", "QPSQL", "QPSQL7")
    // QList("QSQLITE", "QMIMER", "QODBC", "QPSQL")
    // qDebug() << "数据库驱动：" << QSqlDatabase::drivers();


    /*
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setHostName("127.0.0.1");
    db.setPort(3306);//是mysql服务器的端口号
    db.setDatabaseName("mysql");
    db.setUserName("root");
    db.setPassword("1234");
    if (!db.open()) {
    //        qDebug() << QString::fromLocal8Bit("打开失败");
        qDebug() << QString("打开失败");
        return 0;
    }
    else{
    //        qDebug() << QString::fromLocal8Bit("打开成功");
        qDebug() << QString("打开成功");
    }

    QStringList tables = db.tables();
    foreach(QString table, tables)
        qDebug()<<table;




    QSqlQuery query(db); //关联数据库
    query.exec("select * from texttable;"); //使用查询语句
    qDebug() << "query.numRowsAffected()" << query.numRowsAffected();

    while (query.next()) {
        qDebug() << query.value("id").toInt() << " " << query.value("name").toString();


        // for(int i = 0; i < query.numRowsAffected(); i++)
        // {
        //     qDebug()<<query.value(i).toString();
        // }

        qDebug()<<query.value(0).toInt()<< " " <<query.value(1).toString()<<" ";
    }


    // 插入 格式化字符
    query.clear();
    query.prepare("insert into texttable (id, name) "
                  "values (:id, :name)");
    query.bindValue(":id", 3);
    query.bindValue(":name", "cherry");
    if(query.exec())
    {
        qDebug() << "插入数据库成功！";
    }
    else
    {
        qDebug() << "插入数据库成功！";
    }

    */



    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setHostName("127.0.0.1");
    db.setPort(3306);//是mysql服务器的端口号
    db.setDatabaseName("emarket");
    db.setUserName("root");
    db.setPassword("1234");
    if (!db.open()) {
        //        qDebug() << QString::fromLocal8Bit("打开失败");
        qDebug() << QString("打开失败");
        return 0;
    }
    else{
        //        qDebug() << QString::fromLocal8Bit("打开成功");
        qDebug() << QString("打开成功");
    }


    QSqlQuery query(db); //关联数据库
    // QString photoPath = "E:\\QtDocument\\QT\\testMySqlConnect\\11.jpg";
    // QString photoPath = "E:\\QtDocument\\QT\\testMySqlConnect\\12.jpg";
    // QString photoPath = "E:\\QtDocument\\QT\\testMySqlConnect\\13.jpg";
    // QString photoPath = "E:\\QtDocument\\QT\\testMySqlConnect\\31.jpg";
    // QString photoPath = "E:\\QtDocument\\QT\\testMySqlConnect\\22.jpg";
    QString photoPath = "E:\\QtDocument\\QT\\testMySqlConnect\\21.jpg";
    QFile photoFile(photoPath);
    if(photoFile.exists())
    {
        // 存入数据库
        QByteArray picdata;
        photoFile.open(QIODevice::ReadOnly);
        picdata = photoFile.readAll();
        photoFile.close();

        QVariant var(picdata);
        QString sql = "update commodity set picture=? where CommodityID=6";
        query.prepare(sql);
        query.addBindValue(var);
        if(!query.exec())
        {
            QMessageBox::information(0, QObject::tr("提示"), "照片写入失败");
        }
        else
        {
            QMessageBox::information(0, QObject::tr("提示"), "照片写入成功");
        }
    }

    db.close();






    Widget w;
    w.show();
    return a.exec();
}
