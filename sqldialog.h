#ifndef SQLDIALOG_H
#define SQLDIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>

namespace Ui {
class sqlDialog;
}

class sqlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit sqlDialog(QWidget *parent = nullptr);
    ~sqlDialog();
public:
    void creatorsql(void);
    void creatortable(void);
    void quertable(void);
    void writetable(int order, QString name, int value, QString state, QString time);

private:
    Ui::sqlDialog *ui;

    QSqlDatabase m_DB;

    QSqlQueryModel m_model;
};

#endif // SQLDIALOG_H
