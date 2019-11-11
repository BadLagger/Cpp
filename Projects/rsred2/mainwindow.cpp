#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//  инициализация первого порта
    port1 = new ComPort(ui->P1Cbox);
    port1->setSpeedCbox(ui->P1SpeedCbox);
    port1->setParityCbox(ui->P1ParityCbox);
    port1->setBitCbox(ui->P1BitCbox);
    port1->setStopCbox(ui->P1StopCbox);
    port1->setFlowCbox(ui->P1FlowCtrlCbox);
//  инициализация первого порта
    port2 = new ComPort(ui->P2Cbox);
    port2->setSpeedCbox(ui->P2SpeedCbox);
    port2->setParityCbox(ui->P2ParityCbox);
    port2->setBitCbox(ui->P2BitCbox);
    port2->setStopCbox(ui->P2StopCbox);
    port2->setFlowCbox(ui->P2FlowCtrlCbox);
#ifndef QT_DEBUG
    ui->textEdit->hide();
    ui->textEdit_2->hide();
    setMinimumHeight(79);
    setMaximumHeight(79);
#else
    setMinimumHeight(235);
    setMaximumHeight(235);
#endif
// Нажатие кнопки конект/дисконект
    connect(ui->pushButton, &QPushButton::clicked, [=]{
       static bool state = false;

       if(!state){
           state = true;
           ui->pushButton->setText("Disconnect");
           port1->openPort();
           port2->openPort();
       } else {
           state = false;
           ui->pushButton->setText("Connect");
           port1->closePort();
           port2->closePort();
       }
    });
// Ошибка открытия порта 1
    connect(port1, &ComPort::errorSignal, [=](QString err){
        if(port2->isOpen())
            port2->closePort();
        QMessageBox msg(QMessageBox::Warning, "Ошибка порта 1", err, QMessageBox::Ok, this, Qt::Dialog);
        msg.exec();
    });
// Ошибка открытия порта 1
    connect(port2, &ComPort::errorSignal, [=](QString err){
            if(port1->isOpen())
              port1->closePort();
            QMessageBox msg(QMessageBox::Warning, "Ошибка порта 2", err, QMessageBox::Ok, this, Qt::Dialog);
            msg.exec();
    });
// Приём с первого порта
    connect(port1, &ComPort::readyRead, [=](){
       QByteArray data = port1->readAll();
#ifdef QT_DEBUG
       QString    printStr = "";
       for(int i = 0; i < data.size(); i++)
       {
           printStr += (QString("%1 ").arg(int(data.at(i)), 0, 16).rightJustified(3, '0').right(3).toUpper());
       }
       ui->textEdit->insertPlainText(printStr);
       ui->textEdit->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
#endif
       port2->write(data);
    });
// Приём со второго порта
    connect(port2, &ComPort::readyRead, [=](){
       QByteArray data = port2->readAll();
#ifdef QT_DEBUG
       QString    printStr = "";
       for(int i = 0; i < data.size(); i++)
       {
           printStr += (QString("%1 ").arg(int(data.at(i)), 0, 16).rightJustified(3, '0').right(3).toUpper());
       }
       ui->textEdit_2->insertPlainText(printStr);
       ui->textEdit_2->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
#endif
       port1->write(data);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
