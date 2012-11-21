#ifndef SETUPSPLITCELL_H
#define SETUPSPLITCELL_H

#include <QMetaType>
#include "card/cardcell.h"
#include "card/card.h"

class SetupSplitCell : public CardCell
{
public:
    SetupSplitCell(int index = Card::INVALID_INDEX,
                   const CardCell& cell = NULL_CELL) :
        CardCell(cell), m_index(index) {}

    int index() const { return m_index; }
    void setIndex(int index) { m_index = index; }

private:
    int m_index;
};
Q_DECLARE_METATYPE(SetupSplitCell)

#endif // SETUPSPLITCELL_H
