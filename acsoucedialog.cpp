#include "acsoucedialog.h"
#include "ui_acsoucedialog.h"

AcSouceDialog::AcSouceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AcSouceDialog)
{
    ui->setupUi(this);

    QList<QString> mode;
    for (int i = 0; i < 24; ++i) {
        mode << tr("模块%1").arg(i+1);
    }

    ui->id_comboBox->addItems(mode);
    ui->volt_lineEdit->setValidator(new QIntValidator(30,280,this));
    ui->fre_lineEdit->setValidator(new QIntValidator(10,70,this));
}

AcSouceDialog::~AcSouceDialog()
{
    delete ui;
}

void AcSouceDialog::on_volt_pushButton_clicked()
{
    int id = ui->id_comboBox->currentIndex() + 1;
    int data = ui->volt_lineEdit->text().toInt();

    if(data >= 30 && data <= 280){
        emit setcmd(id , 1423,data);
    }
    else{
        QMessageBox::critical(this,"错误","输入数据超出范围",QMessageBox::Ok);
    }

}


void AcSouceDialog::on_fre_pushButton_clicked()
{
    int id = ui->id_comboBox->currentIndex() + 1;
    int data = ui->fre_lineEdit->text().toInt() * 10;

    if(data >= 100 && data <= 700){
        emit setcmd(id , 1424, data);
    }
    else{
        QMessageBox::critical(this,"错误","输入数据超出范围",QMessageBox::Ok);
    }

}

