// created by Heitor Adao Junior
#include "configdialog.h"
#include "ui_configdialog.h"

ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);
}

ConfigDialog::~ConfigDialog()
{
    delete ui;
}

void ConfigDialog::on_spinBox_valueChanged(int value)
{
    ui->tableWidget->setRowCount(value);
    ui->tableWidget->setColumnCount(value);
}
