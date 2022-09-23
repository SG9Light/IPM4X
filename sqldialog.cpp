#include "sqldialog.h"
#include "ui_sqldialog.h"

sqlDialog::sqlDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sqlDialog)
{
    ui->setupUi(this);

    creatorsql();
    creatortable();
    quertable();
}

sqlDialog::~sqlDialog()
{
    delete ui;
}

//创建本地数据库
void sqlDialog::creatorsql(void)
{
    m_DB = QSqlDatabase::addDatabase("QSQLITE");
    m_DB.setDatabaseName("dataCache.db");

    if(m_DB.open() == true){
        qDebug() << "数据库打开成功";
    }
    else{
        qDebug() << "数据库打开失败";
    }
}
//创建本地数据库表
void sqlDialog::creatortable(void)
{
    QSqlQuery query;
    QString str = "CREATE TABLE dataCache ("
                  "序号 INT PRIMARY KEY NOT NULL,"
                  "名称 TEXT NOT NULL,"
                  "数值 INT NOT NULL,"
                  "单位状态 TEXT NOT NULL,"
                  "更新时间 TEXT NOT NULL"
                  ") ";

    if(query.exec(str) == false){
        qDebug()<< str ;
    }
    else{
        qDebug()<<"数据表创建成功";
    }

}
//遍历本地数据库表
void sqlDialog::quertable(void)
{
    QString str = "SELECT * FROM dataCache";
    m_model.setQuery(str);

    ui->tableView->setModel(&m_model);
}

//将数据写入数据库
void sqlDialog::writetable(int order, QString name, int value, QString state, QString time)
{
    QSqlQuery query;
    QString str = QString("INSERT INTO dataCache VALUES(%1,'%2',%3,'%4','%5')"
                          ).arg(order).arg(name).arg(value).arg(state).arg(time);

    if(query.exec(str) == false){
        qDebug()<< str ;
    }
    else{
        qDebug()<<"数据写入成功";
    }

}
