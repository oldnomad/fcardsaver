#include <QColorDialog>
#include "colorbutton.h"

ColorButton::ColorButton(QWidget *parent) :
    QPushButton(parent), m_color()
{
    setAutoDefault(false);
    connect(this, SIGNAL(clicked()), this, SLOT(colorPick()));
    updateColor();
}

void ColorButton::colorPick()
{
    QColor color = QColorDialog::getColor(m_color, this);
    if (color.isValid())
        setColorValue(color);
}

void ColorButton::updateColor()
{
    setText(m_color.name());
    QSize isz = iconSize();
    isz.setHeight(height()/2);
    QPixmap pix(isz);
    pix.fill(m_color);
    setIcon(QIcon(pix));
//    update();
}
