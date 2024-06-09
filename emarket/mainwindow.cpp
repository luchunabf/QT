#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    // QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    // // QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    // // QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    // QTextCodec::setCodecForLocale(codec);


    initMainWindow(); // 执行初始化方法
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initMainWindow()
{
    // 用初始化方法对系统主窗体进行初始化
    ui->stackedWidget->setCurrentIndex(0);
    ui->toolBox->setCurrentIndex(0);

    QSqlQueryModel *categoryModel = new QSqlQueryModel(this); // 商品类别模型数据
    categoryModel->setQuery("select Name from category");
    ui->newCategoryComboBox->setModel(categoryModel); // 商品类别加载【comboBox还可以绑定model数据】
    ui->preCategoryComboBox->setModel(categoryModel); // 另外一页的商品类别

    commodity_model = new QSqlTableModel(this); // 商品信息视图
    commodity_model->setTable("commodityview"); // 数据库创建的视图名称
    commodity_model->select();
    ui->commodityTableView->setModel(commodity_model); // 库存商品记录数据网格信息加载

    loadPreCommodity(); // 在“预售订单”中加载商品信息

    myMemberID = "b02020622";
    myOrdered = false;
    myOrderID = 0;
    myPaySum = 0;

    //后面的几行感觉没意义
    QListWidgetItem *title = new QListWidgetItem;
    title->setText(QString("当前订单【编号%1】").arg(myOrderID));
    title->setTextAlignment(Qt::AlignCenter);
}

void MainWindow::onTableSelectChange(int row)
{
    // 当用户变更选择商品信息数据网格中的条目时执行对应的表单更新
    int r = 1; // 默认索引是1
    if(row != 0) r = ui->commodityTableView->currentIndex().row();
    QModelIndex index;
    index = commodity_model->index(r, 0); // 获取视图中的第一列，名称
    ui->newNameLineEdit->setText(commodity_model->data(index).toString());

    index = commodity_model->index(r, 1); // 获取视图中的第二列，进价
    ui->newInputPriceLineEdit->setText(commodity_model->data(index).toString());

    index = commodity_model->index(r, 2); // 获取视图中的第三列，售价
    ui->newOutputPriceLineEdit->setText(commodity_model->data(index).toString());

    // 商品样照
    showCommodityPhoto();

    // 实现类别联动
    QSqlQuery query;
    query.exec(QString("select Name from category where CategoryID=("
                       "select CategoryID from commodity where Name=%1)").arg(
                       ui->newNameLineEdit->text()));
    query.next();
    ui->newCategoryComboBox->setCurrentText(query.value(0).toString());
}

void MainWindow::showCommodityPhoto()
{
    // 显示商品样照
    QPixmap photo;
    QModelIndex index;
    QSqlQueryModel *pictureModel = new QSqlQueryModel(this); // 用来查询商品样照的模型数据

    QString name = ui->newNameLineEdit->text();
    pictureModel->setQuery("select Picture from commodity where Name ='" + name + "'");
    index = pictureModel->index(0,0);
    photo.loadFromData(pictureModel->data(index).toByteArray(), "JPG"); // 从MQSQL数据库中读取字节数组类型的照片
    ui->newPictureLabel->setPixmap(photo);

}

void MainWindow::loadPreCommodity()
{
    // “预售订单”页加载显示商品信息
    QSqlQueryModel *commdityNameModel = new QSqlQueryModel(this); // 商品名称模型数据

    commdityNameModel->setQuery(QString("select Name from commodity where CategoryID=("
        "select CategoryID from category where Name='%1')").arg(ui->preCategoryComboBox->currentText()));
    ui->preNameComboBox->setModel(commdityNameModel); // 商品名称列表加载
    // 【当comboBox绑定model之后，更改comboBox选项信息随之改变，主要是因为model跟comboxBox的name有关】
    // 【comboxBox改变-->model改变-->其他与model绑定的信息随之改变】

    onPreNameComboBoxChange();

}

void MainWindow::onPreNameComboBoxChange()
{
    // 在“预售订单”页改选商品名称时联动显示该商品的各信息项
    QSqlQueryModel *preCommodityModel = new QSqlQueryModel(this); // 商品表模型数据
    QString name = ui->preNameComboBox->currentText(); // 当前选中的商品名称
    preCommodityModel->setQuery("select OutputPrice, Amount, Picture from commodity where name='"
                                + name + "'");

    QModelIndex index;
    index = preCommodityModel->index(0, 0); // sql语句查询到的第一个结果：单价
    ui->preOutputPriceLabel->setText(preCommodityModel->data(index).toString());

    index = preCommodityModel->index(0, 1); // sql语句查询到的第二个结果：库存
    ui->preAmountLabel->setText(preCommodityModel->data(index).toString());
    ui->preCountSpinBox->setMaximum(ui->preAmountLabel->text().toInt());

    index = preCommodityModel->index(0, 2); // sql语句查询到的第三个结果：照片
    QPixmap photo;
    photo.loadFromData(preCommodityModel->data(index).toByteArray(), "JPG");
    ui->prePictureLabel->setPixmap(photo);

    // 计算总价
    ui->preTotalLabel->setText(QString::number(ui->preOutputPriceLabel->text().toFloat() *
                                               ui->preCountSpinBox->value()));


}

void MainWindow::on_commodityTableView_clicked(const QModelIndex &index)
{
    onTableSelectChange(1); // 在选择数据网格中不同的商品条目中执行
}


void MainWindow::on_preCategoryComboBox_currentIndexChanged(int index)
{
    loadPreCommodity(); // 下拉列表改变类别时加载对应类下的商品
}


void MainWindow::on_preNameComboBox_currentIndexChanged(int index)
{
    onPreNameComboBoxChange(); // 选择不同商品名联动显示该商品各信息项
}


void MainWindow::on_preCountSpinBox_valueChanged(int arg1)
{
    //修改商品数量时，对应计算总价
    ui->preTotalLabel->setText(QString::number(ui->preOutputPriceLabel->text().toFloat() * arg1));
}


void MainWindow::on_preSellPushButton_clicked()
{
    // “出售”按钮单击事件过程代码
    QSqlQuery query;
    if(!myOrdered)
    {
        query.exec(QString("insert into orders(MemberID, PaySum, PayWay, OTime)"
                           " values('%1', NULL, NULL, NULL)").arg(myMemberID));
        myOrdered = true;
        query.exec(QString("select OrderID from orders where OTime IS NULL"));
        query.next();
        myOrderID = query.value(0).toInt();
    }
    // 开始预售
    query.exec(QString("select CommodityID from commodity where Name='%1'").
               arg(ui->preNameComboBox->currentText()));
    query.next();
    int commodityid = query.value(0).toInt(); // 本次预售商品编号
    int count = ui->preCountSpinBox->value(); // 预售量
    int amount = ui->preCountSpinBox->maximum() - count; // 剩余库存量
    // 开启一个事务
    QSqlDatabase::database().transaction();
    // 新增订单项
    bool insOk = query.exec(QString("insert into orderitems(OrderID, CommodityID, Count)"
                                    " values(%1, %2, %3)").arg(myOrderID).arg(commodityid).arg(count));
    // 更新库存
    bool upOk = query.exec(QString("update commodity set Amount=%1 where CommodityID=%2").arg(amount).arg(commodityid));
    if(insOk && upOk)
    {
        QSqlDatabase::database().commit(); // 提交事务
        onPreNameComboBoxChange();
        // 显示预售清单
        QString curtime = QTime::currentTime().toString("hh:mm:ss");
        QString curname = ui->preNameComboBox->currentText();
        QString curcount = QString::number(count, 10);
        QString curoutprice = ui->preOutputPriceLabel->text();
        QString curtotal = ui->preTotalLabel->text();
        myPaySum += curtotal.toFloat();
        QString sell_record = curtime + " " + "售出：" + curname +"\r\n 数量：" +
                              curcount + "； 单价：" + curoutprice + "￥；总价：" + curtotal + "￥";
        QListWidgetItem *split = new QListWidgetItem;
        split->setText("---.---.---.---.---.---.---.---.---.---.---.---");
        split->setTextAlignment(Qt::AlignCenter);
        ui->sellListWidget->addItem(split);
        ui->sellListWidget->addItem(sell_record);
        ui->prePlaceOrderPushButton->setEnabled(true); // 激活“下单”按钮
        QMessageBox::information(0, QObject::tr("提示"), "已加入订单！");
    }
    else
    {
        QSqlDatabase::database().rollback(); // 回退事务
    }
}


void MainWindow::on_prePlaceOrderPushButton_clicked()
{
    // “下单”按钮单击事件过程代码
    QSqlQuery query;
    QString otime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    // 开启一个事务
    QSqlDatabase::database().transaction();
    // 下订单
    bool ordOk = query.exec(QString("update orders set PaySum=%1, OTime='%2' where OrderID=%3").
                 arg(myPaySum).arg(otime).arg(myOrderID));
    // 确认发货
    bool uptOk = query.exec(QString("update orderitems set Affirm=1, SendGoods=1 where OrderId=%1").
                            arg(myOrderID));
    if(ordOk && uptOk)
    {
        QSqlDatabase::database().commit(); // 事务提交
        ui->prePlaceOrderPushButton->setEnabled(false);
        // 显示下单记录
        QString order_record = "日期：" + otime + "\r\n 订单号：" +
                               QString("%1").arg(myOrderID) + "\r\n 应付款总额：" +
                               QString("%1￥").arg(myPaySum) + "\r\n 下单成功！";
        QListWidgetItem *split = new QListWidgetItem;
        split->setText("***.***.***.***.***.***.***.***.***.***");
        split->setTextAlignment(Qt::AlignCenter);
        ui->sellListWidget->addItem(split);
        ui->sellListWidget->addItem(order_record); // ListWidget中还能添加QString项
        myPaySum = 0;
        QMessageBox::information(0, QObject::tr("提示"), "下单成功！");
        // 刷新视图网格数据（“新品入库”页）
        commodity_model->setTable("commodityview");
        commodity_model->select();
        ui->commodityTableView->setModel(commodity_model);
    }
    else
    {
        QSqlDatabase::database().rollback(); // 事务回退
    }
}


void MainWindow::on_newUploadPushButton_clicked()
{
    // “上传...”按钮单击事件过程代码
    QString pictureName = QFileDialog::getOpenFileName(this, "选择商品照片", ".",
                                                       "Image File(*.png *.jpg *.jpeg *.bmp)");
    if(pictureName.isEmpty())
        return;
    myPicImg.load(pictureName);
    ui->newPictureLabel->setPixmap(QPixmap::fromImage(myPicImg)); // QImage-->QPixmap转换使用QPixmap::fromImage函数
}


void MainWindow::on_newPutinStorePushButton_clicked()
{
    // “入库”按钮单击事件过程代码
    QSqlQuery query;
    query.exec(QString("select CategoryID from category where Name='%1'").
               arg(ui->newCategoryComboBox->currentText())); // 通过控件获取Name
    query.next();
    int categoryid = query.value(0).toInt(); // 查询结果的第一条：将要入库的商品类别
    QString name = ui->newNameLineEdit->text(); // 商品名称
    float inputprice = ui->newInputPriceLineEdit->text().toFloat(); // 进价
    float outputprice = ui->newOutputPriceLineEdit->text().toFloat(); // 售价
    int count = ui->newCountSpinBox->value(); // 入库量
    query.exec(QString("insert into commodity(CategoryID, Name, Picture, InputPrice,"
                       "OutputPrice, Amount) values(%1, '%2', NULL, %3, %4, %5)").arg(categoryid).
               arg(name).arg(inputprice).arg(outputprice).arg(count));
    // 插入照片
    QByteArray picdata;
    QBuffer buffer(&picdata);
    buffer.open(QIODevice::WriteOnly);
    myPicImg.save(&buffer, "JPG"); // 从myPicImg获取照片保存至QBuffer
    QVariant var(picdata);
    QString sqlstr = "update commodity set Picture=? where Name='" + name + "'";
    query.prepare(sqlstr);
    query.addBindValue(var);
    if(!query.exec())
    {
        QMessageBox::information(0, QObject::tr("提示"), "照片写入失败");
    }
    //刷新网格信息
    commodity_model->setTable("commodityview");
    commodity_model->select();
    ui->commodityTableView->setModel(commodity_model); // 刷新数据网格（“新品入库”页）
}


void MainWindow::on_newClearancePushButton_clicked()
{
    // “清仓”按钮单击事件过程代码
    QSqlQuery query;
    query.exec(QString("delete from commodity where Name='%1'").arg(ui->newNameLineEdit->text()));
    // 刷新界面
    ui->newNameLineEdit->setText("");
    ui->newInputPriceLineEdit->setText("");
    ui->newOutputPriceLineEdit->setText("");
    ui->newCountSpinBox->setValue(1);
    ui->newPictureLabel->clear();
    // 刷新视图数据
    commodity_model->setTable("commodityview");
    commodity_model->select();
    ui->commodityTableView->setModel(commodity_model);
}























