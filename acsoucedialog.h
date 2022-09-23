#ifndef ACSOUCEDIALOG_H
#define ACSOUCEDIALOG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class AcSouceDialog;
}

class AcSouceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AcSouceDialog(QWidget *parent = nullptr);
    ~AcSouceDialog();   

signals:
       void setcmd(int id, int addr, int data);

private slots:
    void on_volt_pushButton_clicked();

    void on_fre_pushButton_clicked();

private:
    Ui::AcSouceDialog *ui;
};

#endif // ACSOUCEDIALOG_H
