#include "./ui_mainwindow.h"
#include <QPainter>
#include <QDebug>
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow){
    ui->setupUi(this);
}



MainWindow::~MainWindow()
{
    delete ui;
}
