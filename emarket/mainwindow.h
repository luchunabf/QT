#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFileDialog>
#include <QBuffer>
#include <QSqlTableModel> // MySql表模型库
#include <QTime>
#include <QPixmap> //图像处理类库
// #include <QTextCodec>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initMainWindow(); // 界面初始化方法
    void onTableSelectChange(int row); // 商品信息数据网格与表单联动
    void showCommodityPhoto(); // 显示商品样照
    void loadPreCommodity(); // 加载“预售订单”页商品名称列表
    void onPreNameComboBoxChange(); // “预售订单”页商品名称与表单联动

private slots:
    // 商品信息数据网格单击事件槽
    void on_commodityTableView_clicked(const QModelIndex &index);
    // 类别与商品名列表联动信息槽
    void on_preCategoryComboBox_currentIndexChanged(int index);
    // 改选商品名信息槽
    void on_preNameComboBox_currentIndexChanged(int index);
    // 售出商品数改变信息槽
    void on_preCountSpinBox_valueChanged(int arg1);
    // “出售”按钮单击事件
    void on_preSellPushButton_clicked();
    // “下单”按钮单击事件
    void on_prePlaceOrderPushButton_clicked();
    // “上传...”按钮单击事件
    void on_newUploadPushButton_clicked();
    // “入库”按钮单击事件
    void on_newPutinStorePushButton_clicked();
    // “清仓”按钮单击事件
    void on_newClearancePushButton_clicked();

private:
    Ui::MainWindow *ui;
    QImage myPicImg; // 保存商品样照（界面显示）
    QSqlTableModel *commodity_model; // 访问数据库商品信息视图的模型
    QString myMemberID; // 会员账号
    bool myOrdered; // 是否正在购买（订单已写入数据库）
    int myOrderID; // 订单编号
    float myPaySum; // 当前订单累计需要付款的总金额
};

// 访问数据库的静态方法
static bool createMySqlConn()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setHostName("localhost");
    db.setPort(3306);
    db.setDatabaseName("emarket");
    db.setUserName("root");
    db.setPassword("1234");
    if(!db.open())
    {
        QMessageBox::critical(0, QObject::tr("后台数据库连接失败"),
                              QObject::tr("无法连接数据库，请排除故障后重启程序"),
                              QMessageBox::Cancel);
        return false;
    }
    return true;
}



#endif // MAINWINDOW_H
