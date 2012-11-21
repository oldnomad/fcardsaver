#ifndef CARD_H
#define CARD_H

#include <QStringList>
#include "card/carddisplay.h"

class Card : private QStringList
{
public:
    Card(const QStringList& values = QStringList(), const CardDisplay* display = 0) :
        QStringList(values), m_display(display) {}

    const QString value(int index) const;
    const QStringList& values() const { return *this; }
    const CardDisplay* display() const { return m_display; }

    enum {
        INVALID_INDEX = -1
    };

private:
    const CardDisplay* m_display;
};

#endif // CARD_H
