#ifndef CARDSET_H
#define CARDSET_H

#include <QIODevice>
#include <QList>
#include <QStringList>
#include "card/card.h"
#include "util/csvfile.h"

class CardSet : private QList<Card>
{
public:
    CardSet();
    CardSet(const CardSet& other);
    CardSet& operator=(const CardSet& other);

    typedef QList<Card>::const_iterator const_iterator;
    const_iterator begin() const { return base_t::begin(); }
    const_iterator end() const { return base_t::end(); }

    void clear();
    void append(const Card& card) { base_t::append(card); }
    const Card& get(int index) const;

    int loadCsv(const CardDisplay& defDisplay, CsvFile& file);

private:
    typedef QList<Card> base_t;

    static const Card NONE;

    QList<CardDisplay> m_displays;
};

#endif // CARDSET_H
