#ifndef COMBOXEX_H
#define COMBOXEX_H

#include <QComboBox>

class comboxex : public QComboBox
{
    Q_OBJECT
public:
    explicit comboxex(QWidget *parent = nullptr);


    void adjustItemWidth();

};

#endif // COMBOXEX_H
