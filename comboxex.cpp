#include "comboxex.h"
#include "qabstractitemview.h"


comboxex::comboxex(QWidget *parent)
    : QComboBox{parent}
{
#ifdef detail
    QFont font;
    //设置文字字体
    font.setFamily("宋体");
    //设置文字大小为50像素
    font.setPixelSize(50);
    //设置文字为粗体
    font.setBold(true); //封装的setWeight函数
    //设置文字为斜体
    font.setItalic(true); //封装的setStyle函数
    //设置文字大小
    font.setPointSize(20);
    //设置文字倾斜
    font.setStyle(QFont::StyleItalic);
    //设置文字粗细;enum Weight存在5个值
    font.setWeight(QFont::Light);
    //设置文字上划线
    font.setOverline(true);
    //设置文字下划线
    font.setUnderline(true);
    //设置文字中划线
    font.setStrikeOut(true);
    //设置字间距%
    font.setLetterSpacing(QFont::PercentageSpacing, 300); //300%,100为默认
    //设置字间距像素值
    font.setLetterSpacing(QFont::AbsoluteSpacing, 20); //设置字间距为100像素
    //设置首个字母大写;跟参数有关,也可以设置全部大写AllUppercase
    font.setCapitalization(QFont::Capitalize);
#else
    this->setFont(QFont("Microsoft Yahei", 10));
#endif
}

void comboxex::adjustItemWidth()
{

    QFontMetrics fm(this->font());
    QRect rect;
    int max_len = 0;

    for (int i = 0; i < this->count(); i++)
    {
        rect = fm.boundingRect(this->itemText(i)); //获得字符串所占的像素大小
        if (max_len < rect.width())
        {
            max_len = rect.width();
        }
    }

    max_len *= 1.2;
    int w = qMax(max_len, this->width());
    this->view()->setFixedWidth(w);
}
