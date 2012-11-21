#include "card/card.h"

const QString Card::value(int index) const
{
    if (index < 0 || index >= size())
        return QString();
    return at(index);
}
