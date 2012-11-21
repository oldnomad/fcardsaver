#include "card/cardcell.h"
#include "util/textconv.h"

const CardCell CardCell::NULL_CELL(CardCell::NULL_POS, CardCell::NULL_POS);

void CardCell::assign(const QStringList &list)
{
    int coord[2] = { m_x, m_y };
    bool ok;
    for (int i = 0; i < 2 && i < list.size(); i++)
    {
        ok = false;
        int val = list[i].toInt(&ok);
        if (ok)
            coord[i] = val < 0 ? INVALID_POS : val;
    }
    m_x = coord[0];
    m_y = coord[1];
    if (list.size() < 3)
        return;
    m_textColor = TextConv::toColor(list[2], m_textColor);
    if (list.size() < 4)
        return;
    m_fontFamily = list[3];
    if (list.size() < 5)
        return;
    ok = false;
    double dval = list[4].toDouble(&ok);
    if (ok && dval >= 0)
        m_fontLines = dval;
    if (list.size() < 6)
        return;
    m_pattern = list[5];
    if (list.size() < 7)
        return;
    m_stylesheet = list[6];
}

void CardCell::debugPrint(const char* prefix)
{
    qDebug("%s[%d,%d]: color %s, font '%s'[%.1f], pattern '%s', stylesheet '%s'",
           prefix ? prefix : "", m_x, m_y,
           qPrintable(m_textColor.name()), qPrintable(m_fontFamily), m_fontLines,
           qPrintable(m_pattern), qPrintable(m_stylesheet));
}

QString CardCell::posAsString() const
{
    return QString().sprintf("%d,%d", m_x, m_y);
}

void CardCell::setPos(const QString &text)
{
    QStringList parts = text.split(',');
    while (parts.size() > 2)
        parts.removeLast();
    assign(parts);
}

bool CardCell::operator ==(const CardCell& other) const
{
    return m_x == other.m_x && m_y == other.m_y &&
            m_textColor == other.m_textColor &&
            m_fontFamily == other.m_fontFamily &&
            m_fontLines == other.m_fontLines &&
            m_pattern == other.m_pattern &&
            m_stylesheet == other.m_stylesheet;
}

CardCell& CardCell::operator +=(const CardCell& other)
{
    if (m_x == NULL_POS)
        m_x = other.m_x;
    if (m_y == NULL_POS)
        m_y = other.m_y;
    if (!m_textColor.isValid() || m_textColor.alpha() == 0)
        m_textColor = other.m_textColor;
    if (m_fontFamily.isEmpty())
        m_fontFamily = other.m_fontFamily;
    if (m_fontLines < 1)
        m_fontLines = other.m_fontLines;
    if (m_pattern.isEmpty())
        m_pattern = other.m_pattern;
    if (m_stylesheet.isEmpty())
        m_stylesheet = other.m_stylesheet;
    return *this;
}
