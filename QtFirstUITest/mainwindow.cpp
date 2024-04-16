#include "mainwindow.h"
#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //this->setWindowIconText("test");
    this->resize(800, 500);


    // 添加菜单
    QMenuBar* mb = this->menuBar();
    QMenu* menuFile = mb->addMenu("文件(F)");
    QMenu* mwnuEdit = mb->addMenu("编辑(E)");
    mb->addMenu("格式(O)");
    mb->addMenu("查看(V)");
    mb->addMenu("帮助(H)");


}

MainWindow::~MainWindow()
{
}

