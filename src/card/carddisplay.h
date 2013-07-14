#ifndef CARDDISPLAY_H
#define CARDDISPLAY_H

#include <QVector>
#include <QColor>
#include <QMap>
#include "card/cardcell.h"

class CardDisplay : protected QMap<int, CardCell>
{
public:
    CardDisplay();
    CardDisplay(const CardDisplay& other);
    CardDisplay& operator =(const CardDisplay& other);
    bool operator ==(const CardDisplay& other) const;
    bool operator !=(const CardDisplay& other) const { return !(*this == other); }

    CardDisplay& operator +=(const CardDisplay& other);
    CardDisplay& operator +=(const CardDisplay* other);

    void debugPrint() const;

    void clear();
    int maxIndex() const { return isEmpty() ? -1 : base_t::keys().last(); }
    void setMaxIndex(int index);
    CardCell value(int index) const { return base_t::value(index, CardCell::NULL_CELL); }
    void setValue(int index, const CardCell& cell);
    void setValues(const CardDisplay& other);
    void removeValue(int index);

    bool isProtected(int code) { return (m_protect & (01 << code)) != 0; }
    void setProtected(int code, bool value);
    QColor background() const { return m_background; }
    void setBackground(const QColor& color) { m_background = color; }
    int period() const { return m_period; }
    void setPeriod(int period) { m_period = period; }

    const QList<int>& splitX() const { return m_split_x; }
    const QList<int>& splitY() const { return m_split_y; }
    void setSplitX(const QList<int>& split_x) { m_split_x = split_x; }
    void setSplitY(const QList<int>& split_y) { m_split_y = split_y; }
    void setSplitX(int pos, int value) { m_split_x.replace(pos, value); }
    void setSplitY(int pos, int value) { m_split_y.replace(pos, value); }
    void addSplitX(int pos, int value) { m_split_x.insert(pos, value); }
    void addSplitY(int pos, int value) { m_split_y.insert(pos, value); }
    void delSplitX(int pos) { m_split_x.removeAt(pos); }
    void delSplitY(int pos) { m_split_y.removeAt(pos); }

    enum {
        MIN_PERIOD = 300,
        MAX_PERIOD = 60000
    };

private:
    typedef QMap<int, CardCell> base_t;

    unsigned    m_protect;      // Protection flags
    QColor      m_background;   // Background color
    int         m_period;       // Display period, milliseconds
    QList<int>  m_split_x;      // Split proportions (horizontal, pixel/percent)
    QList<int>  m_split_y;      // Split proportions (vertical, pixel/percent)
};

#endif // CARDDISPLAY_H
