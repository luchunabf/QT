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

    commodity_model = new QSqlTableModel(this); // 商品信息视图
    // commodity_model->setTable("commodity_inf"); // ???
    commodity_model->setTable("commodityview"); // 数据库创建的视图名称
    commodity_model->select();
    ui->commodityTableView->setModel(commodity_model); // 库存商品记录数据网格信息加载
    ui->preCategoryComboBox->setModel(categoryModel); // 另外一页的商品类别

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
}


void MainWindow::on_prePlaceOrderPushButton_clicked()
{
    // “下单”按钮单击事件过程代码
}


void MainWindow::on_newUploadPushButton_clicked()
{
    // “上传...”按钮单击事件过程代码
}


void MainWindow::on_newPutinStorePushButton_clicked()
{
    // “入库”按钮单击事件过程代码
}


void MainWindow::on_newClearancePushButton_clicked()
{
    // “清仓”按钮单击事件过程代码
}

