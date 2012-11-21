#ifndef CARDCELL_H
#define CARDCELL_H

#include <QString>
#include <QColor>
#include <QPoint>

class CardCell
{
public:
    CardCell() :
        m_x(INVALID_POS), m_y(INVALID_POS),
        m_textColor(), m_fontFamily(), m_fontLines(0),
        m_pattern(), m_stylesheet() {}
    void assign(const QStringList& list);
    void debugPrint(const char* prefix = 0);

    int pos_x() const { return m_x; }
    int pos_y() const { return m_y; }
    QPoint pos() const { return QPoint(m_x, m_y); }
    QString posAsString() const;
    void setPos(int x, int y) { m_x = x; m_y = y; }
    void setPos(const QPoint& pos) { m_x = pos.x(); m_y = pos.y(); }
    void setPos(const QString& text);
    bool isValid() const { return m_x >= 0 && m_y >= 0; }
    bool isNull() const { return m_x == NULL_POS || m_y == NULL_POS; }

    QColor textColor() const { return m_textColor; }
    void setTextColor(QColor color) { m_textColor = color; }

    QString fontFamily() const { return m_fontFamily; }
    void setFontFamily(const QString& family) { m_fontFamily = family; }

    double fontLines() const { return m_fontLines; }
    void setFontLines(double lines) { m_fontLines = lines; }

    QString pattern() const { return m_pattern; }
    void setPattern(const QString& pattern) { m_pattern = pattern; }

    QString stylesheet() const { return m_stylesheet; }
    void setStylesheet(const QString& style) { m_stylesheet = style; }

    bool operator ==(const CardCell& other) const;
    bool operator !=(const CardCell& other) const { return !(*this == other); }
    CardCell& operator +=(const CardCell& other);

    enum {
        NULL_POS = -2,
        INVALID_POS = -1
    };

    static const CardCell NULL_CELL;

private:
    CardCell(int x, int y) { CardCell(); m_x = x; m_y = y; }

    int     m_x, m_y;
    QColor  m_textColor;
    QString m_fontFamily;
    double  m_fontLines;
    QString m_pattern;
    QString m_stylesheet;
};

#endif // CARDCELL_H
