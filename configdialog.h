// created by Heitor Adao Junior
#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>

namespace Ui {
class ConfigDialog;
}

class ConfigDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ConfigDialog(QWidget *parent = 0);
    ~ConfigDialog();
    
private slots:
    void on_spinBox_valueChanged(int value);

private:
    Ui::ConfigDialog *ui;
};

#endif // CONFIGDIALOG_H
