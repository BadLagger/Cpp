#include "additionalsetswindow.h"
#include "ui_additionsetswindow.h"

/*!
 * \brief     Конструктор класса AdditionalSetsWindow
 * \remark    Основные связи элементов ГУИ и логики приложения находятся здесь
 * \param[in] указатель на родительский виджет
 ***********************************************************************************/
AdditionalSetsWindow::AdditionalSetsWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AdditionSetsWindow)
{
    ui->setupUi(this);
    setWindowModality(Qt::ApplicationModal);
    //  Установка флагов окна
    Qt::WindowFlags flags = windowFlags();
    Qt::WindowFlags closeFlag = Qt::WindowMinimizeButtonHint;
    flags = flags & (~closeFlag);
    setWindowFlags(flags);

    connect(ui->CancelBtn, &QPushButton::clicked, this, &AdditionalSetsWindow::close);
    connect(ui->Block1RBtn, &QPushButton::clicked, [=]{
       if(!ui->SetBtn->isEnabled())
           ui->SetBtn->setEnabled(true);
       set = Set_Block1;
    });

    connect(ui->Block2RBtn, &QPushButton::clicked, [=]{
       if(!ui->SetBtn->isEnabled())
           ui->SetBtn->setEnabled(true);
       set = Set_Block2;
    });

    connect(ui->SetBtn, &QPushButton::clicked, [=]{
        emit setBlockSignal(set);
    });

    hide();
}
/*!
 * \brief     Деструктор класса InfoWindow
 ***********************************************************************************/
AdditionalSetsWindow::~AdditionalSetsWindow()
{
    delete ui;
}
/*!
 * \brief Отобразить окно
 ***********************************************************************************/
void AdditionalSetsWindow::showWindow()
{
    ui->SetBtn->setEnabled(false);

    ui->Block1RBtn->setAutoExclusive(false);
    ui->Block1RBtn->setChecked(false);
    ui->Block1RBtn->setAutoExclusive(true);

    ui->Block2RBtn->setAutoExclusive(false);
    ui->Block2RBtn->setChecked(false);
    ui->Block2RBtn->setAutoExclusive(true);

    set = Set_Max;

    show();
}
