#ifndef DATAMONITORING_H
#define DATAMONITORING_H

#include "sqldialog.h"
#include <QWidget>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTableWidget>
#include <QDesktopServices>
#include <QAxObject>
#include <QTimer>

namespace Ui {
class datamonitoring;
}

class datamonitoring : public QWidget
{
    Q_OBJECT

public:
    explicit datamonitoring(QWidget *parent = nullptr);
    ~datamonitoring();

public:
    void modelboxinit(void);

    void disptableinit(void);

    void upsupdata();

    void appendOneRow(int row, QString name, float value, QString unit, QString time);

    void dispalarm(int modelnum);

    void dispsate(int modelnum);

    void dispanalog(int modelnum);

    void settableheader(void);

    void onealarmmonitor(QString name);

    void onestatemonitor(QString name);

    void oneanalogmonitor(QString name);

public slots:
    void pollpace(int n);

private slots:
    void on_mOpenfile_pushButton_clicked();

    void on_mSavefile_pushButton_clicked();

    void on_mAnalog_pushButton_clicked();

    void on_mState_pushButton_clicked();

    void on_mWaring_pushButton_clicked();

    void on_datatype_comboBox_textActivated(const QString &arg1);

    void on_modenum_comboBox_activated(int index);

    void on_msql_pushButton_clicked();

    void on_openacset_pushButton_clicked();

    void on_m_uishow_pushButton_clicked();

signals:
    void open_acui();

    void mainshow();

private:
    Ui::datamonitoring *ui;

    sqlDialog *ui_Sql = new sqlDialog;

    int distype;
};

#endif // DATAMONITORING_H
