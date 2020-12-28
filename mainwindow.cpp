#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->widget->setTriangleSize(16);
    ui->widget->setTotalCount(5);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);
    ui->widget->setCurrentIndex(0);
}


void MainWindow::on_pushButton_clicked()
{
    int r = ui->widget->currentIndicateIndex();
    while (r == ui->widget->currentIndicateIndex())
        r = qrand() % 5;
    ui->widget->moveIndicate(r);
}
