#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPortInfo>
#include <QString>
#include <QDebug>
#include <QTime>
#include <QThreadPool>
#include <QFile>

#include "datamonitoring.h"
#include "threadpool.h"
#include "userport.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    void portrefresh();
    void dispdata(QByteArray arr);

private slots:
    void on_mClear_pushButton_clicked();

    void on_mShowdata_pushButton_clicked();

    void on_mRefresh_pushButton_clicked();

signals:
    void transParam1(int num);
    void portname(QString str);

public:
    datamonitoring *ui_showdata = new datamonitoring;

    QDateTime *mSystem_time = new QDateTime;

    userport *mUserPort = new userport;

    QThread *portThread = new QThread;

    Task *mTask =new Task;

    AcSouceDialog *ui_acset = new AcSouceDialog;

private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
