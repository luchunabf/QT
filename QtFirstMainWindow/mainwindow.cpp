#include "mainwindow.h"
#include <QMenuBar>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(800, 600);

    QMenuBar* mb = this->menuBar();
    QMenu* menuFile = mb->addMenu("新建(F)");
    QMenu* menuEdit = mb->addMenu("编辑(E)");

    mb->addMenu("格式(O)");
    mb->addMenu("查看(V)");
    mb->addMenu("帮助(H)");



//    QStatusBar* sb = this->statusBar();
//


}

MainWindow::~MainWindow()
{
}

