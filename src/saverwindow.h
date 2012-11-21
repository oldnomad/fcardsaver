#ifndef SAVERWINDOW_H
#define SAVERWINDOW_H

#include <QWidget>
#include <QTimer>
#include "saversettings.h"

class SaverWindow : public QWidget
{
    Q_OBJECT
public:
    explicit SaverWindow(WId window, bool keymode, const SaverSettings& set);

protected:
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void focusOutEvent(QFocusEvent *event);
    virtual void paintEvent(QPaintEvent *event);

private slots:
    void timerUpdate();

private:
    struct paint_context {
        paint_context() : xcnt(0), ycnt(0), xpos(), ypos() {}
        int         xcnt, ycnt;
        QList<int>  xpos, ypos;
    };

    int nextCard();
    paint_context buildPaint() const;
    void paintText(QPainter* paint, const paint_context& ctxt,
                   const CardCell& disp, const QString& text) const;

    bool                 m_keymode;
    int                  m_lastx, m_lasty;
    const SaverSettings& m_set;
    int                  m_index;
    Card                 m_card;
    CardDisplay          m_display;
    QTimer               m_timer;
};

#endif // SAVERWINDOW_H
