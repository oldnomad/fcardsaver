#include "card/carddisplay.h"
#include "card/card.h"

CardDisplay::CardDisplay()
{
    clear();
}

CardDisplay::CardDisplay(const CardDisplay &other) : base_t(other)
{
    m_protect = other.m_protect;
    m_background = other.m_background;
    m_period = other.m_period;
    m_split_x = other.m_split_x;
    m_split_y = other.m_split_y;
}

CardDisplay& CardDisplay::operator =(const CardDisplay& other)
{
    base_t::operator =(other);
    m_protect = other.m_protect;
    m_background = other.m_background;
    m_period = other.m_period;
    m_split_x = other.m_split_x;
    m_split_y = other.m_split_y;
    return *this;
}

bool CardDisplay::operator ==(const CardDisplay& other) const
{
    return base_t::operator ==(other) &&
            m_protect == other.m_protect &&
            m_background == other.m_background &&
            m_period == other.m_period &&
            m_split_x == other.m_split_x &&
            m_split_y == other.m_split_y;
}

CardDisplay& CardDisplay::operator +=(const CardDisplay& other)
{
    m_protect |= other.m_protect;
    if (other.m_background.isValid() && other.m_background.alpha() != 0)
        m_background = other.m_background;
    if (other.m_period >= MIN_PERIOD && other.m_period <= MAX_PERIOD)
        m_period = other.m_period;
    if (!other.m_split_x.isEmpty())
        m_split_x = other.m_split_x;
    if (!other.m_split_y.isEmpty())
        m_split_y = other.m_split_y;
    base_t list(static_cast<base_t>(other));
    CardCell cell(base_t::value(maxIndex()));
    int i;
    for (i = 0; i <= maxIndex() && i <= other.maxIndex(); i++)
        list[i] += base_t::value(i);
    for (; i <= other.maxIndex(); i++)
        list[i] += cell;
    base_t::operator =(list);
    return *this;
}

CardDisplay& CardDisplay::operator +=(const CardDisplay* other)
{
    if (other)
        *this += *other;
    return *this;
}

void CardDisplay::debugPrint() const
{
    qDebug("PRT: %08X", m_protect);
    qDebug("BGD: %s", qPrintable(m_background.name()));
    qDebug("PER: %d", m_period);
    for (int i = 0; i < m_split_x.size(); i++)
        qDebug("X[%d]: %d", i, m_split_x[i]);
    for (int i = 0; i < m_split_y.size(); i++)
        qDebug("Y[%d]: %d", i, m_split_y[i]);
    for (int i = 0; i <= maxIndex(); i++)
        value(i).debugPrint(qPrintable(QString().sprintf("T[%d]: ", i)));
}

void CardDisplay::clear()
{
    m_protect = 0;
    m_background = QColor();
    m_period = 0;
    m_split_x.clear();
    m_split_y.clear();
    base_t::clear();
}

void CardDisplay::setProtected(int code, bool value)
{
    if (value)
        m_protect |= 01 << code;
    else
        m_protect &= ~(01 << code);
}

void CardDisplay::setMaxIndex(int index)
{
    base_t::iterator it = base_t::upperBound(index);
    while (it != base_t::end())
        it = base_t::erase(it);
}

void CardDisplay::setValue(int index, const CardCell &value)
{
    if (index < 0)
        return;
    base_t::insert(index, value);
}

void CardDisplay::setValues(const CardDisplay &other)
{
    base_t::operator =(other);
}

void CardDisplay::removeValue(int index) {
    base_t::remove(index);
}
