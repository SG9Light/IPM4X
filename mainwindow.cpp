#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->mShowdata_pushButton->setEnabled(false);
    //串口查询
    connect(this,&MainWindow::portname,mUserPort,&userport::rportname);
    QThreadPool::globalInstance()->setMaxThreadCount(4);

    portrefresh();
    connect(ui->mOpen_pushButton,&QPushButton::clicked, this, [=](){
        QList<QString> comport = (ui->mPort_comboBox->currentText()).split(" ");
        emit portname(comport[0]);
    });

    connect(mUserPort,&userport::sendnum, ui_showdata, &datamonitoring::pollpace);

    connect(ui->mOpen_pushButton,&QPushButton::clicked, mUserPort, &userport::openport);
    connect(mUserPort,&userport::openstate, this, [=](int portstate)
    {
        if(portstate == 1){
            ui->mDebug_textEdit->append("串口打开成功");
            ui->mOpen_pushButton->setText("关闭");
            ui->mShowdata_pushButton->setEnabled(true);
        }else if(portstate == 2){
            ui->mDebug_textEdit->append("串口打关闭");
            ui->mOpen_pushButton->setText("打开");
            ui->mShowdata_pushButton->setEnabled(false);
        }else{
            ui->mDebug_textEdit->append("串口打开失败");
            ui->mOpen_pushButton->setText("打开");
            ui->mShowdata_pushButton->setEnabled(false);
        }
    });

    //数据显示
    ui->mDebug_textEdit->setReadOnly(true);
    connect(mUserPort,&userport::txdata, this, [=](QByteArray arr){
        ui->mDebug_textEdit->append(mSystem_time->currentDateTime().toString("hh:mm:ss")+"☺->：");
        dispdata(arr);
    });
    connect(mTask,&Task::u8_rxdata, this, [=](QByteArray arr){
        ui->mDebug_textEdit->append(mSystem_time->currentDateTime().toString("hh:mm:ss")+"<-☻：");
        dispdata(arr);
    });

    //接收数据子线程处理
    connect(mUserPort,&userport::rxdata,mTask,&Task::receData);

    connect(mUserPort,&userport::rxdata,this,[=](){
        QThreadPool::globalInstance()->start(mTask);
    });

    connect(mTask,&Task::taskflash,this,[=](bool ok){
        if(ok == true){
            ui_showdata->upsupdata();
        }
    });

    //AC源设置下发
    connect(ui_acset, &AcSouceDialog::setcmd, mUserPort, &userport::ac_setvcmd);

    connect(ui_showdata,&datamonitoring::open_acui, this, [=](){
        ui_acset->show();
    });

    connect(ui_showdata,&datamonitoring::mainshow, this,[=](){
        this->show();
    });
}

MainWindow::~MainWindow()
{
    delete mSystem_time;
    delete mTask;
    QThreadPool::globalInstance()->waitForDone();

    delete ui;
}

void MainWindow::portrefresh()
{
    ui->mPort_comboBox->clear();
    foreach (const QSerialPortInfo &Info, QSerialPortInfo::availablePorts()) {
        ui->mPort_comboBox->addItem(Info.portName() +"  "+ Info.description());
        ui->mPort_comboBox->adjustItemWidth();
    }
}

void MainWindow::dispdata(QByteArray arr)
{
    QDataStream out(&arr,QIODevice::ReadWrite);    //将字节数组读入
    while(!out.atEnd())
    {
        qint8 outChar = 0;
        out>>outChar;   //每字节填充一次，直到结束
        //十六进制的转换
        QString str = QString("%1").arg(outChar&0xFF,2,16,QLatin1Char('0'));
        ui->mDebug_textEdit->insertPlainText(" ");//每发送两个字符后添加一个空格
        ui->mDebug_textEdit->insertPlainText(str.toUpper());//大写
        ui->mDebug_textEdit->moveCursor(QTextCursor::End);
    }
    ui->mDebug_textEdit->append(tr(" "));
}


void MainWindow::on_mClear_pushButton_clicked()
{
    ui->mDebug_textEdit->clear();
}

void MainWindow::on_mShowdata_pushButton_clicked()
{
    //    ui_showdata->setAttribute(Qt::WA_QuitOnClose,false);
    this->close();
    ui_showdata->show();
}


void MainWindow::on_mRefresh_pushButton_clicked()
{
    portrefresh();
}

