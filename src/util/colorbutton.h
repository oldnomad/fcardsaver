#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <QPushButton>

class ColorButton : public QPushButton
{
    Q_OBJECT
public:
    explicit ColorButton(QWidget *parent = 0);

    QColor colorValue() const { return m_color; }
    void setColorValue(QColor color) { m_color = color; updateColor(); }
    
signals:
    
public slots:

private slots:
    void colorPick();
    
private:
    void updateColor();

    QColor m_color;
};

#endif // COLORBUTTON_H
